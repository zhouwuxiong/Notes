## shell中记录变量
1. 全局环境变量
/etc/profile
2. 用户环境变量
~/.bash_profile
3. 临时环境变量（窗口关闭后清除）
export a=b

```shell
# 该脚本的主要目的是替换linux中的rm命令，防止误删除文件
# 将 rm 命令绑定到 mv 命令，rm文件时，将文件移动到～/.trash文件中
# 使用该脚本前需要先建立～/.trash文件，然后更改文件夹的权限
# 参考至 https://zhuanlan.zhihu.com/p/378585542

# rm transform
function rm() {
    # 定期清理回收站,时间可调整
    now=$(date +%s)
    for s in $(ls --indicator-style=none $HOME/.trash/) ;do
	dir_name=${s//_/-}
        dir_time=$(date +%s -d $dir_name)
        # if big than one month then delete
        if [[ 0 -eq dir_time || $(($now - $dir_time)) -gt 2592000 ]] ;then
            echo "Trash " $dir_name " has Gone "
            /bin/rm $HOME/.trash/$s -rf
	   # echo $s
        fi
    done
    # 重写rm命令,将文件mv到回收站下
    prefix=$(date +%Y_%m_%d)
    hour=$(date +%H)
    mkdir -p $HOME/.trash/$prefix/$hour
    if [[ -z $1 ]] ;then
            echo 'Missing Args'
        return
    fi
    echo "Hi, 小姐姐 " ${!#} "已被移到回收站了呢~"
    mv ${!#} $HOME/.trash/$prefix/$hour
    # 记录最后一次删除的文件，以便恢复
    export restorefile="mv $HOME/.trash/$prefix/$hour/${!#} `pwd`/${!#}"
}

function mr() {
echo $restorefile
$restorefile
}
```
