
## 什么是coredump?
Coredump叫做核心转储，它是进程运行时在突然崩溃的那一刻的一个内存快照。操作系统在程序发生异常而异常在进程内部又没有被捕获的情况下，会把进程此刻内存、寄存器状态、运行堆栈等信息转储保存在一个文件里。

该文件也是二进制文件，可以使用gdb、elfdump、objdump或者windows下的windebug、solaris下的mdb进行打开分析里面的具体内容。

## core文件设置
### 设置core文件的大小
#### linux
1. `ulimit -a`
查看core文件大小

2. `ulimit  -c`
设置core文件的大小，如果这个值为0.则不会产生core文件，这个值太小，则core文件也不会产生，因为core文件一般都比较大。
3. `ulimit  -c unlimited`
设置错哦热文件大小为无限大，则任意情况下都会产生core文件。

#### Windows
需要修改注册表，详情见原文[gdb调试coredump(使用篇)](https://blog.csdn.net/sunxiaopengsun/article/details/72974548)

**注意**
`ulimit`的设置只对当前命令行有效。

#### 永久设置core文件大小
打开`/etc/security/limits.conf`，在该文件的最后加上两行
```
@root soft core unlimited
@root hard core unlimited

```
### 设置core文件路径和命令格式
1. 查看core文件的默认路径
```shell
cat /proc/sys/kernel/core_pattern
```
1. 设置core文件的默认路径
```shell
echo "/tmp/core-%c-%e-%g-%h-%p-%s-%t-%u" > /proc/sys/kernel/core_pattern
```
```text
%c	文件的大小上限
%e	程序的名字
%g	程序进程组ID
%p	程序的ID
%h	主机名
%s	导致本次coredump的信号
%t	存储时间（UTC时间）
%u	进程用户ID
```
#### 永久修改core文件路径
```text
kernel.core_pattern = /var/core_log/core_%e_%t_%p
kernel.core_uses_pid = 0
```
使修改结果马上生效
```shell
sysctl –p
```

### 调试coredump
在带上调试信息的情况下，我们实际上是可以看到core的地方和代码行的匹配位置。但往往正常发布环境是不会带上调试信息的，因为调试信息通常会占用比较大的存储空间，一般都会在编译的时候把-g选项去掉。
#### 定位产生core dump的代码行
1. `bt`|`where`
    查看堆栈
2. `f n`
    跳转到core堆栈的第n帧，（自己函数所在的帧）
3. `disassemble`
   打开该帧函数的反汇编代码
4. `i f` | `info frame`
   查看堆栈寄存器信息
5. x 地址
   根据程序的内存分布查看信息
#### 调试多线程堆栈
1. `info threads`
   查看所有线程正在运行的指令信息
2. `thread apply all bt`
   打开所有线程的堆栈信息
3. `thread apply 5 bt`
   查看指定线程堆栈信息
4. `thread 5`
    进入指定线程栈空间

    
## 参考
[gdb调试coredump(使用篇)](https://blog.csdn.net/sunxiaopengsun/article/details/72974548)


### 堆栈调试
1. 定位要调试的代码
```text
1. attach 线程
gdb -p pid  
2. 查看每个线程正在执行的代码
info threads
3. 查看所有线程的调用堆栈，根据调用栈定位用户线程找到需要调试的线程
thread apply all bt
4. 跳转到指定线程，threadsNum 为 gdb 的线程标号
threads threadsNum
5. 查看线程的调用堆栈
backtrace
6. 跳转到指定栈帧
up 、down、frame id
7. 打印栈帧中执行的指令地址和对应的代码行
frame、frame id、info line
8. 打印指令执行的源码行,(只有Debug模式才会记录源码行)
list、list -、list +、list linenum
9. 打印地址对应的汇编代码
disas 0x00007efd8f088af4
```
2. 控制代码执行
```text
1. 单步执行
step     // 会进入函数
next     // 不会进入函数
finish   // 执行完当前栈帧对应的函数
until    // 继续执行直到越过源码行，例如: 跳过循环的跳转指令,执行到循环结束

2. 继续执行
continue // 继续执行到下一个断点，可使用 ctrl+c 中段
run      // 重新执行
```

3. 查看变量
```text
info args         // 查看，栈帧的参数
info locals       // 查看当前栈帧局部变量
info variables    // 查看所有可访问的全局变量和静态变量 的变量名
info registers    // 查看那所有寄存器中的直
x addr            // 查看内存中的变量
```

4. 编辑源文件
```text
默认使用 /bin/ex 也可以在启动 gdb 之前指定自定义编辑器

export EDITOR=/usr/bin/vi 

edit linenum

```

5. 改变执行
```text
1. 给变量赋值
 print x=4 
 set x=4

2. 跳转到指定位置执行，
jump linespec
jump location
注意：jump 命令除了改变程序计数器之外，不会改变当前堆栈帧，堆栈指针，也不改变任何内存位置和任何寄存器。如果跳转的目标位置和当前位置之间存在栈的操作空间，那么可能导致访问的不是我们想要的值，最好的跳转位置为一个新的的函数重新创建栈帧。

3. 从函数返回
return
return expression    // 将 expression 的结果作为返回值参数

4. 调用调试程序的函数
print expr  // 调用程序并打印函数的返回值
call expr   
```

6. 保存core文件（实在搞不定了，保存现场，以后再调试）
```text
1. 生成core文件
generate-core-file [file]
gcore [file] 

2. 使用 core 文件调试， 从文件里读取符号和纯存储器的内容
gdb ./test test.core
```