# map
## [] 与 insert
operator[] 会覆盖已存在元素， insert 会丢弃新值

## [] 与 find
使用find查找，未找到返回空迭代器。使用[]如果未找到对应的键信息，则自行构造一个默认键值对，值为空
**需要注意的是： find 返回得代器，[] 返回引用**

## 总结
**插入元素用 insert，访问和修改用 []**