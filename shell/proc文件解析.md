# ps
 ps命令是最常用的监控进程的命令,通过此命令可以生成系统快照，查看系统中所有运行进程的详细信息。

## 常用参数
### 基本参数
-A ：所有的进程均显示出来，与 -e 具有同样的效用；
-a ：显示现行终端机下的所有进程，包括其他用户的进程；
-u ：以用户为主的进程状态 ；
x ：通常与 a 这个参数一起使用，可列出较完整信息。
### 输出格式
l ：较长、较详细的将该PID 的的信息列出；
j ：工作的格式 (jobs format)
-f ：做一个更为完整的输出。
### 其他参数
-H 显示树状结构

## 常用命令
```Shell
# 可以查看系统中所有的进程,包含其它使用者；
ps aux
# 查看系统中所有的进程，
ps -elf
# 只能看到当前 Shell 产生的进程；
ps -l
```

## ps命令输出信息含义
**ps -elf**
| 名称     | 含义                                      |
| -------- | ----------------------------------------- |
| F        | 进程标志，说明进程的权限                  |
| S、state | 进程状态，和"psaux"命令中的 STAT 状态一致 |
| PRI、NI  | 进程的优先级                              |
| ADDR     | 该进程在内存的哪个位置；                  |
| SZ       | 该进程占用多大内存；                      |
| WCHAN    | 该进程是否运行。"-"代表正在运行；         |
| -        | -                                         |
| VSZ      | 占用虚拟内存的大小（KB）。                |
| RSS      | 占用实际物理内存的大小（KB）              |
| TTY      | 该进程是在哪个终端运行的。                |
| TIME     | 占用 CPU 的运算时间，注意不是系统时间。   |
| COMMAND  | 产生此进程的命令名。                      |

**ps -aux**
| 名称 | 含义                         |
| ---- | ---------------------------- |
| VSZ  | 占用虚拟内存的大小（KB）。   |
| RSS  | 占用实际物理内存的大小（KB） |

### 进程权限
常见的标志有两个: 
1：进程可以被复制，但是不能被执行；
4：进程使用超级用户权限；

### 执行终端
其中，tty1 ~ tty7 代表本地控制台终端（可以通过 Alt+F1 ~ F7 快捷键切换不同的终端），tty1~tty6 是本地的字符界面终端，tty7 是图形终端。pts/0 ~ 255 代表虚拟终端，一般是远程连接的终端，第一个远程连接占用 pts/0，第二个远程连接占用 pts/1，依次増长。 

### 进程状态
常见的状态有以下几种：
-D：不可被唤醒的睡眠状态，通常用于 I/O 情况。
-R：该进程正在运行。
-S：该进程处于睡眠状态，可被唤醒。
-T：停止状态，可能是在后台暂停或进程处于除错状态。
-W：内存交互状态（从 2.6 内核开始无效）。
-X：死掉的进程（应该不会出现）。
-Z：僵尸进程。进程已经中止，但是部分程序还在内存当中。
-<：高优先级（以下状态在 BSD 格式中出现）。
-N：低优先级。
-L：被锁入内存。
-s：包含子进程。
-l：多线程（小写 L）。
-+：位于后台。

## ps Demo
1. 查看进程及子线程的cpu占用率 
```shell
ps  -eLo pid,lwp,pcpu | grep 43258
```

# top
### top线程模式
top默认以进程的形式展示输出结果，这导致多线程程序的CPU占用率往往会超过100%，可以使用`top -H`命令查看每个线程的CPU占用率
```shell
top -H
top -H -p pid
```
# /proc文件系统
/proc 文件系统是一个虚拟文件系统，通过它可以使用一种新的方法在 Linux内核空间和用户间之间进行通信。在 /proc 文件系统中，我们可以将对虚拟文件的读写作为与内核中实体进行通信的一种手段，但是与普通文件不同的是，这些虚拟文件的内容都是动态创建的。

/proc中的每个文件都提供了有关这个特殊进程的详细信息。
/proc 中另外一些有趣的文件有：
cpuinfo，它标识了处理器的类型和速度；
pci，显示在 PCI 总线上找到的设备；
modules，标识了当前加载到内核中的模块。

/proc 文件系统并不是 GNU/Linux 系统中的惟一一个虚拟文件系统。在这种系统上，sysfs 是一个与 /proc 类似的文件系统，但是它的组织更好（从 /proc 中学习了很多教训）还有一个 debugfs 文件系统，不过（顾名思义）它提供的更多是调试接口。

我们还可以使用 sysctl 来配置这些内核条目

