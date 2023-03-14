# 保存docker到本地
镜像保存/载入：
docker load/docker save；
将一个镜像导出为文件，再使用docker load命令将文件导入为一个镜像，会保存该镜像的的所有历史记录。比docker export命令导出的文件大，很好理解，因为会**保存镜像的所有历史记录**。

容器导入/导出：
docker import/docker export；
将一个容器导出为文件，再使用docker import命令将容器导入成为一个新的镜像，但是**相比docker save命令，容器文件会丢失所有元数据和历史记录，仅保存容器当时的状态，相当于虚拟机快照**。

```Shell
# 1. 查看运行的 container ID
docker ps -a 
# 2. 保存 docker 中的更新到新的 images
docker commit -m "dominant_test" aaef73633e48 dominant_test:1.0
# 3. 将镜像保存为文件
docker save dominant_test > dominant_test.tar
```

## 镜像操作
### 修改 tag
```shell
# 添加新 tag
docker tag imageId repository:newTag
# 删除原有 tag
docker rmi repository:newTag
```
注意：
当一个images 有多个 tag 时无法通过 `rmi`删除，但是可以通过 `rmi -f` 强制删除。