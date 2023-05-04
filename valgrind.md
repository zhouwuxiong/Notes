valgrind --tool=callgrind 

## 使用流程
```
1. 编译
gcc -g test.cpp -o test
2. 执行程序
valgrind --tool=callgrind  --separate-threads=yes --log-file=valgrind/callgrind.out ./bin_Ubuntu_18.04_x86_64_Debug/VSLAM_offline --flagfile=config/vslam_ROEWE.flags > log.txt 2>&1
2. 生成可读性更高的统计报告
e```
## notice
- valgrind 统计的是程序执行的指令数，无法统计 I/O 阻塞产生的消耗

参数|描述|默认|备注|
-|-|-|-
--simulate-cache|是否模拟缓存|no| 
--separate-threads| 是否单独统计每个线程|no|
--toggle-collect=function| 只统计指定的函数| |
---dump-before=function | 进入模个函数前先存储以下统计信息|-|这个选项可以被指定多次或使用正则(如：foo*)，来关注多个函数 |
---dump-after=function | | |
--zero-before=function | 离开函数后清0计数| |
--inclusive | 统计的时间消耗是否包含内部子函数的时间消耗，否则函数的内部调用的函数占用的时间不计为当前函数的时间消耗 | no|
--tree  | 是否打印函数的调用关系| no|

## 产生 dump 文件
在程序中插入  CALLGRIND_DUMP_STATS 来在指定的代码位置产生dump存储

## 
valgrind 默认在所有的事件产生时（例如：指令执行，缓存命中等）都会增加计算，但是如果只关心某个特定的函数，你可以关闭对不感兴趣的程序部分的计数，或者生成多个dunp到不同的文件。这样做的好处是：1. 你可以得到非常细腻度的统计结果（例如：函数中的某个循环）。2. valfrind 执行速度更快

valgrind 在某个时间点是否进行计数，有两种方式来决定：1. collection state， collection state 默认为开启状态，可以通过切换 collection state 来进行细腻度的统计，但是禁用 collection state 不会使得 valgrind 的执行速度变快。2. instrumentation mode ,但是 instrumentation mode 应该被小心和粗糙的使用，因为 instrumentation mode 的每次切换都会重置模拟器的状态

### Counting global bus events
为了访问多线程代码中线程之间的共享数据，需要同步以避免竞争条件。同步原语通常通过原子指令实现。然而，过度使用这样的指令可能导致性能问题。 valgrind 可以统计原子指令的数量，同过`--collect-bus=yes`选项

### Avoiding 

# Using Case
## callgrind 收集特定函数的CPU占用

The toggle-collect option is very picky in how you specify the method to use as trigger. You actually need to specify its argument list as well, and even the whitespace needs to match! Use the method name exactly as it appears in the callgrind output. For instance, I am using this invokation:
```
valgrind 
    --tool=callgrind 
    --collect-atstart=no 
    "--toggle-collect=testClass::ctrl_simulate(float, int)"
    ./swaag
```
Please observe:

    The double quotes around the option.
    The argument list including parentheses.
    The whitespace after the comma character.

[Callgrind: Profile a specific part of my code](https://stackoverflow.com/questions/13688185/callgrind-profile-a-specific-part-of-my-code)

## callgrind 收集特定代码片段的CPU占用
1. CALLGRIND_START_INSTRUMENTATION / CALLGRIND_STOP_INSTRUMENTATION
```
#include <valgrind/callgrind.h>

//request callgrind to start full profile
CALLGRIND_START_INSTRUMENTATION;

//codes...

//request callgrind to stop full profile
CALLGRIND_STOP_INSTRUMENTATION;
```
使用命令CALLGRIND_DUMP_STATS;可以让Callgrind立即生成一个dump. 每次遇到这个命令都会生成一个dump, 即使在比如for循环里面, 那么就会生成循环次数相等的dump. 注意,对于CALLGRIND_START_INSTRUMENTATION和CALLGRIND_STOP_INSTRUMENTATION这对组合控制的是让Callgrind只统计命令区间内的代码, 即使这对组合放在比如for循环中, 如果没有CALLGRIND_DUMP_STATS;, 那么也只会生成一个dump.
使用CALLGRIND_ZERO_STATS;可以清除Callgrind当前的数据状态.

2. CALLGRIND_TOGGLE_COLLECT
start/stop 会每次都会dump区间，这在循环中使用时不够灵活
```
#include <valgrind/callgrind.h>
for (int i=0; i<maxSample; ++i) {
    //Prepare data to be processed...
    CALLGRIND_TOGGLE_COLLECT;
    //Method to be profiled with these data
    CALLGRIND_TOGGLE_COLLECT;
    //Post operation on the data
}
CALLGRIND_DUMP_STATS;

```

```
valgrind --tool=callgrind --collect-atstart=no  ./a.out> log.txt 2>&1t 2>&1
```

[Valgrind.Callgrind使用](https://www.cnblogs.com/willhua/p/9818530.html)


# gprof
## cmake && gprof
```
# cmake use gprof
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg")
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pg")
SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pg")
SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -pg")
```