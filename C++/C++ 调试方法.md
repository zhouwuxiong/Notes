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
### 内存区域属性
内存区域属性提供了描述由系统内存请求的特殊处理的功能。GDB 使用属性来判断是否允许某些类型的内存访问；是否使用明确的访问宽度；是否缓存系统内存。缺省的，内存区域的描述取自系统（如果当前系统支持的话），但用户可以覆盖被取的区域。
```text
mem lower upper attributes…
 定义一个内存区域，从 lower 到 upper，属性是 attribute…，并将其加入由 GDB 监控的
区域列表。注意，upper==0 是个特殊例子：
 当作系统最大内存地址。（16 位系统里是 0xffff，32 位系统是 0xffffffff）
mem auto
 放弃用户对内存区域的改变，并使用系统提供的区域，如果有的话，如果系统不提供的
话就不适用内存区域。
delete mem nums…
 从 GDB 监控的内存列表里删除内存区域 nums…。
disable mem nums…
 禁止监视内存区域 nums…被禁用的内存区域不会被遗忘。可以在此激活之。
enable mem nums…
 激活监控内存区域 nums…
info mem
 打印所有定义的内存区域列表，每个区域都有下面的列：
 Memory Region Number
 Enabled or Disabled.
 以激活的内存区域标记为’y'。已禁用的内存区域标记为’n'。
 Lo Address
 内存区域最低地址。
 Hi Address
 内存区域最高地址。
 Attributes
 内存区域的属性集。
```
**内存访问模式**
访问模式属性决定 GDB 是否可以对一个内存区域进行读写访问。
```text
ro 内存只读。
wo 内存只写。
rw 内存可读写。默认属性。
```
**内存访问的尺寸**
访问尺寸属性告诉 GDB 使用指定大小的内存访问。通常内存和设备寄存器要求的指定大小的访问匹配。如果不指定访问尺寸属性，GDB 可能使用任意大小的访问。
```text
8 使用 8 位内存访问。
16 使用 16 位内存访问。
32 使用 32 位内存访问。
64 使用 64 位内存访问。
```
**数据缓冲**
 数据缓冲属性设置 GDB 是否缓冲系统内存。由于减少了调试协议的开销，这个属性可以改善性能，与此同时也可能导致错误的结果，因为GDB 不知道 volatile 变量和内存映射寄存器。
```text
cache 激活缓存系统内存。
nocache 禁用缓冲系统内存。默认属性。
```
### 内存访问检查
GDB 可以设置拒绝访问没有明确描述的内存。如果在某个系统下，访问这些内存区域存在不能预料的效果的话，要预防这种状况，或者要提供一个更好的错误检查，都是很大帮助的。

### 在内存和文件之间复制数据
可以用命令 dump，append 和 restore 来在目标内存和文件直线复制数据。dump 和 append 命令将数据写入文件，restore 命令将文件数据读入到内存中。文件可以是二进制，Motorola S-record，Inetl16 进制，或着 Tekrronix16 进制格式的；不过，GDB 只支持将数据附加到二进制文件。

### 
core 文件或者 core dump 记录执行中的进程的内存镜像和状态（例如寄存器值）。它的主用作用是对崩溃的程序事后调试。发生崩溃的程序会自动产生 core 文件。
偶尔的，可能希望在调试程序期间产生 core 文件来保存进程的状态快照。GDB 为此提供了一个特殊的命令。
```text
generate-core-file [file]
gcore [file] 
 为调试进程产生 core dump。可选参数 file 指定存储 core dump 的文件名。如果没有指定，文件名那个默认是’core.pid’，这里 pid是被调试进程的进程 ID。
```

### 缓存远程目标的数据
GDB 可以缓存在调试器和远程目标之间交换的数据。这种缓存通常可以改善性能，因为其可减少由于内存读写所带来的远程协议的开销。

## C 预处理宏
程序可能在某个点定义一个宏，在后面删除这个定义，然后在此后给这个宏提供另外的定义 。因此，在程序里不同点上，同一个宏可能有不同的定义，或者根本就没有定义。如果在当前堆栈帧上，GDB 使用这个帧源代码行范围的宏。否则，GDB 使用当前位置范围的宏；

