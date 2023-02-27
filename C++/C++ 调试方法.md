## 打印函数调用栈
### 适用条件
1. 当某个函数被多个函数调用时，查看函数的调用者。例如：构造函数、析构函数
2. 项目中存在很多重名函数时，例如：重载函数
3. 没有 GDB 环境
4. 代码逻辑错误，难以定位函数的调用者
### 注意
- backtrace的实现依赖于栈指针(fp寄存器), 在gcc编译过程中任何非零的优化等级（-On参数）或加入了栈指针优化参数 -fomit-frame-pointer 后多将不能正确得到程序栈信息;
- backtrace_symbols的实现需要符号名称的支持, 在gcc编译过程中需要加入 -rdynamic 参数
- 内联函数没有栈帧, 它在编译过程中被展开在调用的位置；
- 尾调用优化(Tail-call Optimization)将复用当前函数栈, 而不再生成新的函数栈, 这将导致栈信息不能正确被获取。
### 
```C++
#include <execinfo.h>
// 获取当前的调用栈信息，结果存储在buffer中，返回值为栈的深度，参数size限制栈的最大深度，即最大取size步的栈信息。
int backtrace(void **buffer, int size);

// 把backtrace获取的栈信息转化为字符串，以字符指针数组的形式返回，参数size限定转换的深度，一般用backtrace调用的返回值。
char **backtrace_symbols(void *const *buffer, int size);

// 它的功能和backtrace_symbols差不多，只不过它不把转换结果返回给调用方，而是写入fd指定的文件描述符。
void backtrace_symbols_fd(void *const *buffer, int size, int fd);
```

### Demo
```C++
#include<execinfo.h>

void print_stacktrace()
{
    int size = 16;
    void * array[16];
    int stack_num = backtrace(array, size);
    char ** stacktrace = backtrace_symbols(array, stack_num);
    for (int i = 0; i < stack_num; ++i)
    {
        printf("%s\n", stacktrace[i]);
    }
    free(stacktrace);
}
```
2. 使用 lambda 函数，做到即插即用
```C++
#include<execinfo.h>

...

auto print_stacktrace = [](){    
    int size = 16;  // 打印战的深度
    void * array[16];
    int stack_num = backtrace(array, size);
    char ** stacktrace = backtrace_symbols(array, stack_num);
    for (int i = 0; i < stack_num; ++i)
    {
        printf("%s\n", stacktrace[i]);
    }
    free(stacktrace);
    };
print_stacktrace();
```

### 段错误时自动触发 call trace

