## 1. 防止多线程频繁加锁导致资源竞争
当多个线程同时访问一个需要加锁的变量时，如果每次都以加锁的方式访问变量，会导致锁资源的竞争。
解决方法：
１. 给不需要实时运行的线程添加 sleep()。
2. 尽量将加锁变量一次访问后缓存到本地，减少访问。
3. 对于需要实时获取直的变量，无法使用缓存直，那么在进行判断时，尽可能放到最后一个。如：
   ```C++
    // 先判断不需要加锁变量的条件，再判断加锁变量，减少对变量的加锁
    isEnd = CheckFinish() && (mpMap->GetMaxKFid() <= mStratKeyFrameID);
   ```

## 加速磁盘存储
在 SLAM 地图分块保存时，在 SLAM 系统运行结束后，需要将剩余未存储的地图保存到磁盘，但是在保存地图点块时，保存一个窗口花费的时间大概是SLAM运行时的10倍左右？

### linux 缓存写操作
因为 linx 系统在执行 i/o 时，并不是直接写入磁盘，只是划分了磁盘的存储区域，需要存储的用户数据会暂存到内核缓存中，然后 i/o 操作完成。这样当下次访问这些数据时，会优先访问缓存，以提升 i/o 的相应速度。 而缓存中的数据需要等到合适的时间才存储 。例如：写入新文件时需要覆盖当前缓存时，则需要先写入磁盘。
```C++
ssize_t generic_perform_write(struct file *file,
        struct iov_iter *i, loff_t pos)
{
  struct address_space *mapping = file->f_mapping;
  const struct address_space_operations *a_ops = mapping->a_ops;
  do {
    struct page *page;
    unsigned long offset;  /* Offset into pagecache page */
    unsigned long bytes;  /* Bytes to write to page */
    // 1. 
    status = a_ops->write_begin(file, mapping, pos, bytes, flags,
            &page, &fsdata);
    // 2.
    copied = iov_iter_copy_from_user_atomic(page, i, offset, bytes);
    flush_dcache_page(page);
    // 3。  
    status = a_ops->write_end(file, mapping, pos, bytes, copied,
            page, fsdata);
    pos += copied;
    written += copied;

    // 4. 
    balance_dirty_pages_ratelimited(mapping);
  } while (iov_iter_count(i));
}
```
上面的代码主要做了 4 件事：
1. write_begin 写文件系统的日志，防止断电时数据丢失。ext4的日志系统一般有 Journal、order、writeback 三个级别。
2. 将用户数据拷贝到内核缓存
*在内核中，缓存以页为单位放在内存里面，每一个打开的文件都有一个struct file结构，每个struct file结构都有一个struct address_space用于关联文件和内存，就是在这个结构里面，有一棵树，用于保存所有与这个文件相关的的缓存页。*
3. 完成日志的写入，将修改过的缓存标记为脏页。
4. 发现脏页的数目超过了规定的数目，则启动数据回写线程。
另外还有几种场景也会触发回写：
- 用户主动调用sync，将缓存刷到硬盘上去，最终会调用wakeup_flusher_threads，同步脏页；*
- 当内存十分紧张，以至于无法分配页面的时候，会调用free_more_memory，最终会调用wakeup_flusher_threads，释放脏页；
- 脏页已经更新了较长时间，时间上超过了设定时间，需要及时回写，保持内存和磁盘上数据一致性*

[linux系统提升硬盘写速度的方法](https://blog.csdn.net/qq_38639426/article/details/101441284)

### linux 缓存读
linux 缓存读就是访问缓存，缓存未命中才读取磁盘。但是有一点要注意的是，linux 会存在**预读**操作。
## 预读
**Linux 把读模式分为随机读和顺序读两大类，并只对顺序读进行预读**
### 预读的目的：
批量，也就是把小I/O聚集为大I/O，以改善磁盘的利用率，提升系统的吞吐量。
提前，也就是对应用程序隐藏磁盘的I/O延迟，以加快程序运行。

*Linux 把读模式分为随机读和顺序读两大类，并只对顺序读进行预读*
### 修改预读大小
预读窗口的初始值是读大小的二到四倍，后续的预读窗口将逐次倍增，直到达到系统设定的最大预读大小，其缺省值是128KB。
```shell
blockdev–setra 2048 /dev/sda
```
详见 [Linux内核的文件预读机制详细详解](https://blog.csdn.net/kunyus/article/details/104620057)

## 参考
[linux系统提升硬盘写速度的方法](https://blog.csdn.net/qq_38639426/article/details/101441284)

[Linux内核的文件预读机制详细详解](https://blog.csdn.net/kunyus/article/details/104620057)