## 跟踪点
在某些应用程序里，调试器不大可能因为开发者要了解此程序的行为，长时间的中断程序的执行。如果程序的正确性依赖于实时行为，调试器造成的延迟会导致程序根本改变其行为，甚至在代码本省正确的情况下也可能导致失败。不中断程序的执行来观察其行为是非常有用的功能。

使用 GDB 的 trace 或者 collect 命令，可以指定程序里的位置，称为跟踪点，和在跟踪点执行到的时候要计算的任意表达式。稍后在跟踪点执行到的时候，可以用 tfind 命令来查看表达式的值。表达式也可以引用内存里的对象–结构体或者数组。

*跟踪点功能目前只在远程系统上实现 ？？？*
### 设置跟踪点的命令
1. 创建跟踪点
```text
(gdb) trace foo.c:121 // 源文件和行号
 (gdb) trace +2 / 当前行的下两行
 (gdb) trace my function // 函数的第一行代码
 (gdb) trace *my function // 函数的真正开始的地方 
 (gdb) trace *0×2117c4 // 某个地址
```
 trace 可以简写为 tr。
2. 激活和禁用跟踪点
```text
disable tracepoint [num] 
enable tracepoint [num] 
```
3. 跟踪点通过计数
使用通过计数可以自动中止跟踪会话。如果跟踪点通过计数是n，那么跟踪会话会在跟踪点第 n 次执行到的时候自动中止。如果没有指定跟踪点号 num，通过计数命令将设置最近创建的跟踪点。如果没有指定通过计数，那么跟踪会话会在一直执行，直到用户手动终止为止。
`passcount [n [num]] `
4. 跟踪点操作列表
`action [num] `此命令设置在跟踪点执行到时执行的操作。如果没有指定跟踪点号 num，此命令会为最近创建的跟踪点设置操作
```text
 (gdb) trace foo
 (gdb) actions
 Enter actions for tracepoint 1, one per line:
 > collect bar,baz
 > collect $regs
 > while-stepping 12
 > collect $fp, $sp
 > end
 end 
```
```text
 collect expr1, expr2, …
 在跟踪点执行到时，收集指定表达式的结果。此命令可以接受以逗号分隔的任意有效表达式作为参数。另外，全局，静态或本地变量以外的，也支持下列特殊的参数：
 $regs 收集所有寄存器
 $args 收集函数的所有参数
 $locals 收集所有本地变量
```
5. 跟踪点列表
`info tracepoints [num] `
6. 开始和中止跟踪会话
```text
tstart 此命令不需要参数。开始一次跟踪会话，并开始收集数据。如果不保留上一次跟踪会话期间收集的数据的话，可能会带来一些副作用。
tstop 此命令不需要参数。结束一次跟踪会话，并停止收集数据
tstatus 此命令显示当前跟踪数据收集的状态。
 ```
### 使用已收集的数据
跟踪会话结束以后，可以使用 GDB 命令来检查跟踪数据。基本的概念是，在达到跟踪点的时候每次收集一个跟踪快照，此外每次单步跟踪的时候都收集一次快照。所有这些快照都保存与跟踪缓冲区里，并且是从 0 开始连续编号的

如果远程代理指定了某个跟踪快照的话，在接到 GDB 的请求时，它会从缓冲区里读取此快照相应的内存和寄存器，而不是从实际的内存或寄存器里读取内容，反馈给 GDB。这就意味着 GDB 所有命令
（print,info registers,backtrace 等等）都会像正在调试程序期间一样工作，就如同在跟踪点发生时那样。
1.  tfind n
 从缓冲区里选择一个跟踪快照，如果没有指定参数，那么会选择下一个快照。
2. tdump 
打印在当前跟踪快照里所有收集到的数据 
3. save-tracepoints filename 
   令将当前所有跟踪点的定义以及它们的操作和通过计数保存到文件’filename’里， 以便以后的调试会话里使用。

