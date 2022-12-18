## 打印函数调用栈
### 适用条件
1. 当某个函数被多个函数调用，
2. 项目中存在很多重名函数时
3. 没有 GDB 环境
4. 代码逻辑错误，难以定位
定位函数的调用者

### 
```C++
#include <execinfo.h>
// 获取当前的调用栈信息，结果存储在buffer中，返回值为栈的深度，参数size限制栈的最大深度，即最大取size步的栈信息。
int backtrace(void **buffer, int size);

// 把backtrace获取的栈信息转化为字符串，以字符指针数组的形式返回，参数size限定转换的深度，一般用backtrace调用的返回值。
char **backtrace_symbols(void *const *buffer, int size);

// 它的功能和backtrace_symbols差不多，只不过它不把转换结果返回给调用方，而是写入fd指定的文件描述符。
void backtrace_symbols_fd(void *const *buffer, int size, int fd);
```

### Demo
```C++
#include<execinfo.h>

void print_stacktrace()
{
    int size = 16;
    void * array[16];
    int stack_num = backtrace(array, size);
    char ** stacktrace = backtrace_symbols(array, stack_num);
    for (int i = 0; i < stack_num; ++i)
    {
        printf("%s\n", stacktrace[i]);
    }
    free(stacktrace);
}
```


[在C/C++程序中打印当前函数调用栈](https://blog.csdn.net/rheostat/article/details/8523598)
[在C/C++程序里打印调用栈信息](https://blog.csdn.net/rheostat/article/details/8523598)