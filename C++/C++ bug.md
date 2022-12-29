## `error: use of deleted function in c++`
c++中如果不显示的提供构造函数, 编译器会给一个默认的构造函数, 但在钻石结构的继承关系中, 默认构造函数显然
是有问题的, 因此构造函数不会提供默认版本. 因此会触发该错误.

1. `use of deleted function ‘std::mutex::mutex(const std::mutex&)`
mutex显示的删除了编译器默认生成的拷贝构造和移动构造函数，所以mutex对象不能进行拷贝构造和移动构造
```C++
    mutex(const mutex&) = delete;
    mutex& operator=(const mutex&) = delete;
```

## C++ 的锁 lock 不能加在 const 函数中
由于函数是const的，那么成员变量 lock 是不可修改的，而lock 加锁是要修改 lock 变量的，这显然冲突了.

[C++使用mutable在const成员函数中加锁](https://www.cnblogs.com/coding-my-life/p/16030719.html)

[error: no matching function for call to 'boost::shared_lock<boost::shared_mutex>::shared_lock(const Lock&)'](https://stackoverflow.com/questions/40414777/error-no-matching-function-for-call-to-boostshared-lockboostshared-mutex)