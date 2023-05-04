
[cmake官方教程中文翻译](https://blog.51cto.com/u_12951861/5695609)

## find_package
### Module模式(仅仅查找Findxxx.cmake文件)
为了方便我们在项目中引入外部依赖包，cmake官方为我们预定义了许多寻找依赖包的Module，他们存储在path_to_your_cmake/share/cmake-<version>/Modules目录下。可以在官方文档中查看到哪些库官方已经为我们定义好了，我们可以直接使用find_package函数进行引用[官方文档：Find Modules](https://link.zhihu.com/?target=https%3A//cmake.org/cmake/help/latest/manual/cmake-modules.7.html)。

### Config模式（仅仅查找xxxConfig.cmake文件）
*如果Module模式搜索失败，没有找到对应的Find<LibraryName>.cmake文件，则转入Config模式进行搜索。*,它主要通过 `/usr/local/lib/cmake/glog/`文件夹下的 <LibraryName>Config.cmake 来引入我们需要的库. find_package 的参考路径可以参考[官方文档:find_package](https://link.zhihu.com/?target=https%3A//cmake.org/cmake/help/latest/command/find_package.html)

## FetchContent
```txt
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/0296d7d37007ec54441cb9f8cd001da6ac58eea8.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
```

## 
查找路径依次为：
1. `CMake变量${CMAKE_MODULE_PATH}`中的所有目录
2. cmake自己的模块目录 `<CMAKE_ROOT>/share/cmake-x.y/Modules/`(`$CMAKE_ROOT`的具体值可以通过CMake中message命令输出）。如果找到了xxxx模块，那么Findxxxx.cmake一般会设置以下变量供CMakeLists.txt使用：

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