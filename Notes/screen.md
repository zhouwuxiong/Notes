### 创建窗口
screen -S "name"

### 退出
ctrl + A + D

### 查看
screen -ls

### 恢复
```Shell
screen -r "name"
```
```text
-A 　将所有的视窗都调整为目前终端机的大小。
-d <作业名称> 　将指定的screen作业离线。
-h <行数> 　指定视窗的缓冲区行数。
-m 　即使目前已在作业中的screen作业，仍强制建立新的screen作业。
-r <作业名称> 　恢复离线的screen作业。
-R 　先试图恢复离线的作业。若找不到离线的作业，即建立新的screen作业。
-s 　指定建立新视窗时，所要执行的shell。
-S <作业名称> 　指定screen作业的名称。
-v 　显示版本信息。
-x 　恢复之前离线的screen作业。
-ls或--list 　显示目前所有的screen作业。
-wipe 　检查目前所有的screen作业，并删除已经无法使用的screen作业。
```

## 批量删除screen窗口
```Shell
# 1. 查看窗口个数
screen -ls |awk '/Socket/'|awk '{print $1}'

# 2. 如果有20个screen，关闭所有screen
screen -ls|awk 'NR>=2&&NR<=20{print $1}'|awk '{print "screen -S "$1" -X quit"}'|sh

# 3. 关闭包含关键字的screen
screen -ls|awk '{print $1}'|grep "Bicocca"|awk '{print "screen -S "$1" -X quit"}'|sh

# 4. 将(3)包装成shell命令
echo screen -ls|awk '{print $1}'|grep $1|awk '{print "screen -S "$1" -X quit"}'|sh >> ~/.bashrc
```