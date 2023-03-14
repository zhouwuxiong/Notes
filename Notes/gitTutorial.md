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
1. 添加用户名
```shell
   git config --global user.name zhouwuxiong
   git config --global user.email *@*
   git config --list
   git config --global credential.helper store # 下次输入密码后，自动保存
```
 2. 配置ssh
   https://blog.csdn.net/weixin_57390669/article/details/115874349



## 分支管理
```shell
# 1. 查看分支
   git branch -l/a
   git branch -vv

# 2. 创建分支
   git branch “name”
   git brach -b “name” # 创建并切换

# 3. 切换分支
    git checkout "name"
    git checkout  # 没有指定参数时，会用HEAD（master）分支中的文件件替换工作区中的文件
    （注意：这会删除工作区中未添加到暫存区中的改动,但不会影响新增文件）

# 4. 删除分支
   git -d “name”
```
## 文件比较
```shell
   git diff branch1 branch2 --stat            # 显示出所有有差异的文件列表
   git diff branch1 branch2 文件名(带路径)      # 显示指定文件的详细差异
   git diff branch1 branch2                   # 显示出所有有差异的文件的详细差异
```
## 文件提交
工作区 - add -> 暂存区 - commit -> 本地仓库 - push -> 远程仓库
```shell
# 1. 暂存区
   git add “name”       # 添加指定到暫存区
   git add --update     # 添加所有修改、删除文件
   git add --all        # 添加修改、删除、新增文件
   
   git ls-files         # 查看暫存区文件
   git diff             # 查看暫存区文件与工作区文件的差异

# 2. 本地仓库
   git commit           # 暫存区中的内容添加到本地仓库
   git commit -a        # 直接提交，不需要add

   git status           # 显示仓库状态

# 3. 远程仓库
   git remote add “name” “url”
   git push <远程主机名> <本地分支名>:<远程分支名>
   git remote -v                          # 查看远程关联
   git add “主机名” “url”                  # 关联远程仓库与本地仓库
   git remove “name”                      # 取消与远程仓库的关联
   git push origin --delete Chapater6     # 删除远程分支
   git pull origin master:brantest        # 将远程主机 origin 的 master 分支拉取过来，与本地的 brantest 分支合并。
```

## 版本管理
```shell
# 1. 查看提交历史
   git log
# 2. 添加标签
   git tag -a v1.0
# 3. 版本回退
   git reset –hard HEAD ^        #回退到上个版本
   git reset –hard HEAD ^^       #回退到上上个版本

   git reset -hard "version"     #回退到指定版本
   git reflog                    #查看版本号
```
## stash
当A分支没有修改完成，此时需要切换到B分支时，需要将当前修改进行存储，否则checkout时，会覆盖未commit的修改内容。使用stash存储分支后，git status将不显示未提交的修改内容。
```shell
   git stash                  # 存储当前分支
   git stash list             # 显示存储列表
   git stash apply            # 恢复分支，但不删除stash
   git stash pop              # 恢复分支，并删除stash
   git stash drop             # 弃用stash 
```
## 分支合并
1. 将dev分支的某个文件f.txt合并到master
```shell
# 1)
   git checkout master
# 2）比较单个文件的差异，将dev分支上 f 文件追加补丁到master分支上 f文件，需要在目标分支下执行，并通过交互程序选择接受哪些补丁
   git checkout --patch dev f.txt   
# 3）将f.txt copy 到master分支
   git checkout dev f.txt
```

2. 合并某个分支上的单个commit
```shell
# 1）
git checkout master  
# 2）
   git cherry-pick "dev分支提交记录的Hash直"
```

3. 合并某个分支上的一系列commits
   假设你需要合并dev分支的commit76cada ~62ecb3 到master分支：
```shell
# 1）
   git checkout dev
# 2）
   git checkout -b newbranch 62ecb3
# 3）rebase这个新分支的commit到master，76cada^ 指明你想从哪个特定的commit开始。
   git rebase --onto master 76cada^ 
```

