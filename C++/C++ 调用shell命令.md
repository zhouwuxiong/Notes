## system
system()会调用fork()产生子进程，由子进程来调用/bin/sh-c string来执行参数string字符串所代表的命令，此命令执行完后随即返回原调用的进程

**返回值**
如果system()调用成功则最后会返回执行shell命令后的返回值

如果system() 在调用/bin/sh时失败则返回127，其他失败原因返回-1

```C++
system(“ls -al /etc/passwd /etc/shadow”);
```
**注意**
在编写具有SUID/SGID权限的程序时请勿使用system( )，system( )会继承环境变量，通过环境变量可能会造成系统安全的问题。

## popen

popen()会调用fork()产生子进程，然后从子进程中调用/bin/sh -c来执行参数command的指令

popen()会建立管道连到子进程的标准输出设备或标准输入设备，然后返回一个文件指针。随后进程便可利用此文件指针来读取子进程的输出设备或是写入到子进程的标准输入设备中。

## exec函数簇
exec函数会取代执行它的进程,  也就是说, 一旦exec函数执行成功, 它就不会返回了, 进程结束.   但是如果exec函数执行失败, 它会返回失败的信息,  而且进程继续执行后面的代码

通常exec会放在fork() 函数的子进程部分, 来替代子进程执行啦, 执行成功后子程序就会消失,  但是执行失败的话, 必须用exit()函数来让子进程退出!

exec是一组函数的统称,他们的功能都是差不多的,因为要用于接受不同的参数所以要用不同的名字区分它们。
```C++
#include <unistd.h>  
  
int execl(const char *path, const char *arg, ...);  
  
int execlp(const char *file, const char *arg, ...);  
  
int execle(const char *path, const char *arg, ..., char *const envp[]);  
  
int execv(const char *path, char *const argv[]);  
  
int execvp(const char *file, char *const argv[]);  
  
int execve(const char *path, char *const argv[], char *const envp[]);
```


[linux下c语言调用shell命令的方法： system、popen、exec系列函数](https://blog.csdn.net/buleskycode/article/details/89249183)