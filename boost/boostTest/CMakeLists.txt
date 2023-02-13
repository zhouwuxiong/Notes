cmake_minimum_required(VERSION 3.14)
project(my_project)

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 11)

find_package(Boost       # Minimum or EXACT version e.g. 1.36.0
        REQUIRED             					# Fail with error if Boost is not found
        COMPONENTS serialization 				# Boost libraries by their canonical name
        )

enable_testing()

add_executable(
  boostMemoryTest
  boostMemoryTest.cpp
)
target_link_libraries(
	boostMemoryTest
	${Boost_LIBRARIES}
)


add_executable(
  boostLoadPoint
  boostLoadPoint.cpp
)

target_link_libraries(
	boostLoadPoint
	${Boost_LIBRARIES}
)


add_executable(
  boostLoadInstance
  boostLoadInstance.cpp
)
target_link_libraries(
	boostLoadInstance
	${Boost_LIBRARIES}
)
