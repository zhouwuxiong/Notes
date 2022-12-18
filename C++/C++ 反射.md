 ## 变量类型推理
 ### decltype 关键子
 ```C++
 int a;
 auto b   // auto 根据 = 右边的初始值 推理类型
 decltype(a) c; // decltype 跟高 （）内的表达式推理类型，decltype（）内的类型不能为 void 否则会产生编译错误
 assert(typeid(a)==typeuid(b));
 cout<<typeid(a).name()<<","<<typeid(b).name()<<endl;
 ```
typeid是一个很特殊运算符，对于没有虚函数表指针的对象是在编译期间确定类型信息的，对于有虚函数表指针的对象在运行时才能准确的确定类型信息

 因为decltype和模板类型都是在编译时确定的，为什么不能使用decltype给模板指定类型？？难道是为了避免循环推理？
 ```C++
   // TypesLimits::getRandomLimits<decltype(a)>(a);
  // TypesLimits::getRandomLimits<decltype(b)>(b);
 ```
 ## 什么反射?
 反射机制是在运行状态中，对于任意一个类，都能够知道这个类的所有属性和方法，对于任意一个对象，都能够调用它的任意一个方法和属性，这种动态获取的信息以及动态调用对象的方法的功能称为语言的反射机制。

## c++ 反射库

["全球最强" | 99 行 C++ 静态反射库](https://zhuanlan.zhihu.com/p/158147380) 