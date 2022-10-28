### 显示类型转换
#### static_cast
1. 编译器隐式执行的任何类型转换都可以由static_cast显示完成（不能改变const属性）
```C++
const double val=3.14;
double *ptr=NULL;

/*为了使ptr指向val，使用const_cast*/
ptr=const_cast<double *>(&val);
```

#### dynamic_cast
1. dynamic_cast是在运行时进行的，会进行运行时类型检查，而这个信息存储在类的虚函数表中，只有定义了虚函数的类才有虚函数表，故**对没有虚函数表的类使用会导致dynamic_cast编译错误**。
2. 若转换到指针的失败，dynamic_cast的结果是0值，若转换到引用类型的失败，则抛出一个bad_cast类型的异常
3. **要转换的指针指向的对象的实际类型与转换以后的对象类型一定要相同，否则转换失败**
```C++
 struct A {
    virtual void f() { }
  };
  struct B : public A { };
  struct C { };
 
  void f () {
    A a;
    B b;
 
    A* ap = &b;
    B* b1 = dynamic_cast (&a);  // NULL, because 'a' is not a 'B'
    B* b2 = dynamic_cast (ap);  // 'b',ap指向B类型，规则3
    C* c = dynamic_cast (ap);   // NULL.
 
    A& ar = dynamic_cast (*ap); // Ok.
    B& br = dynamic_cast (*ap); // Ok.
    C& cr = dynamic_cast (*ap); // std::bad_cast，规则2
  }
```

#### const_cast
1. const_cast只用于去除一个对象的const或volatile属性
```C++
const double val=3.14;
double *ptr=NULL;
  
/*为了使ptr指向val，使用const_cast*/
ptr=const_cast<double *>(&val)
```

#### reinterpert_cast
1. reinterpert_cast只用于指针间的类型的转换，在编译时重新解释指针的类型
```C++
int *ip=NULL;
char *pc=reinterpret_cast<char *>(ip);
  
/*注：必须牢记pc所指的真实对象是一个int而非字符，如果把pc当成普通的字符指针使用
*就可能在运行时发生错误*/
```

### .hpp文件
.hpp，本质就是将.cpp的实现代码混入.h头文件当中，定义与实现都包含在同一文件，则该类的调用者只需要include该.hpp文件即可，无需再将cpp加入到project中进行编译。而实现代码将直接编译到调用者的obj文件中，不再生成单独的obj，采用hpp将大幅度减少调用project中的cpp文件数与编译次数，也不用再发布lib与dll文件，因此非常适合用来编写公用的开源库。

由于.hpp本质上是作为.h被调用者include的，所以当hpp文件中存在全局对象或者全局函数，而该hpp被多个调用者include时，将在链接时导致符号重定义错误。要避免这种情况，需要去除全局对象，将全局函数封装为类的静态方法。解决办法有：
1. 在声明同时进行定义
2. 类成员函数写成inline，全局变量写成static
3. #pragma onece、#ifdefine等，见上文



### 左值引用与右值
1. 左值与右值
左值是可寻址的变量，有持久性；
右值一般是不可寻址的常量，或在表达式求值过程中创建的无名临时对象，短暂性的。
左值和右值主要的区别之一是左值可以被修改，而右值不能。
```C++
int a; // a 为左值
a = 3; // 3 为右值
```
2. 左值引用和右值引用
左值引用：引用一个对象；
右值引用：就是必须绑定到右值的引用，C++11中右值引用可以实现“移动语义”，通过 && 获得右值引用。
```C++
int x = 6; // x是左值，6是右值
int &y = x; // 左值引用，y引用x

int &z1 = x * 6; // 错误，x*6是一个右值
const int &z2 =  x * 6; // 正确，可以将一个const引用绑定到一个右值

int &&z3 = x * 6; // 正确，右值引用
int &&z4 = x; // 错误，x是一个左值
```
右值引用和相关的移动语义是C++11标准中引入的最强大的特性之一，通过std::move()可以避免无谓的复制，提高程序性能。

### 随机数生成
#### rand()
1. 内部实现是用线性同余法做的，它不是真的随机数，因其周期特别长，故在一定的范围里可看成是随机的。
2. 0~RAND_MAX 每个数字被选中的机率是相同的
3. 用户未设定随机数种子时，系统默认的随机数种子为1
4. rand() 产生的是伪随机数字，每次执行时是相同的; 若要不同, 用函数srand()初始化它。
  
#### srand()
srand() 用来设置 rand() 产生随机数时的随机数种子。参数 seed 必须是个整数，如果每次 seed 都设相同值，rand() 所产生的随机数值每次就会一样。

#### 使用当前时钟作为随机数种子
```C++
        srand((unsigned)time(NULL)); 
        for(int i = 0; i < 10;i++ ) 
                cout << rand() << '\t';
        cout << endl; 
        return 0;
```

## exit code in c/c++
| 退出状态码 | 含义                                                                |
| ---------- | ------------------------------------------------------------------- |
| 0          | 命令成功退出                                                        |
| 0          | 在重定向或者单词展开期间(~、变量、命令、算术展开以及单词切割)失败。 |
| 1~125      | 命令不成功退出。特定的退出值的含义，有各个命令自行定义。            |
| 126        | 命令找到了，但是文件无法执行。                                      |
| 127        | 命令没有找到                                                        |
| 128        | 命令因收到信号而死亡。                                              |

[exit-codes-in-c-c-with-examples](https://www.geeksforgeeks.org/exit-codes-in-c-c-with-examples/)

[**python中的subprocess的returncode状态：**](https://docs.python.org/3/library/subprocess.html#subprocess.CompletedProcess.returncode)
- returncode = None 正在执行
- returncode = 0    正常退出
- returncode < 0    记录接收到的退出信号
- returncode > 0    程序执行异常