[在C/C++程序中打印当前函数调用栈](https://blog.csdn.net/rheostat/article/details/8523598)
[在C/C++程序里打印调用栈信息](https://blog.csdn.net/rheostat/article/details/8523598)


## 检测程序中对某个内存的修改
### 使用 gdb 的观测断点 **watch** 监控内存地址，地址中的直修改时会触发断点。
1. **awatch 和 rwatch 命令只能设置硬件监视点**，硬件断点表达式发生改变时立刻中断，而软断点需要等到执行下一条指令之前才中断。
2. **在多线程程序中，软监视断点只能检测到当前线程引发的表达式变化，而硬监视断点可以检测所有线程导致的表达式变化。**

监视点是特殊的断点，在表达式的值改变的时候中断程序。表达式可以是是一个变量的值，或者是由操作符绑定的一个或多个变量，例如’a+b’.
1. `watch *(data type*)addr`
当内存中的值发生改变时触发断点
2. `watch expr [thread threadnum] `
当变量被某个线程改变时中断
3. `rwatch expr [thread threadnum] `
当变量被读时触发断点
4. `awatch expr [thread threadnum] `
读或写时触发断点
```C++
#include <iostream>
using namespace std;

int main()
{
  int array[10];
  cout << &array << endl;

  for (int i = 0; i < 10; i++)
  {
    array[i] = i;
    cout << "i " << i << endl;
  }
  return 0;
}
```
在多线程程序中，如果没有添加合理的锁机制，很容易多线程对内存竞争访问，从而导致内存中的数据被以外修改的现象。特别是当程序中出现大量的指针传递时，很难判定数据是在那个函数中产生了修改。为了定位这个问题，需要监控程序对特定内存的修改操作，从而定位问题产生的原因。
[C/C++程序中内存被非法改写的一个检测方法](https://www.cnblogs.com/CodeComposer/p/5826015.html)

### 设置捕获断点 **`catch`**
**尽量在异常处理函数前设置断点，因为程序的栈还没有进行切换，更容易找到异常产生的精确位置**
```text
catch event

event {
    throw
    catch
    execption   // Ada 异常
    execption 异常名
    assert  // 断言失败异常
    exec 、fork、vfork // 系统函数调用
    load、 load libname 、 unload 、  unload libname  // 加载和卸载动态库
}
```

### 条件断点 `break if expr` `watch if expr`
1. 使用 `condition bnum expression ` 改变断点条件

2. 设置断点的忽略技术 `ignore bnum count `
当忽略计数为正时，断点不会进行条件判断

### 断点命令列表
断点命令列表可以在断点触发时执行一些命令，如打印，设置新断点等。
1. 中断后打印
```text
 break foo if x>0
 commands
 silent //不打印断点中断时产生的输出
 printf “x is %d\n”,x
 cont
 end 
```

```text
 break 403
 commands
 silent
 set x = y + 4
 cont
 end 
```

### 断点菜单
当函数有重载时，判定在哪个函数上加断点


### 信号
```text
handle signal [keywords...]  

keywords {
    nostop 、stop 、 print 、 noprint 、 pass 、 noignore 、nopass 、ignore
}
```

### 多线程
gdb 过以通过 `thread threadno` 指定在特定的线程上加断点。threadno 可以通过 `info threads` 获得。
```text
break linespec thread threadno
break linespec thread threadno if …

break frik.c:13 thread 28 if bartab > lim 
```
需要注意的是：
1. 如果某个线程由于系统调用阻塞，可能导致系统调用过早的返回，如 `sleep`,线程被中断期间，系统的计数器还会继续计数。
2. 当断点触发时，所有线程都会中断，但是当单步执行时，其它线程执行多少个语句是不确定的。当下次中断时，其它线程可能会停在某个语句中间。
3. **在某些操作系统里，可以锁住操作系统的调度器来直让一个线程运行。** `set scheduler-locking mode`

## 内存查看
### 
1. 查看函数的地址
`info funcname`
2. 打印程序的汇编代码
`disas addr`
3. 打印内存
`x[/nfu] addr`
```text
n，重复次数
 10 进制整数；默认是 1。指定显示多长的内存（需要和单元长度 u 一起计算得到）。

f，显示格式
 显示格式和 print 命令的格式一样（’x',’d',’u',’o',’t',’a',’c',’f',’s’），外加 ’i'（表示机器指令格式）。默认是’x'（16进制）。默认格式在用 x 或 print 命令的时候都会改变。

u，单元大小
单元大小如下：
 b 字节
 h 2 节节
 w 4 字节。默认值。
 g 8 字节。
```
4. 打印某个地址的汇编代码
`x/i addr`
### 值历史
 以 print 命令打印的值保存于 GDB 值历史里，要引用此前的值，用’$'后接值历史编号就可以了

 $$n 指倒数 n 近的值；$$2 是比$$靠前一个的值，$$1 和$$相等，$$0 和$相等。

 ```text
 例如，假设刚打印过一个指针指向的结构体，想要查看这个结构体的内容。输入下列命令
就可以了：
 p *$
 如果有一个结构体链表，其结构体里有一个成员 next 指向下一个结构体，可以用下面的
命令来打印下一个结构体的内容：
 p *$.next
可以重复执行这个命令来连续打印这个链表–只需要输入回车键。
 ```
*注意，值历史记录值，不记录表达式。*

### 惯用变量
 GDB 提供了惯用变量，用户可以在 GDB 里用来存储数据，在以后引用。这类变量在 GDB 里全程存在；
1.  `set $foo = *object_ptr `

2. `show convenience `
打印目前为止的惯用变量列表和它们的值。缩写为 show conv。

### 寄存器
在表达式里可以引用系统寄存器内容，将在寄存器名前置$符作为变量来用。寄存器名对于各系统可能不一样；使用 info registers 可以查看系统的寄存器名。

```text
info registers
 打印所有的寄存器名和值，除了浮点和向量寄存器（在选定的堆栈帧里）。
info all-registers
 打印所有的寄存器名和值，包括浮点和向量寄存器（在选定的堆栈帧里）。
info registers regname …
 打印所有指定寄存器对应的值。
```
```text
例如，可以用 16 进制打印程序计数
器：
 p/x $pc
或者打印下一条要执行的指令
 x/i $pc
或者将堆栈指针加 4(注)：
 set $sp += 4 
```
### 使用机器指令单步执行