## 查看符号表
以查询在程序里定义的符号（变量名，函数名，类型名）。这些信息位于程序文本段，不会在程序执行时改变的。
## 调试使用覆盖技术的程序
如果程序大到不能放到系统的内存里，你可以使用覆盖技术来规避这个问题。GDB 提供了调试使用覆盖技术的程序的支持。

## 用 GDB 调试不同语言编写的程序
```text 
info address "symbol" 
    显示符号 symbol 数据在何处存储。对于寄存器变量，是说此变量在哪个寄存器里存储。对于非寄存器的本地变量，此命令打印此变量对于堆栈帧的偏移。

info symbol addr
    打印存储于地址 addr 上的符号名。如果在此地址上没有存储符号，GDB 打印最近的符号和偏移量

whatis [arg] 
ptype [arg]

    打印 arg 的数据类型，arg 可以是表达式或数据类型。如果不带参数，打印$的数据类型，值历史里最近的值。
    如果 arg 是表达式，不会真的计算，且此表达式里的任何边际效应（例如赋值或函数调用）操作都不会执行。
    如果 arg 是类型名，可能是一个类型的名字或者 typedef，或者对于 C 代码而言，可以有’class class-name’，’struct struct-tag’，’unionunion-tag’或者’enum enum-tag’形式.

info types regexp
info types
打印所有名字匹配正则表达式 regexp（如果没有参数的话，打印程序里所有的类型）的的类型的简短介绍

info scope location 
列出在指定范围内的所有变量,此命令那个接受位置参数、函数名、源代码行、前面一个’*'的地址，且打印所有此范围内的变量。

info source
 显示当前源文件的信息–就是说，包含当前函数执行点的源文件
 
 ... 太多了,自行查看pdf ...
```

## 改变执行
如果认为在程序中发现了错误，也许你想知道，纠正当前的错误是否能让剩余的运算得到正确的结果。 使用 GDB的执行中改变的功能，通过实践就可以找到答案。例如：将新值存入变量或内存地址、给程序一个信号、在不同的地址上重新执行程序、甚至从函数提前返回。
### 给变量赋值
```text
 print x=4      返回复制结果
 set width=47   不返回结果
 set var width=47   当复制变量与gdb内置变量同名
```
### 在不同的位置上继续执行
```text
jump linespec
jump location
 在给定的行 linespec 或者 location 上继续执行。如果那个位置上有断点的话，执行会立即中断。
```
**jump 命令除了改变程序计数器之外，不会改变当前堆栈帧，堆栈指针，也不改变任何内存位置和任何寄存器**。如果行 linespec 是在当前执行的函数之外的函数里，如果这两个函数的参数模式或者本地变量不一样的话，那么结果就可能很诡异。
### 为程序设置信号
```text
signal signal
 在中断处立即继续执行程序，但立即给程序一个信号 signal。signal 可以是信号名字或信号的编号。
```
### 从函数里返回
```text
return
return expression
 用 return 命令可以取消函数调用的执行。如果指定一个 expression 表达式参数，其值将是此函数的返回值。
 要是运用 return，GDB 将销毁当前选定的堆栈帧（包括此栈内的所有帧）。可以认为是从销毁的堆栈帧中永久返回。
 return 命令不一定非得在最内沉的函数返回，也可以切换到其他外部的函数，其返回结果会被存储在寄存器中
  return 命令不会继续执行程序；它会让程序中断地像函数刚刚返回的状态那样。与此相反，finish 命令（参见 5.2 节[继续和单步执行]，56 页）会继续执行直到此堆栈帧自然返回。
```
### 调用程序函数
```text
print expr
 计算表达式 expr 和打印结果。expr 可以包含调用被调试程序里的函数。
call expr
 计算表达式 expr 而不打印空返回值。
 
 call 是 print 命令的变体，使用它可以用来执行无返回值的函数（也称为 void 函数 ），但不会将 void 返回值打印

 可以为用 print 和 call 命令调用的函数产生一个信号（例如，如果此函数里有个 bug，或者给这个函数传递了错误的参数）。set unwindonsignal 命令决定在此情况下的行为。
```
```text
 set unwindonsignal
 设置如果在 GDB 调用被调试程序里的函数的时候接收到一个信号时，堆栈退绕。如果设置为打开的话，GDB 会退绕到被创建的堆栈上，并且恢复到被调用前的上下文。如果设置为关闭（缺省的），GDB 会在信号接收到的地方上中断。
```
###  为程序打补丁
缺省地，GDB 以只读方式打开包含程序可执行码的文件（或者 core 文件）。这样可以防止意外修改机器码；不过，这也组织了你主动对程序的二进制文件进行打补丁修改。如果希望能对二进制文件进行补丁，可以用 set write 命令明确指定。例如，可能希望打开内部调试标志，或者紧急修改。
```text
set write on
set write off
    如果指定了’set write on’，GDB 以可读可写的方式打开可执行文件和 core 文件；如果指定了’set write off’（缺省方式），GDB 以只读方式打开。如果已经加载了文件，要是让新的 set  write 设置产生效果，必须在改变了设置之后，重新加载它（使用 exec-file 或者 core-file 命令）。
show write
    显示可执行文件和 core 文件的读写的方式。
```
## GDB 文件
使用core dump 文件调试

