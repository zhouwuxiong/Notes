查看系统配置：
cat /proc/cpuinfo


## sftp
### 基本操作
1. 连接
指定密钥和端口
sftp -P 99 -oIdentityFile=deming_rsa deming@192.168.111.137

2. 下载


# 文件自动下载
```shell
#!/bin/bash
data=$(date +%Y%m%d -d '-1 day')
cd /home/ICBC_HD
sftp -oIdentityFile=/home/docker/ftp_rsa -oPort=22 ftpuser@192.168.137.130 <<EOF // <<EOF 不可换行
cd data
mget *$data* #当天的所有文件
exit
EOF
```