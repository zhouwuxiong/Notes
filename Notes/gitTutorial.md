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
   git branch -vv

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

## 删除文件
1）仅仅删除远程分支文件，不删除本地文件
   git rm -r cached "name"
   git commit -m "delete *"
   git push "sever name" "branch name"


## git用户管理
git 的配置分为三级别，System —> Global —>Local。System 即系统级别，Global 为配置的全局，Local 为仓库级别，优先级是 Local > Global > System。
```
全局用户 global   ~/.gitconfig
系统用户 system   /etc/gitconfig
局部用户 Local   .git/config
```
在执行git config命令时，通过 --global 指定操作哪个配置文件

### 管理用户信息
1. 查看本地用户名 
git config user.name
git config user.email

2. 查看所有配置(./git/config)
git config --list

3. 删除用户
   git config --unset user.name  


### git本地多用户
在本地配置多个git用户，区分每个用户使用的密钥
https://blog.csdn.net/yuanlaijike/article/details/95650625

#### 密钥生成
1. 密钥生成
   ssh-keygen -t rsa -C “jitwxs@foxmail.com”
注：
   第一个输入为莫要保存的文件名，可以在这里为不同的帐号保存为不同的文件名，也可以在2中手动重命名
2. 重命名密钥
   mv ~/.ssh/id_rsa ~/.ssh/id_rsa_zwx
   mv ~/.ssh/id_rsa.pub ~/.ssh/id_rsa_zwx.pub
3. 添加密钥（可选）
   ssh-add命令是把专用密钥添加到ssh-agent的高速缓存中,从而提高ssh的认证速度
   ssh-add ~/.ssh/id_rsa_github
4. 查看本地缓存中的密钥
   ssh-add -l
5. 将公钥添加到网页仓库的ssh key
   ...
   
#### 密钥管理
1.   创建密钥配置文件，实现根据仓库的remoate链接地址自动选择合适的密钥
   vim ~/.ssh/config
```text
Host gitee
HostName gitee.com
User zhouwuxiong
IdentityFile ~/.ssh/id_rsa_zwx

Host gitee
HostName e.gitee.com
User zhouwuxiong-dominant-tech
IdentityFile ~/.ssh/id_rsa_domain
```
2. 测试连接
   ssh -T git@gitee.com

##### 设置密码保存
https的url方式每次push的时候都要输入密码，比较麻烦，一般就会用credential.helper把账号密码缓存到本地。
1. 查看git配置文件的信息
2. 删除global或system中的密码
   git config --global --unset credential.helper
3. 设置local中的密码
   git config --local credential.helper store

注：
   - 当首次ssh链接服务器时，服务器会下发公钥，本地将公钥存储在known_hosts文件中，下次ssh服务器时，会将服武器下发的公钥与本地known_hosts中的公钥进行比对。避免本地主机受到DNS Hijack（DNS劫持）之类的攻击。并且在本地主机首次 ssh -T 时也会提示，是否信任服务器的链接。

注：
   git的凭证存储方式：
   - cache，存储在内存中，15分钟后清除
   - store  密码会以明文的方式存储在~/.git-credentials中
   - osxkeychain mac 存储在系统文件中，有系统管理
   - manageer windwos 通上

注：
   多用户场景下，store模式会产生冲突，因为如果你设置了.git-credentials=store时，git会使用~/.git-credentials中的密码直接登录，而不会判断是否为当前用户的密码，因此如果你发现git无法登录， 且./git/config中记录的信息正常时，你可能就需要检查~/.git-credentials，看看是否使用了其它的用户名和密码。
   （这种多用户冲突在使用浏览器登录时也会发生）