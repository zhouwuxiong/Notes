# GNU parallel
GNU parallel 是一个可以将多个命令并行运行的命令。

[GNU Operating System](https://www.gnu.org/software/parallel/)

[GNU Parallel Tutorial](https://www.gnu.org/software/parallel/parallel_tutorial.html#chunk-size)
## brief
### 常见参数
--eta：显示任务完成的预计剩余时间。
-j 2 或 -jobs 2：同时运行的命令数，在本例中设置为2。
--load 80%：最大cpu负载。在上面的命令中，我们指定最多可以运行80％的CPU。
--noswap：如果服务器处于大量内存负载下，则不会启动新作业，以便在存储新信息之前从内存中删除信息。
后面的命令使用重定向和管道符时：">>" ">" "|" 需要加上双引号;

## Demo
### 字符串替换
```shell
# 默认字符串“{}”代表输入： # /tmp
parallel echo {} ::: /tmp
# 替换字符串'{/}'删除所有内容，包括最后一个正斜杠：#stdio.h
parallel echo {/} ::: /tmp/stdio.h
# 如果只想返回路径，请使用'{//}'字符串：#/tmp
parallel echo {//} ::: /tmp/stdio.h
# 字符串'{.}'删除所有文件扩展名：#/tmp/stdio
parallel echo {.} ::: /tmp/stdio.h

# -k 强制执行顺序，请使用“ -k”选项，如下所示：
parallel -k sleep {}\; echo {} ::: 5 2 1 4 3
```

### 使用不同的参数执行命令
```shell
parallel echo ::: A B C
parallel echo ::: A B C ::: D E F
parallel --link echo ::: A B C ::: D E F
# If no command is given after parallel the arguments themselves are treated as commands:
parallel ::: ls 'echo foo' pwd
```
2. 执行 shell 脚本中的函数
```shell
# The command can be a script, a binary or a Bash function if the function is exported using export -f:
# Only works in Bash
my_func() {
  echo in my_func $1
}
export -f my_func
parallel my_func ::: 1 2 3
```

```text
parallel echo ::: A B C ::: D E F
Output (the order may be different):
A D
A E
A F
B D
B E
B F
C D
C E
C F

parallel --link echo ::: A B C ::: D E F
A D
B E
C F
```
### 
```shell
# 1. 并行运行 shell script 中的多个命令
parallel --jobs 6 < jobs2run
# 2. 将目录中的文件进行两两合并 
ls -1 | parallel --max-args=2 cat {1} {2} ">" {1}_{2}.person
```
### parallel --pipe

## Demo
```shell
find /path/to/files -type f -name "*.txt" | parallel sed -i 's/old_text/new_text/g' {}
```