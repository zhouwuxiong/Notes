cmake_minimum_required(VERSION 3.17.1)
project(Tutorial)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)


project(Tutorial VERSION 1.0)



find_package(Eigen3 REQUIRED)
find_package(Ceres REQUIRED)


include_directories(
	${EIGEN3_INCLUDE_DIR})

add_executable(Tutorial posTransform.cpp)
# target_include_directories(Tutorial PUBLIC"${PROJECT_BINARY_DIR}")

add_executable(ceresDemo ceres.cpp)
target_link_libraries(ceresDemo ceres)

add_executable(powell powell.cpp)
target_link_libraries(powell ceres)

add_executable(curve_fitting curve_fitting.cpp)
target_link_libraries(curve_fitting ceres)

add_executable(robust_curve_fit robust_curve_fit.cpp)
target_link_libraries(robust_curve_fit ceres)

add_executable(simple_bundle_adjuster simple_bundle_adjuster.cpp)
target_link_libraries(simple_bundle_adjuster ceres)
