# 检查程序的 I/O 等待时间
## 1. 检查是不是I/O引起系统缓慢
使用 top 查看 cpu 等待时间
 
top 中的 wa 是指当CPU空闲且磁盘IO阻塞的时间占比（不包含网络IO），需要注意的是，当CPU空闲时，如果存在磁盘I/O，wa 指标也会增加，所以 wa 指标并不能 100% 确定是有 I/O 阻塞引起的。


## 查找哪个硬盘正在被写入
使用 iostat 命令

## 查找引起高I/O的进程 
iotop 统计所有进程的磁盘活动，`iotop --only` 只显示正在进行 I/O 的进程

还有一种方法是检查 /proc/<pid>/io 文件查看程序的 I/O 状态

## 进程状态列表
通过 ps 查看进程状态列表查看进程是否在等待I/O，`man ps` 可查所有可能的进程状态.

D - 非中断 sleep，通常是由于 I/O
S - 中断 sleep

## 查找哪个文件在被繁重地写入
1. 通过lsof -p <pid> 查看进程打开的文件
2. 同过 df <file> 进一步验证文件是否位于高I/O 的磁盘上

[发现并解决linux高I/O Wait问题 – 如何发现linux中引起高io等待的进程](https://blog.csdn.net/donglynn/article/details/80041144)

(https://www.cnblogs.com/dongzhiquan/p/top_wa_io.html)