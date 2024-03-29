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


## NaN 和 INF
NaN: not a number，表示“无效数字”。
INF：infinite，表示“无穷大”。

[NaN和INF产生的原因]https://blog.csdn.net/wokaowokaowokao12345/article/details/72846436/

注意：
  NaN 会在执行一些非法数学运算后产生，例如：1/0、sqrt(-1)等。
  变量未初始化时，不会为 NaN，因为在栈内存在分配时会自动初始化，new 分配的也会初始化，malloc 分配的内存不会初始化，但是应该为随机值，而不应该为 NaN。
## 判断 NaN 和 INF

1. 使用表达式判断 （`nan！=nan`）为 True，即只有当f为 NaN 时 `f!=f`为 True ，但是表达式容易被编译器优化

- NaN 是无序的（unordered），无法对其进行逻辑运算。它不大于、小于或等于任何数（包括它自己），将<，>，<=，和>=作用于nan产生一个exception。得到nan时就查看是否有非法操作，如果表达式中含有nan，那么表达式的结果为nan。

- +inf大于任何数（除了它自己和nan），-inf小于任何数（除了它自己和nan），得到inf时就查看是否有溢出或者除以0。inf在C语言表达式中就表示数学里无限的概念，如1.0/inf等于0.0，并可以与其他浮点数进行比较的（可以参与<=、>+、==、!=等运算）。

1. 下面几个宏即包含在math.h头文件，可用于判断一个表达式的结果是否为inf、nan
```C++
int isfinite(x);
int isnormal(x);
int isnan(x);
int isinf(x);
```

## C++如何利用成员函数修改this指针的值
在使用boost进行save和load时，需要在load()获取内存对象地址后，再进行恢复。而load()函数必须使用实例对象调用，因此考虑传入，修改实例中的this指针，指向内存中的对对象，实现修改的目的。

[C++如何利用成员函数修改this指针的值](https://blog.csdn.net/qq_37232329/article/details/126520957)

## C/C++ 函数的调用约定
## 
- __cdecl C 调用
它是 C/C++函数默认的调用规范，C/C++运行时库中的函数基本都是__cdecl 调用。在该调用约定下，参数从右向左依次压入栈中，由主调函数负责清理参数的栈空间。该调用约定适用于支持可变参数的函数，因为只有主调函数才知道给该种函数传递了多少个参数，才知道应该清理多少栈空间。比如支持可变参数的 C 函数 printf：

-  __stdcall 标准调用
Windows 系统提供的系统 API 函数的调用约定

-  __stdcall 标准调用
该调用约定之所以被称作为快速调用，因为有部分参数可以通过寄存器直接传递，效率比较高。对于内存大小小于等于 4 字节的参数，直接使用 ECX 和 EDX 寄存器传递，剩余的参数则依次从右到左压入栈中通过栈传递，参数传递占用的栈空间由被调用函数清理。

[C/C++ 函数的调用约定详解](https://xie.infoq.cn/article/5fd35e4b7fc65b59185a8eabc)

[Linux和Win平台下函数调用参数传递约定详解](https://blog.csdn.net/weixin_46222091/article/details/114698551)

### 指定调用约定
Windows
```C++
int __stdcall func()
```

linux
```C++
int __attribute__((__stdcall__)) func()
```

[GCC attribute 扩展](http://gcc.gnu.org/onlinedocs/gcc-4.6.0/gcc/Function-Attributes.html#Function-Attributes)

[调用约定的通用参考](http://en.wikipedia.org/wiki/X86_calling_conventions)

## 构造函数的初始化列表
1. 初始化顺序和初始化列表列出的顺序无关，只与它在类中声明顺序有关
2. 对象的成员变量的初始化动作发生在进入构造函数本体之前
3. 构造函数中手动赋值需要调用默认构造函数和拷贝构造函数，初始化列表只调用默认构造函数，效率更高

### 初始化列表的使用情况
1. const 成员
2. 引用成员
3. 类中包含另外一个类作为成员变量，且被包含的类有有参构造函数，没有无参构造函数（编译器无法调用默认构造函数进行初始化）
4. 两个类有继关系,父类没有显示声明无参构造数,但声明了有参数构函数（无参的默认构造函数被覆盖）

### 默认够找函数与自定义构造函数
1. 默认构造函数可以被编译器隐式调用，而自定义构造函数需要用户显示调用。这些情况包括：
  - 在子类的构造函数中，显示调用基类的自定义构造函数
  - 在子类的复制构造函数中，调用成员变量中包含类的自定义构造函数（如果有显示定义）
  - 子类析构函数中显示调用基类的析构函数（如果有显示定义）
  - C++11 移动函数 ...
### 

[C++中必须用参数列表初始化的4种情况](https://blog.csdn.net/weixin_42769044/article/details/103348669)


## string
   C++的string是对C字符串的类封装，它们都占用连续的内存（对于存储数据而言）。
### string 的内存占用
1. 字符串的长度小于24字节时分配在栈空间，大于24字节时分配在堆空间，并在栈中记录堆中的地址。
2. 由于字符串的内存是连续的，因此字符串每次相加时会重新申请内存。
3. +=、append、stringstream、sprintf四种方式在消耗的时间大致为1：1：4：2(见参考）

[C/C++指针与字符串](https://www.jianshu.com/p/6469fad4d5d5)
[C++ 中 std::string 的内存布局](https://zhuanlan.zhihu.com/p/510507837)
[C++的string-两手抓的内存分配](https://zhuanlan.zhihu.com/p/187499607)
[C++字符串拼接效率比较(+=、append、stringstream、spintf)](https://blog.csdn.net/mijichui2153/article/details/118154341)