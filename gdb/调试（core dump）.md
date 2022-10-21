
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