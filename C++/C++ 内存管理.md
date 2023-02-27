
## 内存管理方案
### ptmalloc 
glibc的内存分配管理模块
```text
后分配的内存先释放,因为 ptmalloc 收缩内存是从 top chunk 开始,如果与 top chunk 相邻的 chunk 不能释放, top chunk 以下的 chunk 都无法释放。

多线程锁开销大， 需要避免多线程频繁分配释放。

内存从thread的areana中分配， 内存不能从一个arena移动到另一个arena， 就是说如果多线程使用内存不均衡，容易导致内存的浪费。比如说线程1使用了300M内存，完成任务后glibc没有释放给操作系统，线程2开始创建了一个新的arena， 但是线程1的300M却不能用了。

每个chunk至少8字节的开销很大

不定期分配长生命周期的内存容易造成内存碎片，不利于回收。64位系统最好分配32M以上内存，这是使用mmap的阈值。
```
### tcmalloc
google的gperftools内存分配管理模块
## 参考

[C++内存管理全景指南|C++硬核指南](https://zhuanlan.zhihu.com/p/368901281)

[map容器clear操作不会释放内存？](https://cloud.tencent.com/developer/article/1157252)

(强烈推荐)[[引擎开发] 深入C++内存管理](https://blog.csdn.net/zju_fish1996/article/details/108858577)