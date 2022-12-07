## tar
该命令可以将许多文件一起保存到一个单独的磁带或磁盘中进行归档

## 参数
| 选项 | 描述           |
| ---- | -------------- |
| -c   | 打包           |
| -x   | 解包           |
| -A   | 追加           |
| -t   | 查看包中的文件 |
| -C   | 指定解压位置   |
| -f   | 指定包名       |
| -P   | 使用绝对路径   |
| -z   | 使用gzip压缩   |
| -v   | 显示过程       |

## Demo
### 压缩
`‵‵shell
# 解压
tar -xvf test.tar.gz -C test
# 压缩
tar -zcvf test.tar.gz test
# 压缩
tar -zcvf test.tar.gz test
```

## Notice
### 1. 注意原文件和目标文件的位置
**tar的第一个路径为目标文件，第二个路径为原文进，如果错误的将原文件指定为目标文件，tar会删除原文件，一定要小心确认**

### 2. 使用绝对路径压缩
tar默认使用相对路径压缩，因此会去掉开头的`/`,如果要使用绝对路径，需要添加爱 -P 参数
```text
tar: Removing leading '/' from member names
```
```shell
# f只能位于结尾
tar -zcvPf test.tar.gz test
```