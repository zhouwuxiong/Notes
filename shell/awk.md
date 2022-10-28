## 简介
AWK 是一种处理文本文件的语言，是一个强大的文本分析工具，主要是用来格式化。

linux 三剑客功能:
grep ： 过滤文本
sed  :  修改文本
awk  :  处理文本

### 参数
-F 指定分隔符
-f 从文件读取

### 预定义变量
```text
$0  当前行

$n  第n列

NR   记录行号

NF

FS 指定文本内容分隔符(默认是空格)
```

### 执行流程控制
1. BEGIN{}	: 最开始执行
2. //		: 正则
3. {}		: 循环体
4. END{}	: 最后执行
这里面最少有一个，最多有四个！

### akw中的函数
```text
	print	: 打印
	printf	: 格式化打印
	%s		: 字符串
	%d		: 数字
	-		: 左对齐
	+		: 右对齐
	15		: 至少占用15字符
```

## 实例
### 每隔5行，打印一行横线
```Shell
awk -F: '{if(NR%5==0){print "----------"}print $0}' /etc/passwd
```
### 删除screen中的包含关键字的窗口
```shell
screen -ls|awk '{print $1}'|grep "Bicocca"|awk '{print $1}
```
## 参考
[awk命令详解](https://www.cnblogs.com/zhengyan6/p/16290156.html)