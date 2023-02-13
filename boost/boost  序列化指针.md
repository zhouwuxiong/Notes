
## boost 学 
指向任何实例的指针都可以用 Boost 完成序列化和反序列化的操作,Boost 在实现序列化指针的时候,其实内部实现了以下几个可能被我们忽略的点：
1. 如果同一个对象通过不同的指针多次保存，则只需要保存该对象的一个副本。
2. 如果一个对象通过不同的指针被多次加载，那么应该只创建一个新对象，并且所有返回的指针都应该指向它。
3. 派生类的对象可以通过基类的指针存储。必须确定并保存对象的真实类型。恢复后，必须创建正确的类型，并将其地址正确地转换到基类。也就是说，必须考虑多态指针。
4. NULL指针必须在保存时检测到，并在反序列化时恢复为NULL。
5. boost 会自动为指针分配内存空间（在堆区），并调用无参构造函数进行初始化。即使在 load 之前手动使用 new 为将要 load 的指针分配了内存空间， boost 也不会使用该内存，反而会导致内存地址丢失，产生内存泄漏问题。![boost内春分配测试](boostTest/boostLoadPoint.cpp)，但是 boost 自动创建的内存需要用户自己手动释放（真坑），或更建议使用智能指针（boost库自带了boost::shared_ptr的实现）。详见[List of Examples 中的 Demo.cpp](https://www.boost.org/doc/libs/1_81_0/libs/serialization/doc/index.html)
6. 函数在调用 delete 时，没有立即归还内存，下次 new 相同的对象时，会在直接分配到之间的地址空间。![boost内存释放测试](boostTest/boostMemoryTest.cpp)


## boost 序列化多态类型
1. 如果基类中没有虚函数，那么就认为不需要实现多态，指针内自然保存的是指向基类的实例，
2. 如果基类中存在虚函数，那么boost库便会认为需要实现多态的序列化操作，这都交由库自己实现。boost给出了两种实现方式：
- registration
- export

## 问题：boost反序列化指针时自动分配的内存，在何时进行释放？是否需要手动释放？
boost
[boost serialize Pointer Demo](https://www.boost.org/doc/libs/1_81_0/libs/serialization/example/demo.cpp)

## 参考
[Boost – 序列化 (Serialization)](http://www.yhwang.club/boost-%E5%BA%8F%E5%88%97%E5%8C%96-serialization/)