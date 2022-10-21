cmake_minimum_required (VERSION 2.8)
project (extractKeyFrame C CXX)

find_package(OpenCV 4.0 REQUIRED)

add_executable(${PROJECT_NAME} extractKeyFrame.cpp)

# 添加依赖的库文件 
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})

# 设置C++编译标准  
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 11)