## 设置调试目标
目标是指被调试程序所拥有的执行环境。通常，GDB 运行于被调试程序所在的相同宿主环境里；在此类环境里，file 或者 core 命令执行的同时，也顺带设置了调试目标。要是需要更灵活的话–例如，在物理上隔离的宿主上运行 GDB，或者通过串口控制单独的系统，甚或通过 TCP/IP 链接实时系统–可以使用 target命令为 GDB 设置目标类型

## 调试远程程序
如果要调试运行于一个不能运行 GDB 的机器上的程序时，使用远程调试就很有帮助了。例如，可以用远程调试操作系统内核，或者在小型的，没有足够的操作系统能力来支持运行完全功能的调试器的系统里调试。 GDB 可以配置特殊串口和 TCP/IP 接口来支持远程调试特殊的调试目标。另外，GDB还有通用串口协议（特指 GDB，不是指任何特殊目标系统），如果你可以用它实现远程代理–远程代理的代码运行于远程系统，用来和 GDB 通讯。 可能还有其它的远程目标在你的 GDB 配置中也可用；可用 help target 来查看。
## 配置相关的信息
### 使用机器指令单步执行


## gprof
1. gcc 自带
2. 只能统计用户态的CPU占用
3. 不使用于多线程，多线程中只会统计主线程的CPU占用
4. 只有在程序正常退出后才会生成报告
```text
要监控到第三方库函数的执行时间，第三方库也必须是添加 –pg 选项编译的。

gprof只能分析应用程序所消耗掉的用户时间.

程序不能以demon方式运行。否则采集不到时间。（可采集到调用次数）

首先使用 time 来运行程序从而判断 gprof 是否能产生有用信息是个好方法。

如果 gprof 不适合您的剖析需要，那么还有其他一些工具可以克服 gprof 部分缺陷，包括 OProfile 和 Sysprof。

gprof对于代码大部分是用户空间的CPU密集型的程序用处明显。对于大部分时间运行在内核空间或者由于外部因素（例如操作系统的 I/O 子系统过载）而运行得非常慢的程序难以进行优化。

gprof 不支持多线程应用，多线程下只能采集主线程性能数据。原因是gprof采用ITIMER_PROF信号，在多线程内只有主线程才能响应该信号。但是有一个简单的方法可以解决这一问题：http://sam.zoy.org/writings/programming/gprof.html

gprof只能在程序正常结束退出之后才能生成报告（gmon.out）。

a) 原因： gprof通过在atexit()里注册了一个函数来产生结果信息，任何非正常退出都不会执行atexit()的动作，所以不会产生gmon.out文件。

b) 程序可从main函数中正常退出，或者通过系统调用exit()函数退出。
```