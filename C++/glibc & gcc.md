## 简介
[gcc与glibc关系](https://blog.csdn.net/native_lee/article/details/113309783)

[理清gcc、libc、glibc、libc++、libstdc++的关系](https://blog.csdn.net/qq_41854911/article/details/122017552)


# glibc 内存管理 (ptmalloc)


## 分配区
一个进程的动态内存，由分配区管理，分配区分为主分配区与非主分配区，一个进程可以持有一个主分配区（主线程创建）和多个非主分配区（支持多线程访问）。

主分配区可以通过 brk 和 mmap 进行内存分配，分主分配区只能通过 mmap 进行内存分配。

## chunk
ptmalloc 通过 malloc_chunk 来管理内存，malloc_chunk 记录了每个chunk的大小并维护链表结构，一段连续的内存被分成多个chunk。每个chunk被分配到不同的bins中

```text
struct malloc_chunk {  
  INTERNAL_SIZE_T      prev_size;    /* Size of previous chunk (if free).  */  
  INTERNAL_SIZE_T      size;         /* Size in bytes, including overhead. */  
  
  struct malloc_chunk* fd;           /* double links -- used only if free. */  
  struct malloc_chunk* bk;  
  
  /* Only used for large blocks: pointer to next larger size.  */  
  struct malloc_chunk* fd_nextsize;      /* double links -- used only if free. */  
  struct malloc_chunk* bk_nextsize; 
};  
```
## bins
bins 是由多个 chunk 组成的链表结构，其记录了进程维护的堆空间（为了避免频繁的系统调用，进程free的对空间不会立即归还各系统）。bins 根据其维护的 chunk 的大小可以分为 fast_bins 、small_bins、unsorted_bins 、large_bins 四类。
bins|nums|size(KB)| descript
-|-|-|-
fast_bins|10|8~80:8| 1. fast bin的chunk的P(未使用标志位)总是设置为1 2. LIFO
unsorted_bins|1|nolimit| unsorted_bins 的chunk大小没有限制
small_bins| 62|16~512:8| 
large_bins | 63|>512|1. large_bins 中每个bins链表记录的的chunk大小不是固定的，而是一个范围

## 特殊chunk
### top chunk
top chunk 是堆最上面的一段空间（堆顶以下的空闲区域），它不属于任何bin，当所有的bin都无法满足分配要求时，就要从这块区域里来分配，分配的空间返给用户，剩余部分形成新的top chunk，如果top chunk的空间也不满足用户的请求，就要使用brk或者mmap来向系统申请更多的堆空间

在free chunk的时候，如果chunk size不属于fastbin的范围，就要考虑是不是和top chunk挨着，如果挨着，就要merge到top chunk中。

### mmaped chunk
当分配的内存非常大（大于分配阀值，默认128K）的时候，需要被mmap映射，则会放到mmaped chunk上，当释放mmaped chunk上的内存的时候会直接交还给操作系统。（chunk中的M标志位置1）

#### last remainder chunk
last remainder chunk 是上一次chunk分裂时剩余的部分，其有 unsorted bin 维护，主要是为了提升内存分配的局部性。

*last remainder chunk主要通过提高内存分配的局部性来提高连续malloc（产生大量 small chunk）的效率*

## 内存分配(malloc)
分配顺序：
**fast_bin(精确分配) -> small_bin(精确分配) -> last_remainder_chunk(可切分) -> unsorted_bin（不可切分）-> large_bin(可切分) -> top_chunk(可切分) -> sbrk() 或 mmap()**
```text
根据用户请求分配的内存的大小，ptmalloc有可能会在两个地方为用户分配内存空间。在第一次分配内存时，一般情况下只存在一个主分配区，但也有可能从父进程那里继承来了多个非主分配区，在这里主要讨论主分配区的情况，brk值等于start_brk，所以实际上heap大小为0，top chunk 大小也是0。这时，如果不增加heap大小，就不能满足任何分配要求。所以，若用户的请求的内存大小小于mmap分配阈值， 则ptmalloc会初始heap。然后在heap中分配空间给用户，以后的分配就基于这个heap进行。若第一次用户的请求就大于mmap分配阈值，则ptmalloc直接使用mmap()分配一块内存给用户，而heap也就没有被初始化，直到用户第一次请求小于mmap分配阈值的内存分配。第一次以后的分配就比较复杂了，简单说来，ptmalloc首先会查找fast bins，如果不能找到匹配的chunk，则查找small bins。若仍然不满足要求，则合并fast bins，把chunk加入unsorted bin，在unsorted bin中查找，若仍然不满足要求，把unsorted bin 中的chunk全加入large bins 中，并查找large bins。在fast bins 和small bins中的查找都需要精确匹配， 而在large bins中查找时，则遵循“smallest-first，best-fit”的原则，不需要精确匹配。若以上方法都失败了，则ptmalloc会考虑使用top chunk。若top chunk也不能满足分配要求。而且所需chunk大小大于mmap分配阈值，则使用mmap进行分配。否则增加heap，增大top chunk。以满足分配要求。
```
**主分配区在第一次调用 malloc 时会进行初始化工作，分配一块大小为(chunk_size + 128KB) align 4KB 大小的空间作为初始的 heap**
## 内存释放(free)
1. 如果是 mmap 映射区域映射的内存，同过 munmap() 直接释放。
2. 如果与 top chunk 相邻，则合并到 top chunk 中
3. 如果chunk的大小大于max_fast（64b），则放入unsorted bin，并且检查是否有合并。
4. 如果chunk的大小小于 max_fast（64b），则直接放入fast bin
5. 在fast bin，如果当前chunk的下一个chunk也是空闲的，则将这两个chunk合并，放入unsorted bin上面。合并后的大小如果大于64B，会触发进行fast bins的合并操作，fast bins中的chunk将被遍历，并与相邻的空闲chunk进行合并，合并后的chunk会被放到unsorted bin中，fast bin会变为空。合并后的chunk和topchunk相邻，则会合并到topchunk中。
6. 判断top chunk的大小是否大于mmap收缩阈值（默认为128KB），如果是的话，对于主分配区，则会试图归还top chunk中的一部分给操作系统。free结束。

## thread cache
如果需要在一个线程内部的各个函数调用都能访问、但其它线程不能访问的变量（被称为static memory local to a thread 线程局部静态变量），就需要新的机制来实现。这就是TLS。

thread cache 本质上是在static区为每一个thread开辟一个独有的空间，因为独有，不再有竞争。

每次malloc时，先去线程局部存储空间中找area，用thread cache中的area分配存在thread area中的chunk。当不够时才去找堆区的area。

[glibc内存管理](https://zhuanlan.zhihu.com/p/428216764)

[内存泄漏之malloc_trim](malloc_trim 参数的最大值)

[The GNU C++ Library Manual](https://gcc.gnu.org/onlinedocs/libstdc++/manual/)