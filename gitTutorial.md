# git使用教程
## 基本概念
1. 四个区
   工作区：电脑本地目录
   版本库：.git目录
   暫存区：.git目录下的index文件，每个分支的暫存区是不一样的
   远程仓库：

5. 五种状态
   未修改（Origin）
   已修改（Modified）
   已暂存（Staged）
   已提交（Committed）
   已推送（Pushed）

## 开始
### 初始化仓库
1. 安装git
   
2. git init
### 关联远程仓库
1. git remote add origin code@github.git  //绑定本地和远程仓库,origin为仓库链接的别名，可自定义
### 添加的登录信息
1. 
   git config --global user.name zhouwuxiong
   git config --global user.email *@*
   git config --list
   git config --global credential.helper store //下次输入密码后，自动保存

2. 配置ssh
   https://blog.csdn.net/weixin_57390669/article/details/115874349

## 分支管理
1. 查看分支
   git branch -l/a

2. 创建分支
   git branch “name”
   git brach -b “name” //创建并切换

3. 切换分支
    git checkout "name"
    git checkout  //没有指定参数时，会用HEAD（master）分支中的文件件替换工作区中的文件（注意：这会删除工作区中未添加到暫存区中的改动,但不会影响新增文件）

4. 删除分支
   git -d “name”
## 文件比较
   git diff branch1 branch2 --stat   //显示出所有有差异的文件列表
   git diff branch1 branch2 文件名(带路径)   //显示指定文件的详细差异
   git diff branch1 branch2                   //显示出所有有差异的文件的详细差异

## 文件提交
工作区 - add -> 暂存区 - commit -> 本地仓库 - push -> 远程仓库
1. 暂存区
   git add “name”       //添加指定到暫存区
   git add --update     //添加所有修改、删除文件
   git add --all        //添加修改、删除、新增文件
   
   git ls-files         //查看暫存区文件
   git diff             //查看暫存区文件与工作区文件的差异

2. 本地仓库
   git commit           //暫存区中的内容添加到本地仓库
   git commit -a        //直接提交，不需要add

   git status           //显示仓库状态

3. 远程仓库
   git remote add “name” “url”
   git push <远程主机名> <本地分支名>:<远程分支名>
   git remote -v                          //查看远程关联
   git add “主机名” “url”                  //关联远程仓库与本地仓库
   git remove “name”                      //取消与远程仓库的关联
   git push origin --delete Chapater6     //删除远程分支

## 版本管理
1. 查看提交历史
   git log
2. 添加标签
   git tag -a v1.0
3. 版本回退
   git reset –hard HEAD ^        //回退到上个版本
   git reset –hard HEAD ^^       //回退到上上个版本

   git reset -hard "version"     //回退到指定版本
   git reflog                    //查看版本号

## stash
当A分支没有修改完成，此时需要切换到B分支时，需要将当前修改进行存储，否则checkout时，会覆盖未commit的修改内容。使用stash存储分支后，git status将不显示未提交的修改内容。
   git stash                  //存储当前分支
   git stash list             //显示存储列表
   git stash apply            //恢复分支，但不删除stash
   git stash pop              //恢复分支，并删除stash
   git stash drop             //弃用stash 

## 分支合并
1. 将dev分支的某个文件f.txt合并到master
1）
   git checkout master
2）比较单个文件的差异，将dev分支上 f 文件追加补丁到master分支上 f文件，需要在目标分支下执行，并通过交互程序选择接受哪些补丁
   git checkout --patch dev f.txt   
3）将f.txt copy 到master分支
   git checkout dev f.txt

2. 合并某个分支上的单个commit
1）
git checkout master  
2）
   git cherry-pick "dev分支提交记录的Hash直"


3. 合并某个分支上的一系列commits
   假设你需要合并dev分支的commit76cada ~62ecb3 到master分支：
1）
   git checkout dev
2）
   git checkout -b newbranch 62ecb3
3）rebase这个新分支的commit到master，76cada^ 指明你想从哪个特定的commit开始。
   git rebase --onto master 76cada^ 

4. 合并分支
1）
   git checkout dev
2）
   git  merge dev

## 文件重命名
1)git通过文件名追踪文件，如果直接更改文件名，会变成deleted 和 untracked
   git mv -f            //强制移动，覆盖目标文件
   git mv -k            //跳过重名文件和出错文件
