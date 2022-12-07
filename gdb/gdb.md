[gdb调试命令的使用及总结](https://www.cnblogs.com/lsgxeva/p/8024867.html)

## 基本命令
backtrace（或bt）	查看各级函数调用及参数
finish	连续运行到当前函数返回为止，然后停下来等待命令
frame（或f） 帧编号	选择栈帧
info（或i） locals	查看当前栈帧局部变量的值
list（或l）	列出源代码，接着上次的位置往下列，每次列10行
list 行号	列出从第几行开始的源代码
list 函数名	列出某个函数的源代码
next（或n）	执行下一行语句
print（或p）	打印表达式的值，通过表达式可以修改变量的值或者调用函数
quit（或q）	退出gdb调试环境
set var	修改变量的值
start	开始执行程序，停在main函数第一行语句前面等待命令
step（或s）	执行下一行语句，如果有函数调用则进入到函数中

## 调试步骤
### 1. 进入调试
    gdb --args a.out

### 2. 设置断点
    1. break
    break + 行号(main文件) 
    break + 文件名 + 行号
    break + 内存地址
    break + 行号 + if + 条件

    2. tbreak（临时断点，到达后自动删除）
    tbreak + 行号或函数名

    3. watch/awatch（观察点）
    awatch/watch + 变量　　awatch/watch i　　　　设置一个观察点，当变量被读出或写入时程序被暂停 
    rwatch + 变量　　　　　　rwatch i　　　　　　　　设置一个观察点，当变量被读出时，程序被暂停 

    4. catch
     catch　　　　　　　　　　　　　　　　　　设置捕捉点来补捉程序运行时的一些事件。如：载入共享库（动态链接库）或是C++的异常
     tcatch　　　　　　　　　　　　　　　　　　只设置一次捕捉点，当程序停住以后，应点被自动删除

Note:
info breakpoints/watchpoints [n]　　info break　　n表示断点号，查看断点/观察点的情况
clear + 要清除的断点行号　　clear 10　　　　用于清除对应行的断点，要给出断点的行号，清除时GDB会给出提示
delete + 要清除的断点编号　　delete 3　　　　用于清除断点和自动显示的表达式的命令，要给出断点的编号，清除时GDB不会给出任何提示
disable/enable + 断点编号　　disable 3　　　　让所设断点暂时失效/使能，如果要让多个编号处的断点失效/使能，可将编号之间用空格隔开

### 3. 数据操作
display +表达式　　display a　　用于显示表达式的值，每当程序运行到断点处都会显示表达式的值 

whatis + 变量　　whatis i　　显示某个表达式的数据类型

print(p) + 变量/表达式　　p n　　用于打印变量或表达式的值

set + 变量 = 变量值　　set i = 3　　改变程序中某个变量的值


#### 4. 调试运行环境相关命令
1. 调试准备
set args　　set args arg1 arg2　　设置运行参数
show args　　show args　　参看运行参数
cd + 工作目录  切换工作目录
set width + 数目　　set width 70　　设置GDB的行宽

2. 运行
    run　　r/run　　程序开始执行

    step(s)　　s　　进入式，单步执行
    next(n)　　n　　非进入式，单步执行（或者按 Enter）

    finish　　 　　一直运行到函数返回并打印函数返回时的堆栈地址和返回值及参数值等信息
    until + 行数　　u 3　　运行到函数某一行
    continue(c)　　c　　执行到下一个断点或程序结束

    return <返回值>　　return 5　　改变程序流程，直接结束当前函数，并将指定值返回
    call + 函数　　call func　　在当前位置执行所要运行的函数
### 5. 堆栈相关命令
    backtrace/bt　　bt　　用来打印栈帧指针，也可以在该命令后加上要打印的栈帧指针的个数，查看程序执行到此时，是经过哪些函数呼叫的程序，程序“调用堆栈”是当前函数之前的所有已调用函数的列表（包括当前函数）。每个函数及其变量都被分配了一个“帧”，最近调用的函数在 0 号帧中（“底部”帧）

    frame　　frame 1　　用于打印指定栈帧

    info reg　　info reg　　查看寄存器使用情况

    info stack　　info stack　　查看堆栈使用情况

    up/down　　up/down　　跳到上一层/下一层函数

    jump  指定下一条语句的运行点。可以是文件的行号，可以是file:line格式，可以是+num这种偏移量格式。表式着下一条运行语句从哪里开始。相当于改变了PC寄存器内容，堆栈内容并没有改变，跨函数跳转容易发生错误。


### 7. 信号命令
    signal 　　signal SIGXXX 　　产生XXX信号，如SIGINT

    handle 　　在GDB中定义一个信号处理。信号可以以SIG开头或不以SIG开头，可以用定义一个要处理信号的范围（如：SIGIO-SIGKILL，表示处理从SIGIO信号到SIGKILL的信号，其中包括SIGIO，SIGIOT，SIGKILL三个信号），也可以使用关键字all来标明要处理所有的信号。一旦被调试的程序接收到信号，运行程序马上会被GDB停住，以供调试。
            其可以是以下几种关键字的一个或多个：
        　　nostop/stop
        　　　　当被调试的程序收到信号时，GDB不会停住程序的运行，但会打出消息告诉你收到这种信号/GDB会停住你的程序  
        　　print/noprint
        　　　　当被调试的程序收到信号时，GDB会显示出一条信息/GDB不会告诉你收到信号的信息 
        　　pass 
        　　noignore 
        　　　　当被调试的程序收到信号时，GDB不处理信号。这表示，GDB会把这个信号交给被调试程序会处理。 
        　　nopass 
        　　ignore 
        　　　　当被调试的程序收到信号时，GDB不会让被调试程序来处理这个信号。 
        　　info signals 
        　　info handle 
        　　　　可以查看哪些信号被GDB处理，并且可以看到缺省的处理方式

        　　single命令和shell的kill命令不同，系统的kill命令发信号给被调试程序时，是由GDB截获的，而single命令所发出一信号则是直接发给被调试程序的。


## 更多程序运行选项和调试
### 运行环境。 
　　path 可设定程序的运行路径。 
　　show paths 查看程序的运行路径。

　　set environment varname [=value] 设置环境变量。如：set env USER=hchen 

　　show environment [varname] 查看环境变量

### 调试已运行的程序 
　　(1)在UNIX下用ps查看正在运行的程序的PID（进程ID），然后用gdb PID格式挂接正在运行的程序。 

　　(2)先用gdb 关联上源代码，并进行gdb，在gdb中用attach命令来挂接进程的PID。并用detach来取消挂接的进程。

注意：
可以看出，用gdb连接进程后，他会找到运行这个进程所需的全部文件，当前进程关闭后，仍然可以使用**run**在gdb中启动这个程序。



### 暂停 / 恢复程序运行
当进程被gdb停住时，你可以使用**info program**来查看程序的是否在运行，进程号，被暂停的原因。 在gdb中，我们可以有以下几种暂停方式：断点（BreakPoint）、观察点（WatchPoint）、捕捉点（CatchPoint）、信号（Signals）、线程停止（Thread Stops），如果要恢复程序运行，可以使用c或是continue命令。

### 线程（Thread Stops）调试
1. 查看线程号
   info threads
   这个ID是GDB分配的，与系统中的不同
2. 设置断点
   break thread + 行号 / if
例如：
```
　　(gdb) break frik.c:13 thread 28 if bartab > lim 

```

### 调试core文件
Core Dump：Core的意思是内存，Dump的意思是扔出来，堆出来。开发和使用Unix程序时，有时程序莫名其妙的down了，却没有任何的提示(有时候会提示core dumped)，这时候可以查看一下有没有形如core.进程号的文件生成，这个文件便是操作系统把程序down掉时的内存内容扔出来生成的, 它可以做为调试程序的参考
#### (1)生成Core文件
　一般默认情况下，core file的大小被设置为了0，这样系统就不dump出core file了。修改后才能生成core文件。
　　#设置core大小为无限
　　ulimit -c unlimited
　　#设置文件大小为无限
　　ulimit unlimited

　　这些需要有root权限, 在ubuntu下每次重新打开中断都需要重新输入上面的第一条命令, 来设置core大小为无限

core文件生成路径:输入可执行文件运行命令的同一路径下。若系统生成的core文件不带其他任何扩展名称，则全部命名为core。新的core文件生成将覆盖原来的core文件。

1）/proc/sys/kernel/core_uses_pid可以控制core文件的文件名中是否添加pid作为扩展。文件内容为1，表示添加pid作为扩展名，生成的core文件格式为core.xxxx；为0则表示生成的core文件同一命名为core。
可通过以下命令修改此文件：
echo "1" > /proc/sys/kernel/core_uses_pid

2）proc/sys/kernel/core_pattern可以控制core文件保存位置和文件名格式。
可通过以下命令修改此文件：
echo "/corefile/core-%e-%p-%t" > core_pattern，可以将core文件统一生成到/corefile目录下，产生的文件名为core-命令名-pid-时间戳
以下是参数列表:
    %p - insert pid into filename 添加pid
    %u - insert current uid into filename 添加当前uid
    %g - insert current gid into filename 添加当前gid
    %s - insert signal that caused the coredump into the filename 添加导致产生core的信号
    %t - insert UNIX time that the coredump occurred into filename 添加core文件生成时的unix时间
    %h - insert hostname where the coredump happened into filename 添加主机名
    %e - insert coredumping executable name into filename 添加命令名

#### (2)用gdb查看core文件
发生core dump之后, 用gdb进行查看core文件的内容, 以定位文件中引发core dump的行.
```
　　gdb [exec file] [core file]
　　如:
　　gdb ./test core

　　或gdb ./a.out
core-file core.xxxx
```
gdb后, 用bt命令**backtrace**或**where**查看程序运行到哪里, 来定位core dump的文件行.

　　待调试的可执行文件，在编译的时候需要加-g，core文件才能正常显示出错信息
例如：
```
　　1）gdb -core=core.xxxx
　　file ./a.out
　　bt
　　2）gdb -c core.xxxx
　　file ./a.out
　　bt
```

#### (3)用gdb实时观察某进程crash信息

　　启动进程
　　gdb -p PID
　　c
　　运行进程至crash
　　gdb会显示crash信息
　　bt


## vscode中使用gdb
在使用其它 IDE 调试的时候，有时候仅仅使用 IDE 提供的调试按钮不能满足我们的需求，这时候我们会直接在 IDE 提供的 Console 中输入 LLDB/GDB 指令，进行细粒度的控制。但是 VsCode 不像其他 IDE 可以直接输入 LLDB/GDB 指令，VsCode 需要使用 `-exec` 作为前缀来执行调试命令。例如想要打印变量 `v`，输入 `-exec p v` 就可以执行 LLDB/GDB 的 `p v`指令了。

vscode中如果只想打印变量的值，直接输入变量名即可。例如`this`,或者`*this`查看变量中的成员

[vscode 配置gdb调试](https://blog.csdn.net/weixin_42145502/article/details/107455999)

[《100个gdb小技巧》](https://wizardforcel.gitbooks.io/100-gdb-tips/content/info-frame.html)