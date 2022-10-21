
[cmake官方教程中文翻译](https://blog.51cto.com/u_12951861/5695609)

1. find_package
查找路径依次为：
1. `CMake变量${CMAKE_MODULE_PATH}`中的所有目录
2. cmake自己的模块目录 `/share/cmake-x.y/Modules/`(`$CMAKE_ROOT`的具体值可以通过CMake中message命令输出）。如果找到了xxxx模块，那么Findxxxx.cmake一般会设置以下变量供CMakeLists.txt使用：

```text
xxxx_FOUND    #为true
xxxx_INCLUDE_DIRS   #include路径
xxxx_LIBRARY_DIRS    #library路径
xxxx_LIBRARIES           #library的名字
xxxx_yyyy_VERSION    #具体详见Findxxxx.cmake
```

参考：
https://www.cnblogs.com/lidabo/p/16661086.html