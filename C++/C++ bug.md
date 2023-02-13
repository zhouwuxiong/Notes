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


## map 的 end() 中 Key 存储的是 map 的长度，valu=0
![map find 测试程序](../test/STL%20Map/map_find.cpp)
```C++
  // simple comparison demo
  std::map<int, bool> example = {{1, true}, {2, true}, {3, true}};

    // if (search != example.end())
  cout << " example.size(): " << example.size() << endl;
  cout << " example.end().first: " << example.end()->first << endl;
  cout << " example.end().second: " << example.end()->second << endl;

  auto search = example.find(4); // find不到，返回 example.end()
  if (example.end()->second == false) // example.end()->value 永远等于 0，所以一直判断为 true
  {
    std::cout << "Found " << search->first << " " << search->second << '\n';
    cout << example.size() << endl;
  }
  else
    std::cout << "Not found\n";
```
总结：
使用 map 的 find() 函数时，一定要判断是否等于 end（） 

## vector erase 时，导致迭代其失效
![vector erase bug](../test/vector.erase/erase.cpp)

## boost 反序裂化指针时，会自动分配内存，需要在外部手动释放
![boost Serialization bug](../test/boost/boostLoadPoint.cpp)