## /proc中的内容
| 文件名      | 文件描述                                                                                                                                           |
| ----------- | -------------------------------------------------------------------------------------------------------------------------------------------------- |
| cmdline     | 系统启动时输入给内核命令行参数                                                                                                                     |
| cpuinfo     | CPU的硬件信息 (型号, 家族, 缓存大小等)                                                                                                             |
| devices     | 主设备号及设备组的列表，当前加载的各种设备（块设备/字符设备）                                                                                      |
| dma         | 使用的DMA通道                                                                                                                                      |
| filesystems | 当前内核支持的文件系统，当没有给 mount(1) 指明哪个文件系统的时候， mount(1) 就依靠该文件遍历不同的文件系统                                         |
| interrupts  | 中断的使用及触发次数，调试中断时很有用                                                                                                             |
| ioports I/O | 当前在用的已注册 I/O 端口范围                                                                                                                      |
| kcore       | 该伪文件以 core 文件格式给出了系统的物理内存映象(比较有用)，可以用 GDB 查探当前内核的任意数据结构。该文件的总长度是物理内存 (RAM) 的大小再加上 4KB |
| kmsg        | 可以用该文件取代系统调用 syslog(2) 来记录内核日志信息，对应dmesg命令                                                                               |
| kallsym     | 内核符号表，该文件保存了内核输出的符号定义, modules(X)使用该文件动态地连接和捆绑可装载的模块                                                       |
| loadavg     | 负载均衡，平均负载数给出了在过去的 1、 5,、15 分钟里在运行队列里的任务数、总作业数以及正在运行的作业总数。                                         |
| locks       | 内核锁 。                                                                                                                                          |
| meminfo     | 物理内存、交换空间等的信息，系统内存占用情况，对应df命令。                                                                                         |
| misc        | 杂项 。                                                                                                                                            |
| modules     | 已经加载的模块列表，对应lsmod命令 。                                                                                                               |
| mounts      | 已加载的文件系统的列表，对应mount命令，无参数。                                                                                                    |
| partitions  | 系统识别的分区表 。                                                                                                                                |
| slabinfo    | sla池信息。                                                                                                                                        |
| stat        | 全面统计状态表，CPU、内存的利用率等都是从这里提取数据。对应ps命令。                                                                                |
| swaps       | 对换空间的利用情况。                                                                                                                               |
| version     | 指明了当前正在运行的内核版本。                                                                                                                     |
| /proc/self  | 链接到当前正在运行的进程                                                                                                                           |

