## C++中的线程安全问题
### STL容器与线程安全
**STL容器不是线程安全的**，对于vector，即使写方（生产者）是单线程写入，但是并发读的时候，由于**潜在的内存重新申请和对象复制问题**，会导致读方（消费者）的迭代器失效。实际表现也就是招致了core dump。另外一种情况，如果是多个写方，**并发的push_back()**，也会导致core dump。

[C++线程安全](https://www.cherryc4t.com/?p=240)
[手动实现 C++并发容器](https://www.jianshu.com/p/1e1af10054a2)

## 如何解决线程安全
### 原子操作 atomic
原子操作通过定义的原子变量及其对应的原子函数保证数据的安全性

### std::call_once 与 std::once_flag
std::call_once 函数，可以注册给所有可调用的函数、lambda等(callable)。
std::once_flag 保证，只有一个注册的功能将被调用。
可以通过once_flag注册更多不同的函数，但是只会有一个函数被调用
```C++
// callOnce.cpp

#include <iostream>
#include <thread>
#include <mutex>

std::once_flag onceFlag;

void do_once(){
  std::call_once(onceFlag, [](){ std::cout << "Only once." << std::endl; });
}

int main(){

  std::cout << std::endl;

  std::thread t1(do_once);
  std::thread t2(do_once);
  std::thread t3(do_once);
  std::thread t4(do_once);

  t1.join();
  t2.join();
  t3.join();
  t4.join();

  std::cout << std::endl;

}

```

### 常量表达式
常量表达式是编译器在编译期间可以初始化的表达式。所以，它们是隐式线程安全的。通过在表达式类型前面使用关键字constexpr使其成为常量表达式:
constexpr double pi = 3.14;
用户定义的类型也可以是常量表达式。对于这些类型，在编译时初始化它们有一些限制：

[C++线程安全的数据初始化](https://blog.csdn.net/y396397735/article/details/81125014)


## sleep与wait的区别
- sleep() 不会释放 mutex，wait 会释放 mutex
- 