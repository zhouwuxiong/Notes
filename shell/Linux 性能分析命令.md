# 性能分析命令
## 1. 查看进程发生缺页中断的次数
当一个进程发生缺页中断的时候，进程会陷入内核态，执行以下操作：
1、检查要访问的虚拟地址是否合法
2、查找/分配一个物理页
3、填充物理页内容（读取磁盘，或者直接置0，或者啥也不干）
4、建立映射关系（虚拟地址到物理地址）

### 2. pmap 查看进程的内存分配
通过读取/proc/$PID/maps 和 smaps 的数据，解析数据，生成进程的虚列内存映像和一些内存统计：

```shell

 pmap -X -p 3193131931 
```

```shell
# majflt代表major fault，中文名叫大错误，minflt代表minor fault，中文名叫小错误。
ps -o majflt,minflt -C top
```

## strace 跟踪程序的系统调用
strace是一个可用于诊断、调试和教学的Linux用户空间跟踪器。我们用它来监控用户空间进程和内核的交互，比如系统调用、信号传递、进程状态变更等。
### strace 可以干什么？
1. 监视程序 调用的系统函数、访问的文件、
### 参数说明
```text
-tt 在每行输出的前面，显示毫秒级别的时间
-T 显示每次系统调用所花费的时间
-v 对于某些相关调用，把完整的环境变量，文件stat结构等打出来。
-f 跟踪目标进程，以及目标进程创建的所有子进程
-e 控制要跟踪的事件和跟踪行为,比如指定要跟踪的系统调用名称
-o 把strace的输出单独写到指定的文件
-s 当系统调用的某个参数是字符串时，最多输出指定长度的内容，默认是32个字节
-p 指定要跟踪的进程pid, 要同时跟踪多个pid, 重复多次-p选项即可。
```
这里特别说下strace的-e trace选项。要跟踪某个具体的系统调用，-e trace=xxx即可。但有时候我们要跟踪一类系统调用，比如所有和文件名有关的调用、所有和内存分配有关的调用。你*可以手工输入系统调用的名称*也可使用系统提供的几个常用统调用组合。
```text
-e trace=file     跟踪和文件访问相关的调用(参数中有文件名)
-e trace=process  和进程管理相关的调用，比如fork/exec/exit_group
-e trace=network  和网络通信相关的调用，比如socket/sendto/connect
-e trace=signal   信号发送和处理相关，比如kill/sigaction
-e trace=desc     和文件描述符相关，比如write/read/select/epoll等
-e trace=ipc      进程见同学相关，比如shmget等
-e read=          输出从指定文件中读出的数据
-e write=         输出写入到指定文件中的数据
-e raw=           将指定的系统调用的参数以十六进制显示. 
```
### 事例
```shell
# 1. 跟终命令
strace ls -lh /var/log/messages
# 2. 跟中运行的程序
strace -p 17553
# 3. 跟踪nginx, 看其启动时都访问了哪些文件
strace -tt -T -f -e trace=file -o /data/log/strace.log -s 1024 ./nginx
# 4. 定位共享内存异常。
# ipc 跟踪和进程通信相关的系统调用
strace -tt -f -e trace=ipc ./a_mon_svr     ../conf/a_mon_svr.conf
```
[strace命令详解]https://www.cnblogs.com/machangwei-8/p/10388883.html