4. 合并分支
```shell
# 1）
   git checkout dev
# 2）
   git  merge dev
```
## 文件重命名
1)git通过文件名追踪文件，如果直接更改文件名，会变成deleted 和 untracked
```shell
   git mv -f            # 强制移动，覆盖目标文件
   git mv -k            # 跳过重名文件和出错文件
```
## Git排除目录
### 单个工程
```shell
echo *.tar>>ProjectsRootPath/.gitignore
```
或者直接编辑
```text
# temp ignore
*.log
*.cache
*.diff
*.patch
*.tmp

# file ignore
*.zip
*.tar
*.tar.gz

# target file
build/
*bin*/
data/
```
### 全局设置排除文件
使用命令方式可以配置全局排除文件 
```shell
git config --global core.excludesfile ~/.gitignore
```
`~/.gitconfig`文件中会出现`excludesfile = c:/Users/zhbpeng/.gitignore`。说明Git把文件过滤规则应用到了Global的规则中。
### 排除已经提交的文件（删除文件）
```shell
echo 'build/' >> .gitignore
git rm -r --cached "build"
git commit -m "delete *"
git push "sever name" "branch name"
```
## git用户管理
git 的配置分为三级别，System —> Global —>Local。System 即系统级别，Global 为配置的全局，Local 为仓库级别，优先级是 Local > Global > System。
```
全局用户 global   ~/.gitconfig
系统用户 system   /etc/gitconfig
局部用户 Local   .git/config
```
在执行git config命令时，通过 --global 指定操作哪个配置文件

### 管理用户信息
```shell
# 1. 查看本地用户名 
   git config user.name
   git config user.email

# 2. 查看所有配置(./git/config)
   git config --list

# 3. 删除用户
   git config --unset user.name  
```

### git本地多用户
在本地配置多个git用户，区分每个用户使用的密钥
https://blog.csdn.net/yuanlaijike/article/details/95650625

#### 密钥生成
```shell
# 1. 密钥生成
   ssh-keygen -t rsa -C “jitwxs@foxmail.com”
# 注： 第一个输入为莫要保存的文件名，可以在这里为不同的帐号保存为不同的文件名，也可以在2中手动重命名
# 2. 重命名密钥
   mv ~/.ssh/id_rsa ~/.ssh/id_rsa_zwx
   mv ~/.ssh/id_rsa.pub ~/.ssh/id_rsa_zwx.pub
# 3. 添加密钥
   # ssh-add命令是把专用密钥添加到ssh-agent的高速缓存中,从而提高ssh的认证速度
   ssh-add ~/.ssh/id_rsa_github
# 4. 查看本地缓存中的密钥
   ssh-add -l
# 5. 将公钥添加到网页仓库的ssh key
   ...
```
#### 密钥管理
1.   创建密钥配置文件，实现根据仓库的remoate链接地址自动选择合适的密钥
```shell
   vim ~/.ssh/config
```
文件内容如下：
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
```shell
   ssh -T git@gitee.com
```
##### 设置密码保存
https的url方式每次push的时候都要输入密码，比较麻烦，一般就会用 credential.helper 把账号密码缓存到本地。
```shell
# 1. 查看git配置文件的信息
# 2. 删除global或system中的密码
   git config --global --unset credential.helper
# 3. 设置local中的密码
   git config --local credential.helper store
```
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

