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
valgrind --log-file=./valgrind_report.log --leak-check=full --show-leak-kinds=all --show-reachable=no --track-origins=yes ./
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