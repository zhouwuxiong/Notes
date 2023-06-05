# 生成 core dump 文件
```shell
# 1. 设置 core 文件大小
ulimit -c
# 2. 设置 core 文件路径，
echo "kernel.core_pattern = /tmp/core.%e.%p.%t">/etc/sysctl.conf
sysctl -p
# 3. 使用 g++ -g 选项编译

# 4. 使用 sudo 权限执行程序（要写根目录路径）
```

[在 Linux 生成 core dump 文件 ](senlinzhan.github.io/2017/12/31/coredump/)
[Linux core dump文件生成与使用](https://www.cnblogs.com/lsdb/p/12192768.html)