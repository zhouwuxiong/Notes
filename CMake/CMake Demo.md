# CMake 最低版本号要求
cmake_minimum_required (VERSION 2.8)
# https://zhuanlan.zhihu.com/p/410219342
# 项目信息
project (Demo3 C CXX)


###   1. 编译设置 #####
# 1.1编译选择设置
set(VERSION_MAJOR 1)
set(VERSION_MINOR 0)
set(VERSION_PATCH 0)
set(VERSION_STRING ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH})

SET(CMAKE_BUILD_TYPE "release")  

option(BUILD_GDB   "Build DBoW2"            OFF)
option(BUILD_Demo   "Build DBoW2"           ON)

# 1.2 通用编译设置
set(CMAKE_CXX_COMPILER      "clang++" )         # 显示指定使用的C++编译器
set(CMAKE_CXX_FLAGS   "-std=c++11")             # c++11
set(CMAKE_CXX_FLAGS   "-g")                     # 调试信息
set(CMAKE_CXX_FLAGS   "-Wall")                  # 开启所有警告

set(CMAKE_CXX_FLAGS_DEBUG   "-O0" )             # 调试包不优化
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG " )   # release包优化

# 1.3 路径设置
#执行文件的输出目录
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
#库文件的输出目录
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)


########## 2. 添加资源文件 ####################
# 查找src目录下的所有源文件
# 并将名称保存到 DIR_SRCS 变量
aux_source_directory(src DIR_SRCS)
aux_source_directory(include DIR_HDRS)

# 添加 math 子目录
add_subdirectory(math)

########### 3. 查找依赖包 ####################
# 第三方库的安装路径，在不同的机器上可能不同
# find_package会查找${CMAKE_MODULE_PATH}中的所有目录和自己的模块目录"/share/cmake-x.y/Modules/"中，查找Findxxx.cmake文件和xxxConfig.cmake文件，这两个文件中记录了库的路径和头文件信息，一般由库的作者提供
# 如果找到了会返回以下变量:
#   xxxx_FOUND              #为true
#   xxxx_INCLUDE_DIRS       #include路径
#   xxxx_LIBRARY_DIRS       #library路径
#   xxxx_LIBRARIES          #library的名字
#   xxxx_yyyy_VERSION       #具体详见Findxxxx.cmake
# 3.1 查找库文件
find_package(OpenCV REQUIRED)

# 3.2 如果find_package失败，添加自定义的函数库
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake;${CMAKE_MODULE_PATH}")
add_executable(addtest addtest.cc)
find_package(ADD)
if(ADD_FOUND)
    target_include_directories(addtest PRIVATE ${ADD_INCLUDE_DIR})
    target_link_libraries(addtest ${ADD_LIBRARY})
else(ADD_FOUND)
    message(FATAL_ERROR "ADD library not found")
endif(ADD_FOUND)d

# 3.3 如果find_package失败，在指定目录下寻找头文件和动态库文件的位置，可以指定多个目标路径
find_path(ADD_INCLUDE_DIR libadd.h /usr/include/ /usr/local/include ${CMAKE_SOURCE_DIR}/ModuleMode)
find_library(ADD_LIBRARY NAMES add PATHS /usr/lib/add /usr/local/lib/add ${CMAKE_SOURCE_DIR}/ModuleMode)
if (ADD_INCLUDE_DIR AND ADD_LIBRARY)
    set(ADD_FOUND TRUE)
endif (ADD_INCLUDE_DIR AND ADD_LIBRARY)


### build Demo
if(BUILD_Demo)
    # 指定生成目标(可执行文件) ,在这里添加要使用的所有的.cpp文件
    add_executable(Demo main.cc ${DIR_SRCS})

    # 添加头文件
    target_include_directories(train PUBLIC $(DIR_HDRS))
    #target_include_directories(train PUBLIC include/DBoW2/ include/)

    # 添加依赖的库文件 
    target_link_libraries(train ${OpenCV_LIBS} gflags)

    # 设置C++编译标准  
    set_target_properties(train PROPERTIES CXX_STANDARD 11)
endif(BUILD_Demo)

#### gdb调试
if(BUILD_GDB)
    SET(CMAKE_BUILD_TYPE "Debug")
    SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb")
    SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")
endif(BUILD_GDB)


## 
务必LINK_DIRECTORIES 要放在ADD_EXECUTABLE() or ADD_LIBRARY()