## /proc目录中进程N的信息
| 文件名     | 文件描述                             |
| ---------- | ------------------------------------ |
| cmdline    | 进程启动命令                         |
| cwd        | 链接到进程当前工作目录               |
| environ    | 进程环境变量列表                     |
| exe        | 链接到进程的执行命令文件             |
| fd         | 包含进程相关的所有的文件描述符       |
| maps       | 与进程相关的内存映射信息             |
| mem        | 指代进程持有的内存，不可读           |
| root       | 链接到进程的根目录                   |
| **stat**   | 进程的状态                           |
| **/statm** | 进程使用的内存的状态                 |
| **status** | 进程状态信息，比stat/statm更具可读性 |
| task       | 子线程信息                           |
### /proc/N/status
```C
#cat status                     //使用cat命令列出status文件内容
Name:   calculate               //进程名
State: R (running)              //进程运行状态
Tgid:   2108                    //进程组ID
Pid:    2108                    //进程ID
PPid:   2083                    //父进程ID
TracerPid:      0                //跟踪调试进程ID
Uid:    0       0       0       0   //进程所对应程序的UID
Gid:    0       0       0       0   //进程所对应程序的GID
FDSize: 256                     //进程使用文件句柄大小
Groups: 0 1 2 3 4 10 10         //组信息
//进程所使用的虚拟内存以及实际内存、信号机制方面的信息
VmPeak:    36528 kB           //内存峰值
VmSize:     1344 kB           // 虚拟内存
VmLck:         0 kB           // 加锁内存
VmRSS:       224 kB           // 物理内存|驻存（代码、堆、栈） 
VmData:       12 kB           // 堆内存
VmStk:        16 kB           // 栈内存
VmExe:         4 kB           // 代码段（可执行程序+静态链接库）
VmLib:      1292 kB           // 动态链接库
SigPnd: 0000000000000000
SigBlk: 0000000000000000
SigIgn: 8000000000000000
SigCgt: 0000000000000000
CapInh: 0000000000000000
CapPrm: 00000000fffffeff
CapEff: 00000000fffffeff
```
 | 名称                       | 描述                                                                                                                                                                                             |
 | -------------------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------ |
 | PPid                       | 当前进程的父进程                                                                                                                                                                                 |
 | Tgid                       | Tgid是线程组的ID,一个线程一定属于一个线程组(进程组).                                                                                                                                             |
 | Uid                        | 进程执行者是谁                                                                                                                                                                                   |
 | Gid                        |
 | TracerPid                  | 跟踪当前进程的进程ID,                                                                                                                                                                            |
 | FDSize                     | 当前分配的文件描述符，这个值不是当前进程使用文件描述符的上限. 而程序每打开一个文件，文文件爱女描述符就会地+1，当文件描述符大于FDSize时，FDSizeSize将以32进行递增（32位系统，一次寻址的长度是32） |
 | -                          | -                                                                                                                                                                                                |
 | VmPeak                     | 进程运行过程中占用内存的峰值,程序申请的内存和加载的动态链接库等                                                                                                                                  |
 | VmSize                     | 代表进程现在正在占用的内存，这个值与pmap pid的值基本一致,如果略有不同,可能是内存裂缝所造成的                                                                                                     |
 | VmLck                      | VmLck代表进程已经锁住的物理内存的大小.锁住的物理内存不能交换到硬盘.                                                                                                                              |
 | VmHWM                      | 程序分配到的物理内存的峰值                                                                                                                                                                       |
 | VmRSS                      | 程序现在使用的物理内存                                                                                                                                                                           |
 | VmData                     | 进程数据段的大小                                                                                                                                                                                 |
 | VmStk                      | 进程堆栈段的大小                                                                                                                                                                                 |
 | VmExe                      | 进程代码段的大小                                                                                                                                                                                 |
 | VmLib                      | 进程所使用LIB库的大                                                                           小                                                                                                 |
 | VmPTE                      | 进程占用的页表的大小                                                                                                                                                                             |
 | VmSwap                     | 进程占交换到硬盘的数据量大喜小                                                                                                                                                                   |
 | -                          | -                                                                                                                                                                                                |
 | Threads                    | 进程组拥有的线程数                                                                                                                                                                               |
 | SigQ                       | 程序当前待处理信号的个数                                                                                                                                                                         |
 | -                          | -                                                                                                                                                                                                |
 | CapEff                     | :当一个进程要进行某个特权操作时,操作系统会检查cap_effective的对应位是否有效,而不再是检查进程的有效UID是否为0.                                                                                    |
 | CapPrm                     | 表示进程能够使用的能力,在cap_permitted中可以包含cap_effective中没有的能力，这些能力是被进程自己临时放弃的,也可以说cap_effective是cap_permitted的一个子集.                                        |
 | CapInh                     | 能够被当前进程执行的程序继承的能力.                                                                                                                                                              |
 | CapBnd                     | 系统的边界能力,我们无法改变它.                                                                                                                                                                   |
 | -                          | -                                                                                                                                                                                                |
 | Cpus_allowed               | 该进程可以使用CPU的掩码，例如：3(0011)，使用0，1号CPU                                                                                                                                            |
 | Cpus_allowed_list          | 该进程可以使用CPU的列表                                                                                                                                                                          |
 | Mems_allowed               |
 | Mems_allowed_list          | 内存同CPU一样                                                                                                                                                                                    |
 | voluntary_ctxt_switches    | 进程主动切换的次数，例如：sleep等                                                                                                                                                                |
 | nonvoluntary_ctxt_switches | 进程被动切换的次数，时间片用完                                                                                                                                                                   |
 #### Uid & Gid
Uid:    0       0       0       0
Gid:    0       0       0       0
解释:
第一列数字(RUID):实际用户ID,指的是进程执行者是谁.
第二列数字(EUID):有效用户ID,指进程执行时对文件的访问权限（sudo获取权限）.
第三列数字(SUID):保存设置用户ID,作为effective user ID的副本,在执行exec调用时后能重新恢复原来的effectiv user ID.
第四列数字(FSUID):目前进程的文件系统的用户识别码.一般情况下,文件系统的用户识别码(fsuid)与有效的用户识别码(euid)是相同的.

#### 代码段,堆栈段,数据段
代码段可以为机器中运行同一程序的数个进程共享
堆栈段存放的是子程序（函数）的返回地址、子程序的参数及程序的局部变量
数据段则存放程序的全局变量、常数以及动态数据分配的数据空间（比如用malloc函数申请的内存）
与代码段不同，如果系统中同时运行多个相同的程序，它们不能使用同一堆栈段和数据段.
## 获取CPU占用率
### /proc/stat
  该文件包含了所有CPU活动的信息，该文件中的所有值都是从系统启动开始累计到当前时刻。不同内核版本中该文件的格式可能不大一致
