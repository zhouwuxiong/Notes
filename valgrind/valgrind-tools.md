## valgrind overview
- valgraind 与CPU的实现细节相关
- valgraind 运行速度特别慢，基本比原程序慢10倍左右。[C++ 性能分析工具调研](https://blog.csdn.net/irving512/article/details/117077847)

## valgraind tools
cachegrind 是一个缓冲模拟器。它可以用来标出你的程序每一行执行的指令数和导致的缓冲不命中数。

callgrind 在 cachegrind 基础上添加调用追踪。它可以用来得到调用的次数以及每次函数调用的开销。作为对cachegrind的补充，callgrind可以分别标注各个线程，以及程序反汇编输出的每条指令的执行次数以及缓存未命中数。

helgrind 能够发现程序中潜在的条件竞争。

lackey 是一个示例程序，以其为模版可以创建你自己的工具。在程序结束后，它打印出一些基本的关于程序执行统计数据。

massif 是一个堆剖析器，它测量你的程序使用了多少堆内存和栈内存。

memcheck 是一个细粒度的的内存检查器。

none 没有任何功能。它一般用于Valgrind的调试和基准测试。

##

#### 1.Memcheck
最常用的工具，用来检测程序中出现的内存问题，所有对内存的读写都会被检测到，一切对malloc()/free()/new/delete的调用都会被捕获。所以，它能检测以下问题：
- 对未初始化内存的使用；
- 读/写释放后的内存块；
- 读/写超出malloc分配的内存块；
- 读/写不适当的栈中内存块；
- 内存泄漏，指向一块内存的指针永远丢失；
- 不正确的malloc/free或new/delete匹配；
- memcpy()相关函数中的dst和src指针重叠。

valgrind对于protobuf,stl这样的3方库的兼容性不算太好，所以会造成输出一堆的still reachable字样，可以将这些检查option关闭掉。
```shell
valgrind --log-file=./valgrind_report.log --leak-check=full --show-leak-kinds=all --show-reachable=no --track-origins=yes ./a.out
```
```text
–log-file
指定报告输出文件
–track-origins=yes
是否显示未定义的变量，在堆、栈中被定义没有被initialised的变量都被定义成origins。默认是关闭这个option的。
–show-leak-kinds=all
这里可以支持的选项有[definite|possible]，一般只需要去关注definite（绝逼），possible是可能会存在。
–leak-check=full
当服务器退出时是否收集输出内存泄漏，选项有[no|summary|full]这个地方我们将其设置成全输出，默认将会使用summary方式。
```
参考:
[使用valgrind检查内存问题并且输出报告](https://blog.csdn.net/erlang_hell/article/details/51360149)

其它参考：
[Valgrind安装及使用](https://blog.csdn.net/tannanxi/article/details/119671984)
[Valgrind详细教程（1） Memcheck](https://blog.csdn.net/tissar/article/details/87194737)
[valgrind--memcheck工具命令选项](https://blog.csdn.net/strategycn/article/details/7865525)
[DEBUG神器valgrind之memcheck报告分析](https://blog.csdn.net/jinzeyu_cn/article/details/45969877)

##### memcheck 输出报告分析
valgrind输出结果会报告5种内存泄露，"definitely lost", "indirectly lost", "possibly lost", "still reachable", and "suppressed"。这五种内存泄露分析如下：

```text
"definitely lost"：确认丢失。程序中存在内存泄露，应尽快修复。当程序结束时如果一块动态分配的内存没有被释放且通过程序内的指针变量均无法访问这块内存则会报这个错误。 

"indirectly lost"：间接丢失。当使用了含有指针成员的类或结构时可能会报这个错误。这类错误无需直接修复，他们总是与"definitely lost"一起出现，只要修复"definitely lost"即可。例子可参考我的例程。

"possibly lost"：可能丢失。大多数情况下应视为与"definitely lost"一样需要尽快修复，除非你的程序让一个指针指向一块动态分配的内存（但不是这块内存起始地址），然后通过运算得到这块内存起始地址，再释放它。例子可参考我的例程。当程序结束时如果一块动态分配的内存没有被释放且通过程序内的指针变量均无法访问这块内存的起始地址，但可以访问其中的某一部分数据，则会报这个错误。

"still reachable"：可以访问，未丢失但也未释放。如果程序是正常结束的，那么它可能不会造成程序崩溃，但长时间运行有可能耗尽系统资源，因此笔者建议修复它。如果程序是崩溃（如访问非法的地址而崩溃）而非正常结束的，则应当暂时忽略它，先修复导致程序崩溃的错误，然后重新检测。

"suppressed"：已被解决。出现了内存泄露但系统自动处理了。可以无视这类错误。这类错误我没能用例程触发，看官方的解释也不太清楚是操作系统处理的还是valgrind，也没有遇到过。所以无视他吧~
```
##### 使用 /proc/meminfo 排查内存泄漏问题
```text
MemTotal:          45964 kB    //所有可用的内存大小，物理内存减去预留位和内核使用。系统从加电开始到引导完成，firmware/BIOS要预留一些内存，内核本身要占用一些内存，最后剩下可供内核支配的内存就是MemTotal。这个值在系统运行期间一般是固定不变的，重启会改变。
MemFree:            1636 kB    //表示系统尚未使用的内存。
MemAvailable:       8496 kB    //真正的系统可用内存，系统中有些内存虽然已被使用但是可以回收的，比如cache/buffer、slab都有一部分可以回收，所以这部分可回收的内存加上MemFree才是系统可用的内存
Buffers:               0 kB    //用来给块设备做缓存的内存，(文件系统的 metadata、pages)
Cached:             7828 kB    //分配给文件缓冲区的内存,例如vi一个文件，就会将未保存的内容写到该缓冲区
SwapCached:            0 kB    //被高速缓冲存储用的交换空间（硬盘的swap）的大小
Active:            19772 kB    //经常使用的高速缓冲存储器页面文件大小
Inactive:           3128 kB    //不经常使用的高速缓冲存储器文件大小
Active(anon):      15124 kB    //活跃的匿名内存
Inactive(anon):       52 kB    //不活跃的匿名内存
Active(file):       4648 kB    //活跃的文件使用内存
Inactive(file):     3076 kB    //不活跃的文件使用内存
Unevictable:           0 kB    //不能被释放的内存页
Mlocked:               0 kB    //系统调用 mlock 家族允许程序在物理内存上锁住它的部分或全部地址空间。这将阻止Linux 将这个内存页调度到交换空间（swap space），即使该程序已有一段时间没有访问这段空间
SwapTotal:             0 kB    //交换空间总内存
SwapFree:              0 kB    //交换空间空闲内存
Dirty:                 4 kB    //等待被写回到磁盘的
Writeback:             0 kB    //正在被写回的
AnonPages:         15100 kB    //未映射页的内存/映射到用户空间的非文件页表大小
Mapped:             7160 kB    //映射文件内存
Shmem:               100 kB    //已经被分配的共享内存
Slab:               9236 kB    //内核数据结构缓存
SReclaimable:       2316 kB    //可收回slab内存
SUnreclaim:         6920 kB    //不可收回slab内存
KernelStack:        2408 kB    //内核消耗的内存
PageTables:         1268 kB    //管理内存分页的索引表的大小
NFS_Unstable:          0 kB    //不稳定页表的大小
Bounce:                0 kB    //在低端内存中分配一个临时buffer作为跳转，把位于高端内存的缓存数据复制到此处消耗的内存
WritebackTmp:          0 kB    //FUSE用于临时写回缓冲区的内存
CommitLimit:       22980 kB    //系统实际可分配内存
Committed_AS:     536244 kB    //系统当前已分配的内存
VmallocTotal:     892928 kB    //预留的虚拟内存总量
VmallocUsed:       29064 kB    //已经被使用的虚拟内存
VmallocChunk:     860156 kB    //可分配的最大的逻辑连续的虚拟内存
```

![使用 /proc/meminfo 排查内存泄漏问题](../images/proc-meminfo-memleak.png)

[深入理解Linux 的Page Cache](https://zhuanlan.zhihu.com/p/436313908)
#### 2. Massif
1. 统计程序的内存占用，包括堆和栈
2. 统计一些特殊的"内存泄漏"，例如：还存在指向内存的指针，但是内存却没有再被使用
3. 提供了程序内存分配的详细信息，例如：内存由哪个部分的程序管理

##### 使用：
```shell
# 1. 编译时添加调试选项 -g
g++ a.cpp -g
# 2.1以时间为单位进行统计
valgrind --tool=massif ./a.out
# 2.2 以 size 为单位进行统计
valgrind --tool=massif --time-unit=B ./a.out
# 2.3 同时统计栈内存
valgrind --tool=massif --stacks=yes --time-unit=B ./a.out
# 打印统计报告
ms_print  massif.out.50333
```
#### 注意
- massif 只会统计 malloc, calloc, realloc, memalign, new, new[] 等函数分配的内存，对于更底层的内存分配函数（例如：mmap、mremap、brk）分配的内存，不会进行溶剂。
- massif 不会统计 code 、data、bss 段的大小
- 如果想统计程序使用的所有内存，包括 code、data、BSS段的内存，可以设置`--pages-as-heap=yes`，这时 massif 会将，更底层的内存分配函数（如：mmap等）调用视为不同的块。
- 设置`--pages-as-heap=yes`后，会自动统计栈信息，因为栈最终也是通过mmp进行内存分配，所以不能再设置`--stacks=yes`

##### 报告分析
1. 表格
```text
total 消耗的总内存
useful-heap 程序申请的内存
extra-heap 程序未使用的内存

--------------------------------------------------------------------------------
  n        time(B)         total(B)   useful-heap(B) extra-heap(B)    stacks(B)
--------------------------------------------------------------------------------
  0              0                0                0             0            0
  1          1,008            1,008            1,000             8            0
  2          2,016            2,016            2,000            16            0
  3          3,024            3,024            3,000            24            0
  4          4,032            4,032            4,000            32            0
  5          5,040            5,040            5,000            40            0
  6          6,048            6,048            6,000            48            0
  7          7,056            7,056            7,000            56            0
  8          8,064            8,064            8,000            64            0
  9          9,072            9,072            9,000            72            0

```

分配给程序的内内存小于程序申请的内存？
- 每个堆块都需要管理字节，管理字节的大小与分配器的实现细节有关，massif默认为8字节，可以通过`--heap-admin`进行修改
- 为了进行内存对齐，分配器往往会分配更多的内存，可以通过`--alignment`参数进行修改

2. allocation tree
内存的分配树
```text
// 程序使用过的堆内存分配函数
99.48% (20,000B) (heap allocation functions) malloc/new/new[], --alloc-fns, etc.
// allocation tree
->49.74% (10,000B) 0x804841A: main (example.c:20)
| 
->39.79% (8,000B) 0x80483C2: g (example.c:5)
| ->19.90% (4,000B) 0x80483E2: f (example.c:11)
| | ->19.90% (4,000B) 0x8048431: main (example.c:23)
| |   
| ->19.90% (4,000B) 0x8048436: main (example.c:25)
|   
->09.95% (2,000B) 0x80483DA: f (example.c:10)
  ->09.95% (2,000B) 0x8048431: main (example.c:23)
```
- massif 根据程序的调用位置进行内存分配统计，例如：同一个函数在多个位置调用产生的内存分配会被分别统计，循环中的单个语句分配的内存会被累计统计
- 正常情况下，alocate tree 中的 entry size 等于 child size 的和，但是`a stack trace can be a sub-trace of another stack trace`时，entry's size 可能会大于 child's size 的和。massif 有可能会检测这种情况的发生，这时会发出警告：
```text
Warning: Malformed stack trace detected.  In Massif's output,
         the size of an entry's child entries may not sum up
         to the entry's size as they normally do.
```

#### 统计多进程
- 子进程在 fork 时，会继承父进程的所有统计数据
- 如果`--massif-out-file`指定的 format string 中不包含`%p`，子进程和父进程的统计信息将会输出同一个文件中。

#### massif-visualizer
[massif-visualizer](https://valgrind.org/docs/manual/ms-manual.html#ms-manual.overview)

#### optopms
```text
--heap=<yes|no> [default: yes]

--heap-admin=<size> [default: 8]

--stacks=<yes|no> [default: no]

--pages-as-heap=<yes|no> [default: no]

--depth=<number> [default: 30]
跟踪的最大栈深度（alloc tree中的深度）

--alloc-fn=<name>
将指定的函数（封装了malloc等函数）当作 alloc 函数处理，以避免不必要的输出

--ignore-fn=<name>

--threshold=<m.n> [default: 1.0]

--peak-inaccuracy=<m.n> [default: 1.0]
对小于1%的内存分配分支进行汇总

--time-unit=<i|ms|B> [default: i]

--detailed-freq=<n> [default: 10]
打印详细快照的频率，默认每个 snapshot 都打印详细信息

--max-snapshots=<n> [default: 100]

--massif-out-file=<file> [default: massif.out.%p]
```

[valgrind Massif manual](https://valgrind.org/docs/manual/ms-manual.html)


## callgrind
- 函数之间的调用关系和执行的指令数。
### **Notice**
- 函数的 cost 会包含其内部调用的所有函数的 cost

```shell
# 1. 下载 https://github.com/jrfonseca/gprof2dot
apt-get install python3 graphviz

gprof2dot -f callgrind -n10 -s callgrind.out.31113 > valgrind.dot

dot -Tpng valgrind.dot -o valgrind.png
```
**效果不好,统计信息太细，包含了很多库函数**

[基于Valgrind的callgrind工具进行代码性能分析](https://bbs.huaweicloud.com/blogs/228047)



## Helgrind
Helgrind 主要用于检测线程同步问题。
1. 错误的使用 POSIX thread 的 API
 [ Detected errors: Misuses of the POSIX pthreads API](https://valgrind.org/docs/manual/hg-manual.html#hg-manual.api-checks)
2. 死锁问题
  [Detected errors: Inconsistent Lock Orderings](https://valgrind.org/docs/manual/hg-manual.html#hg-manual.lock-orders)
3. 访问多线程共享的变量没有加锁
   [Detected errors: Data Races](https://valgrind.org/docs/manual/hg-manual.html#hg-manual.data-races)

## Cachegrind
检测执行程序时 CPU 的缓存命中率