## 问题
-    [[rejected] master -> master (fetch first)问题的解决方案](https://blog.csdn.net/weixin_44118318/article/details/85030461)


[git如何处理软连接](https://blog.csdn.net/king_way/article/details/94997434)
  
[Git 新建分支出现报错：“fatal: Not a valid object name: ‘master‘.“](https://blog.csdn.net/Lakers2015/article/details/112320092)
根据提示可以知道，原因是没有一个叫’master’的提交对象。你也可以执行一下git branch，会发现没有看到本地分支列表（没有内容）：
```shell
git branch
branch -a
```
其实，要先进行一次commit操作（进行一次提交操作），才会真正建立master分支。这是因为分支的指针要指向提交的,只有进行了提交，才有指针指向该分支，才算是真正的建立了分支，成为一个有效的对象。

# 应用实例
## 1. git clone 指定分支
```shell
git clone -b 分支名 url
```

## 2. git比较不同分支
方案一：
将一个分支存在本地，然后与远程分支进行比较
工具：
1. vscode
2. vscode git插件，gitlens插件
```shell
# 1.下载原始版本
git clone -b * url
# 2.关联远程仓库
git remote add origin url
# 3. 配置账户信息
...
# 4. 将远程分支关联到本地分支
git pull origin dev:dev
# 5. 比较分支差异
点击 vscode git图标 -> 点击最下方`SEARCH & COMPARE`（gitLens插件）-> 在平面上方弹出的窗口选择要对比的远程仓库和本地仓库
```
方案二：
使用 `git diff` 命令比较，但是输出是txt文件，不能一对一文件进行比较

### 比较不同版本中的文件
```C++
git diff                                       查看尚未暂存的文件更新了哪些部分
 
git diff filename 　　　　　　　　　　　　　　　　　 查看尚未暂存的某个文件更新了哪些
 
git diff –cached                    　　　　　　 查看已经暂存起来的文件和上次提交的版本之间的差异
 
git diff –cached filename 　　　　　　　　　　    查看已经暂存起来的某个文件和上次提交的版本之间的差异
 
git diff ffd98b291e0caa6c33575c1ef465eae661ce40c9 b8e7b00c02b95b320f14b625663fdecf2d63e74c 查看某两个版本之间的差异
 
git diff ffd98b291e0caa6c33575c1ef465eae661ce40c9:filename b8e7b00c02b95b320f14b625663fdecf2d63e74c:filename 查看某两个版本的某个文件之间的差异
```
## 3. 使用远程分支覆盖本地分支
git fetch --all
git reset --hard origin/master (这里master要修改为对应的分支名)
git pull


## 4. 使用rebase实现commit压缩
rebase(变基)主要用来整合来自不同分支的修改，与merge要达到的目的一样。

它的原理是首先找到两个分支 的最近共同祖先 ，然后对比当前分支相对于该祖先的历次提交，提取相应的修改并存为临时文件， 然后将当前分支指向目标基底 , 最后以此将之前另存为临时文件的修改依序应用。

```shell
# 1. 查看那comit记录，
git log --pretty=format:"hash: %h commit message: %s"

# 2. 回退HEAD节点
git rebase -i HEAD~4
# or
git rebase -i --root

# 3. 修改交互文件->保存退出
```
![rebase交互文件命令](../images/git-rebase-detail.png)


## git stash
stash区域就是一个缓存区，但是这个缓存区不同于add命令的缓存区。stash的缓存区是一个栈的结构，用来储存你暂时不想commit的代码修改，并且你的工作空间就会回到修改之前的状态。

新建的，但是还没有使用git add 命令处理的文件不会被 stash

stash 是跨分支的，你在master 存入的修改可以应用于其他任何分支上

## 版本切换
### git reset
git reset的作用是修改HEAD的位置，即将HEAD指向的位置改变为之前存在的某个版本,如果想恢复到之前某个提交的版本，且那个版本之后提交的版本我们都不要了

Git允许我们使用命令`git reset --hard commit_id`在版本的历史之间切换。

HEAD可以用来替换commit_id，HEAD指向的版本是当前版本，上一个版本就是HEAD^ ，上上一个版本就是HEAD^^ ，往上100个版本写成HEAD~100。

^和~这两个特殊符号，除了用在HEAD上外，还可以用在<branchName>上或Commit ID上。如master^，代表master的上一个版本；1f2f476~1代表Commit ID为1f2f476的上一个版本。

可以用`git log`可以查看提交历史，或者用`git reflog`查看命令历史，以便确定要切换的版本的版本号。

**Notice**
```shell
git reset --soft  ：1.仅在本地版本库移动指针。
git reset --mixed : 1.移动本地版本库的指针；2.重置暂存区。（默认的参数）
git reset --hard  : 1.移动本地版本库的指针；2.重置暂存区；3.重置工作区。（可以重新编辑代码）
```
### git revert
git revert是用于“反做”某一个版本，以达到撤销该版本的修改的目的。比如，我们commit了三个版本（版本一、版本二、 版本三），突然发现版本二不行（如：有bug），想要撤销版本二，但又不想影响撤销版本三的提交，就可以用 git revert 命令来反做版本二，生成新的版本四，这个版本四里会保留版本三的东西，但撤销了版本二的东西。

### git checkout
如果你要在此基础上要进行新的迭代修改，那么就需要将这个版本升级成为一个单独的分支，以此作为媒介进行新一轮的迭代。
```shell
# 查看当前分支状态
git branch
# 创建并切换到新分支
git switch -c [branch name]
```
## 本地分支与远程分支合并
```shell
# 1. 查看关联的远程仓库
git remote -v
# 2. 下载远程分支代码到本地分支
git fetch origin master:temp
# 3. 查看temp分支与本地原有分支的不同
git diff temp
# 4. 将temp分支和本地分支合并
git merge temp
# 5. 消除冲突
# 6. 删除 temp 分支
git branch -d temp
```
git fetch 从远程分支拉取代码，常结合merge一起用，`git fetch + git merge == git pull`，git pull 需要先解决冲突再进行拉取代码。常见的 git pull 冲突解决方法有：
1. 暂存本地修改
```
1、git pull  //提示冲突后
2、git stash //放暂存区
3、git pull //成功拉下代码
4、git stash pop //将暂存区的代码还原，会提示代码冲突，解决冲突代码的文件
5、git add .  //重新提交一次
6、git commit -m "冲突"
7、git push  
```
2. 忽略本地修改，强制使用远程分支覆盖本地分支（代码修改较少）
git reset --hard origin/dev
3. 回退本地代码到未修改的版本（代码修改较少）
git reset
4. 冲突解决

### 合并不同分支中的单个文件
git checkout --patch

[git pull时冲突的几种解决方式](https://www.cnblogs.com/zjfjava/p/10280247.html)

## 删除远程提交到远程仓库中的某个历史版本
1. git revert HEAD
revert是放弃指定提交的修改，但是会生成一次新的提交。
2. git reset --hard HEAD^
放弃最后一次提交
3. git rebase 
删除或修改历史某次提交，需要注意的是，在执行rebase命令对指定提交修改或删除之后，该次提交之后的所有提交的”commit id”都会改变。

## commit 信息合并
1. git commit --amend
将当前修改合并到上次 commit
2. git commit --amend -m "new commit"
将当前修改合并到上次 commit，并修改commit注释


## 清理误上传的文件
项目开发初期由于.gitignore 文件配置不正确很有可能导致某些不需要的目录上传到 git 远程仓库上了，这样会导致每个开发者提交的时候这些文件每次都会不同。除了一开始提交的时候注意配置好 .gitignore 文件外，我们也需要了解下出现这种问题后的解决办法。
```shell
# 预览删除（可以防止删除后，程序找不到路径或使用正则匹配进行删除时，查看匹配结果）
git rm -r -n --cached 文件/文件夹名称 
# 真正删除,被删除的文件相当于没有被 add，但是本地工作区中还在。
git rm -r --cached 文件/文件夹名称
git commit -m "提交说明"
# --force 回强制使用本地的git数据库覆盖远程的。可能会导致与本地不一致的远程版本丢失（慎用）
git push origin master --force
```

# git submodule
## 新建项目中添加 submodule
```shell
git submodule add <子模块git地址> <存放的文件名>
```
执行该命令后会在当前目录生成: .gitmodules 和 子模块文件夹：

1. .gitmodules 记录了本地文件与URL之间的映射关系
```conf
[submodule "ProcessManage"]  # 子模块名
	path = ProcessManage
	url = git@gitee.com:dominanttech/Dominant_vslam_ProcessManage.git

```
2. 新 ProcessManage 文件，并 clone 子模块
注意：
   1. 子模块文件夹只存子项目的commit id，父项目的 git 不会记录子模块的文件改动，而是通过 commit id 指定使用子模块的哪个commit版本。 
   2. 如果 ProcessManage 文件夹已存在，可能会创建失败，也可以按照 [git submodule 完整用法整理](https://blog.csdn.net/wkyseo/article/details/81589477) 中的方式创建。

3. .git/config 文件中添加 submodule 记录.
```text
[submodule "ProcessManage"]
	url = git@gitee.com:dominanttech/Dominant_vslam_ProcessManage.git
	active = true
```

4. 创建 ./git/modules/ProcessManage 文件夹，其中存储了子模块的 .git 中的内容，文件 ProcessManage/.git 变成了一个文件,其中记录了 .git文件夹被存放到了主.git/module的目录下.如下：
```text
gitdir: ../.git/modules/ProcessManage
```
## 将已有项目中的子文件夹变为 submodule
### 添加 submodule
```shell
# 1. 重命名文件夹
mv ProcessManage ProcessManage_
# 2. 在当前项目中添加 submodule
git submodule add <子模块git地址> Dominant_vslam_ProcessManage
# 3. 删除 submodule 中需要覆盖的文件（include、src等）
cd Dominant_vslam_ProcessManage && rm -rf include && rm -rf src
# 4. 使用现有项目的文件，覆盖 submodule 中需要覆盖的文件
cd Dominant_vslam_ProcessManage ； cp ../ProcessManage_/include . && cp ../ProcessManage_/src .
# 5. 在主git中编译测试
cd build || cd ../build && cmake .. && make -j4
```
### 添加的  submodule 与现有文件夹重名
再添加 submodule 时，可以自定义文件夹名，但是为了尽量不影响其他人的使用（尽量不修改submodule的cmakeList文件），这个文件名应该要与 submodule 中的文件名一致，但是当项目中已有的子文件夹与要添加的submodule重名时。`git submodule add` 命令会报错。这时需要删除主 git 数据库中对重名文件夹的记录。过程如下：
（在第上节中的第1步之前）
```shell
# 1. 在 git 的缓存中删除文件夹，相当于执行 git add 的逆操作
git rm -r --cached "build"
# 2. 将缓存的修改提交到 git 数据库（可选，因为下次 commit 时这一步会自动执行）
git commit 
```
或者使用 git 的 mv 指令。
```shell
# git mv 应该可以替换上一节第一步中的 mv 和上面的 git rm 操作（没测试过）
git mv <old-filename> <new filename>
```
然后继续执行上一节中的剩余操作

# .git 文件解析
```text
drwxrwxr-x   8 zwx  zwx   4096 Feb 16 17:02 ./
drwxrwxr-x  24 zwx  zwx   4096 Feb 16 17:58 ../
drwxrwxr-x   2 zwx  zwx   4096 Jan  3 10:47 branches/
-rw-r--r--   1 domi domi    11 Feb 16 17:02 COMMIT_EDITMSG
-rw-rw-r--   1 zwx  zwx    356 Feb  3 09:49 config
-rw-rw-r--   1 zwx  zwx     73 Jan  3 10:47 description
-rw-r--r--   1 domi domi    20 Feb 13 16:38 HEAD
drwxrwxr-x   2 zwx  zwx   4096 Jan  3 10:47 hooks/
-rw-r--r--   1 domi domi 15236 Feb 16 17:02 index
drwxrwxr-x   2 zwx  zwx   4096 Jan  3 10:47 info/
drwxrwxr-x   3 zwx  zwx   4096 Jan  3 10:48 logs/
drwxrwxr-x 260 zwx  zwx   4096 Jan  4 18:21 objects/
-rw-r--r--   1 domi domi    41 Feb 13 16:43 ORIG_HEAD
-rw-rw-r--   1 zwx  zwx    317 Jan  3 10:48 packed-refs
drwxrwxr-x   5 zwx  zwx   4096 Jan  3 10:48 refs/
```
## `./git/logs`
logs文件中记录git的操作日志，其中 
- `./git/log/HEAD` 记录的是当前分支的 commit 信息对应的命令为 `git reflog` 。
- `./git/log/refs/` 中包含了 heads 和 remotes 两个文件夹，其中分别记录了本地分支（stet、tmp）和 远程分支的 commit 信息。
- `./git/log/refs/heads/HEAD`、`./git/log/refs/remotes/HEAD`


# Git Hooks
git 在上传软连接文件时，只会上传文件的指向链接，不会上传软连接文件中的内容。可选的解决方法有两个：
1. 使用 git hook
2. 使用 `mount --bind` +  `/etc/fstab` 自动挂载 
## Client-Side Hooks

git 可以在 `.git/hooks` 文件夹中编写自定义脚本，git 提供了 .sample 的默认 shell 脚本，但是你也可以使用 Ruby、Python 等任意熟悉的语言。在使用特定脚本时，只需要删除 .sample 尾缀即可。
### Committing-Workflow Hooks
commit hooks 可以使用 `git commit --no-verify` 绕过不执行。有的 hooks 需要提供参数，当 hooks 函数执行失败时，会终止提交。
```text
pre-commit              在编辑 commit msg 之前执行，一般用于检查代码格式，清理尾部空格等。
prepare-commit-msg      在生成 default msg 之后，键入 new msg 时执行。
commit-msg              在提交之前执行，一般用于检查 commit msg 的内容
post-commit             在 commit 之后执行。一般用于通知等功能
```
### Email Workflow Hooks

### Other Client Hooks
## Server-Side Hooks
### pre-receive

[自定义 Git - Git 钩子](https://git-scm.com/book/zh/v2/%E8%87%AA%E5%AE%9A%E4%B9%89-Git-Git-%E9%92%A9%E5%AD%90)
[巧用 gitHooks 提交前校验代码](https://cloud.tencent.com/developer/article/1916604)