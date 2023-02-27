## 动机
最近在linux上运行程序时，发现每次运行程序之后，linux中的内存占用就会增多，直到内存用完系统卡死。

## 怀疑-1 : 程序有内存泄漏

## 怀疑-2 ： linux 缓存了太多文件
由于linux的缓存机制会将磁盘中的缓存到物理内存，如果运行的程序有大量的文件I/O操作，则会导致缓存占用太高。[Linux内存越用越少，不释放内存](https://developer.aliyun.com/ask/101746)

1. 使用 `free -h` 查看系统内存统计
```text
zwx@domi-ThinkPad-X1-Carbon:~$ free -h
              total        used        free      shared  buff/cache   available
Mem:           15Gi       9.3Gi       635Mi       1.0Gi       5.1Gi       4.4Gi
Swap:         2.0Gi        41Mi       2.0Gi
```
```text
used          程序使用的内存
free          空闲的物理内存
available     程序可以使用的物理内存，因为 buff/cache 中的数据是可以写入磁盘的，所以 available = free + buff/cache
Swap          当物理内存不够时，将内存中的数据缓存到磁盘，从而达到扩展内存的目的。
```
[linux下free命令详解](https://www.cnblogs.com/ultranms/p/9254160.html)

[Linux用户态进程如何监控内存被写事件](https://blog.csdn.net/dog250/article/details/90690292)