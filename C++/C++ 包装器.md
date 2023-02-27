## C++ 包装器
包装器用于给其他编程接口提供更一致或更合适的接口。
- 模板bind可替代bind1st和bind2nd，且更灵活
- 模板mem_fn让您能够将成员函数作为常规函数进行传递
- 模板reference_wrapper让您能够创建像引用但可被复制的对象
- 包装器function让您能够以统一的方式处理多种类似于函数的形式

模板function是在头文件functional头文件中声明的，他从参数和返回值的角度定义了一个对象，可用于包装调用参数和返回值相同的函数指针、伪函数或lambda表达式

### 模板函数
- 在编译时，模板函数会根据不同的参数类型，实例化多份代码。模板类也会实例化多个具体的类型。

### function包装器
- 由于函数调用可以使用函数名、函数指针、函数对象或有名称的lambda表达式，可调用类型太丰富导致模板的效率极低。包装器用于解决效率低的问题。
- 
- 将可调用对象的类型进行统一，便于我们对其进行统一化管理。
- 包装后明确了可调用对象的返回值和形参类型，更加方便使用者使用。

1. 在传入第二个参数类型相同的情况下，如果传入的可调用对象的类型是不同的，那么在编译阶段该函数模板就会被实例化多次。例如：
```C++
template<class F, class T>
T useF(F f, T x)
{
	static int count = 0;
	cout << "count: " << ++count << endl;
	cout << "count: " << &count << endl;

	return f(x);
}

double f(double i)
{
	return i / 2;
}
struct Functor
{
	double operator()(double d)
	{
		return d / 3;
	}
};
int main()
{
  // =======  使用函数对象模板进行实例化时，由于传人的函数类型不同，USF函数会被实例化多份，（函数实例化的地址不同）
	//函数指针
	cout << useF(f, 11.11) << endl;
	//仿函数
	cout << useF(Functor(), 11.11) << endl;
	//lambda表达式
	cout << useF([](double d)->double{return d / 4; }, 11.11) << endl;

  // =========== 使用函数包装器，由于指定了参数和返回值的类型，只会实例化 useF 函数
	//函数名
	function<double(double)> func1 = func;
	cout << useF(func1, 11.11) << endl;
	//函数对象
	function<double(double)> func2 = Functor();
	cout << useF(func2, 11.11) << endl;
	//lambda表达式
	function<double(double)> func3 = [](double d)->double{return d / 4; };
	cout << useF(func3, 11.11) << endl;

	return 0;
}
```

### bind包装器
bind也是一种函数包装器，也叫做适配器。它可以接受一个可调用对象，生成一个新的可调用对象来“适应”原对象的参数列表。

- 将一个函数的某些参数绑定为固定的值，让我们在调用时可以不用传递某些参数。
- 可以对函数参数的顺序进行灵活调整。

1. 绑定固定参数
```C+++
int Plus(int a, int b)
{
	return a + b;
}
int main()
{
	// 1. 无意义的绑定, placeholders::_n 为参数占位符，表示将newCallable的第一个参数
	function<int(int, int)> func = bind(Plus, placeholders::_1, placeholders::_2);
	cout << func(1, 2) << endl; //3

	// 2. 绑定固定参数
	function<int(int)> func = bind(Plus, placeholders::_1, 10);
	cout << func(1, 2) << endl; //-1


	return 0;
}
```
2. bind 调整传参顺序
sub成员函数的第一个参数是隐藏的this指针，如果想要在调用sub成员函数时不用对象进行调用，那么可以将sub成员函数的第一个参数固定绑定为一个Sub对象。例如：
```C++
class Sub
{
public:
	int sub(int a, int b)
	{
		return a - b;
	}
};
int main()
{
	//在调用时会自动传入一个匿名对象给this指针
	function<int(int, int)> func = bind(&Sub::sub, Sub(), placeholders::_1, placeholders::_2);
	cout << func(1, 2) << endl; //1

  	//调整传参顺序
	function<int(int, int)> func = bind(&Sub::sub, Sub(), placeholders::_2, placeholders::_1);
  	cout << func(1, 2) << endl; //1

	return 0;
}
```
 