```text
cpuID | user | nice |system |idle| irq | softirq | guest
cpu  38082 627 27594 89390812256 581 895 0 0

cpu022880 472 16855 430287 10617 576 661 0 0

cpu115202 154 10739 463620 1639 4 234 0 0
```
以第一行为例：
| 参数    |        | 描述                                |
| ------- | ------ | ----------------------------------- |
| user    | 38082  | 处于用户态的运行时间                |
| nice    | 627    | nice值为负的进程所占用的CPU时间     |
| system  | 27594  | 处于核心态的运行时间                |
| idle    | 893908 | 除IO等待时间以外的其它等待时间iowai |
| irq     | 581    | 硬中断时间                          |
| softirq | 895    | 软中断时间                          |
| guest   | 0      | --                                  |
总的cpu时间:
totalCpuTime = user + nice+ system + idle + iowait + irq + softirq + stealstolen +  guest

### /proc/N/stat
  该文件包含了某一进程所有的活动的信息，该文件中的所有值都是从系统启动开始累计到当前时刻
```text
# 格式见文末官方文档链接
6873 (a.out) R 6723 6873 6723 34819 6873 8388608 77 0 0 0 41958 31 0 0 25 0 3 05882654 1409024 56 4294967295 134512640 134513720 3215579040 0 2097798 0 0 0 00 0 0 17 0 0 0
```
进程的总Cpu时间
processCpuTime = utime +stime + cutime + cstime
该值包括其所有线程的cpu时间。

### /proc/\<pid\>/task/
1. /proc/\<pid\>/task/\<tid\>/stat
线程Cpu时间threadCpuTime = utime +stime 
2. /proc/<pid>/comm & /proc/<pid>/task/<tid>/comm
子线程可访问的公共数据
3. /proc/<pid>/task/<tid>/children
子线程的id，tid是linux中的轻量级进程(lwp)
Since this interface is intended to be fast and cheap it doesn’t guarantee to provide precise results and some children might be skipped, especially if they’ve exited right after we printed their pids

线程Cpu时间:
threadCpuTime = utime + stime
## 获取内存占用率
maps, statm, mem: 进程的内存信息。
### /proc/N/statm
```text
Size | Resident(vmRSS) | Shared | Trs | Lrs| Drs | dt
487 185 133 31 0 67 0
```
很简单地返回7组数字，每一个的单位都是一页 （常见的是4KB）
size:任务虚拟地址空间大小,VmSize/4
Resident：正在使用的物理内存大小
Shared：共享页数
Trs：程序所拥有的可执行虚拟内存大小
Lrs：被映像倒任务的虚拟内存空间的库的大小
Drs：程序数据段和用户态的栈的大小
dt：脏页数量

**/proc/N/statm 里的数值实际是按page来统计的，Resident(pages)为应用程序正在使用的物理内存的大小 VmRSS/4，为实际使用内存数值的四分之一**
### /proc/N/maps
```text
内存段的虚拟地址 | 执行权限 | 进程地址偏移量 | 映射文件的主、次设备号 | 印象文件的节点号 | 影像文件的路径 | 
[root@localhost proc]# cat /proc/1/maps
00110000-00111000 r-xp 00110000 00:00 0          [vdso]
0032b000-00347000 r-xp 00000000 fd:00 852733     /lib/ld-2.8.so
00347000-00348000 r--p 0001c000 fd:00 852733     /lib/ld-2.8.so
00348000-00349000 rw-p 0001d000 fd:00 852733     /lib/ld-2.8.so
0034b000-004ae000 r-xp 00000000 fd:00 852734     /lib/libc-2.8.so
004ae000-004b0000 r--p 00163000 fd:00 852734     /lib/libc-2.8.so
004b0000-004b1000 rw-p 00165000 fd:00 852734     /lib/libc-2.8.so
004b1000-004b4000 rw-p 004b1000 00:00 0
08048000-08067000 r-xp 00000000 fd:00 843075     /sbin/init
08067000-08068000 rw-p 0001e000 fd:00 843075     /sbin/init
08b42000-08b6a000 rw-p 08b42000 00:00 0          [heap]
b8046000-b8048000 rw-p b8046000 00:00 0
bfb4e000-bfb63000 rw-p bffeb000 00:00 0          [stack]
```
### /proc/N/memmap

### /proc/N/maps


## 官方介绍
[The /proc Filesystem](https://www.kernel.org/doc/html/latest/filesystems/proc.html)
## 参考
[Linux ps命令详解](http://c.biancheng.net/view/1062.html)
[proc文件系统](https://blog.csdn.net/sty23122555/article/details/51638697)
[Linux下进程信息/proc/pid/status的深入分析](https://blog.csdn.net/beckdon/article/details/48491909)
[Linux平台Cpu使用率的计算](http://www.blogjava.net/fjzag/articles/317773.html)