# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/zwx/Notes/boost/boostTest

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/zwx/Notes/boost/boostTest/build

# Include any dependencies generated for this target.
include CMakeFiles/boostLoadPoint.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/boostLoadPoint.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/boostLoadPoint.dir/flags.make

CMakeFiles/boostLoadPoint.dir/boostLoadPoint.cpp.o: CMakeFiles/boostLoadPoint.dir/flags.make
CMakeFiles/boostLoadPoint.dir/boostLoadPoint.cpp.o: ../boostLoadPoint.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/zwx/Notes/boost/boostTest/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/boostLoadPoint.dir/boostLoadPoint.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/boostLoadPoint.dir/boostLoadPoint.cpp.o -c /home/zwx/Notes/boost/boostTest/boostLoadPoint.cpp

CMakeFiles/boostLoadPoint.dir/boostLoadPoint.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/boostLoadPoint.dir/boostLoadPoint.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/zwx/Notes/boost/boostTest/boostLoadPoint.cpp > CMakeFiles/boostLoadPoint.dir/boostLoadPoint.cpp.i

CMakeFiles/boostLoadPoint.dir/boostLoadPoint.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/boostLoadPoint.dir/boostLoadPoint.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/zwx/Notes/boost/boostTest/boostLoadPoint.cpp -o CMakeFiles/boostLoadPoint.dir/boostLoadPoint.cpp.s

# Object files for target boostLoadPoint
boostLoadPoint_OBJECTS = \
"CMakeFiles/boostLoadPoint.dir/boostLoadPoint.cpp.o"

# External object files for target boostLoadPoint
boostLoadPoint_EXTERNAL_OBJECTS =

boostLoadPoint: CMakeFiles/boostLoadPoint.dir/boostLoadPoint.cpp.o
boostLoadPoint: CMakeFiles/boostLoadPoint.dir/build.make
boostLoadPoint: /usr/lib/x86_64-linux-gnu/libboost_serialization.so.1.71.0
boostLoadPoint: CMakeFiles/boostLoadPoint.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/zwx/Notes/boost/boostTest/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable boostLoadPoint"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/boostLoadPoint.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/boostLoadPoint.dir/build: boostLoadPoint

.PHONY : CMakeFiles/boostLoadPoint.dir/build

CMakeFiles/boostLoadPoint.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/boostLoadPoint.dir/cmake_clean.cmake
.PHONY : CMakeFiles/boostLoadPoint.dir/clean

CMakeFiles/boostLoadPoint.dir/depend:
	cd /home/zwx/Notes/boost/boostTest/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/zwx/Notes/boost/boostTest /home/zwx/Notes/boost/boostTest /home/zwx/Notes/boost/boostTest/build /home/zwx/Notes/boost/boostTest/build /home/zwx/Notes/boost/boostTest/build/CMakeFiles/boostLoadPoint.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/boostLoadPoint.dir/depend

