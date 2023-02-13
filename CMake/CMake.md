
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

## CMake引入外部项目
### git下的Submodule
```shell
git submodule add https://gitee.com/mohistH/fmt.git
```
[submodule官方文档](https://git-scm.com/book/en/v2/Git-Tools-Submodules)

### cmake 的 FetchContent
cmake 在build时会调用 ExternalProject_Add() 取下载 FetchContent 模块

```text
# 1.include(FetchContent) 
# 2.FetchContent_Declare(子模块名) 获取项目。
# 3.FetchContent_MakeAvailable(子模块)，再引入我们的项目中
# 4.target_link_libraries(主项目 PRIVATE 子模块::子模块)
```
### 例1： gtest
```text
# gtest
include(FetchContent)

#- 其中 url 中的 hashcode 需要在 git 的 commit 中寻找
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/0296d7d37007ec54441cb9f8cd001da6ac58eea8.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)
```
### 例2: cmake demo
```text
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        703bd9caab50b139428cea1aaff9974ebee5742e # release-1.10.0
)
FetchContent_Declare(
  myCompanyIcons
  URL      https://intranet.mycompany.com/assets/iconset_1.12.tar.gz
  URL_HASH MD5=5588a7b18261c20068beabfb4f530b87
)
FetchContent_MakeAvailable(googletest myCompanyIcons)
```


参考：
https://www.cnblogs.com/lidabo/p/16661086.html