# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.17

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Disable VCS-based implicit rules.
% : %,v


# Disable VCS-based implicit rules.
% : RCS/%


# Disable VCS-based implicit rules.
% : RCS/%,v


# Disable VCS-based implicit rules.
% : SCCS/s.%


# Disable VCS-based implicit rules.
% : s.%


.SUFFIXES: .hpux_make_needs_suffix_list


# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

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
CMAKE_COMMAND = /usr/local/bin/cmake

# The command to remove a file.
RM = /usr/local/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /dominant/Project_Dominant/test/transform

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /dominant/Project_Dominant/test/transform/build

# Include any dependencies generated for this target.
include CMakeFiles/robust_curve_fit.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/robust_curve_fit.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/robust_curve_fit.dir/flags.make

CMakeFiles/robust_curve_fit.dir/robust_curve_fit.cpp.o: CMakeFiles/robust_curve_fit.dir/flags.make
CMakeFiles/robust_curve_fit.dir/robust_curve_fit.cpp.o: ../robust_curve_fit.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/dominant/Project_Dominant/test/transform/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/robust_curve_fit.dir/robust_curve_fit.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/robust_curve_fit.dir/robust_curve_fit.cpp.o -c /dominant/Project_Dominant/test/transform/robust_curve_fit.cpp

CMakeFiles/robust_curve_fit.dir/robust_curve_fit.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/robust_curve_fit.dir/robust_curve_fit.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /dominant/Project_Dominant/test/transform/robust_curve_fit.cpp > CMakeFiles/robust_curve_fit.dir/robust_curve_fit.cpp.i

CMakeFiles/robust_curve_fit.dir/robust_curve_fit.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/robust_curve_fit.dir/robust_curve_fit.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /dominant/Project_Dominant/test/transform/robust_curve_fit.cpp -o CMakeFiles/robust_curve_fit.dir/robust_curve_fit.cpp.s

# Object files for target robust_curve_fit
robust_curve_fit_OBJECTS = \
"CMakeFiles/robust_curve_fit.dir/robust_curve_fit.cpp.o"

# External object files for target robust_curve_fit
robust_curve_fit_EXTERNAL_OBJECTS =

robust_curve_fit: CMakeFiles/robust_curve_fit.dir/robust_curve_fit.cpp.o
robust_curve_fit: CMakeFiles/robust_curve_fit.dir/build.make
robust_curve_fit: /usr/local/lib/libceres.a
robust_curve_fit: /usr/local/lib/libglog.so.0.4.0
robust_curve_fit: /usr/local/lib/libgflags.so.2.2.0
robust_curve_fit: /usr/lib/x86_64-linux-gnu/libspqr.so
robust_curve_fit: /usr/lib/x86_64-linux-gnu/libtbb.so
robust_curve_fit: /usr/lib/x86_64-linux-gnu/libcholmod.so
robust_curve_fit: /usr/lib/x86_64-linux-gnu/libccolamd.so
robust_curve_fit: /usr/lib/x86_64-linux-gnu/libcamd.so
robust_curve_fit: /usr/lib/x86_64-linux-gnu/libcolamd.so
robust_curve_fit: /usr/lib/x86_64-linux-gnu/libamd.so
robust_curve_fit: /usr/lib/x86_64-linux-gnu/liblapack.so
robust_curve_fit: /usr/lib/x86_64-linux-gnu/libblas.so
robust_curve_fit: /usr/lib/x86_64-linux-gnu/libf77blas.so
robust_curve_fit: /usr/lib/x86_64-linux-gnu/libatlas.so
robust_curve_fit: /usr/lib/x86_64-linux-gnu/libsuitesparseconfig.so
robust_curve_fit: /usr/lib/x86_64-linux-gnu/librt.so
robust_curve_fit: /usr/lib/x86_64-linux-gnu/libcxsparse.so
robust_curve_fit: /usr/lib/x86_64-linux-gnu/liblapack.so
robust_curve_fit: /usr/lib/x86_64-linux-gnu/libblas.so
robust_curve_fit: /usr/lib/x86_64-linux-gnu/libf77blas.so
robust_curve_fit: /usr/lib/x86_64-linux-gnu/libatlas.so
robust_curve_fit: /usr/lib/x86_64-linux-gnu/libsuitesparseconfig.so
robust_curve_fit: /usr/lib/x86_64-linux-gnu/librt.so
robust_curve_fit: /usr/lib/x86_64-linux-gnu/libcxsparse.so
robust_curve_fit: CMakeFiles/robust_curve_fit.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/dominant/Project_Dominant/test/transform/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable robust_curve_fit"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/robust_curve_fit.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/robust_curve_fit.dir/build: robust_curve_fit

.PHONY : CMakeFiles/robust_curve_fit.dir/build

CMakeFiles/robust_curve_fit.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/robust_curve_fit.dir/cmake_clean.cmake
.PHONY : CMakeFiles/robust_curve_fit.dir/clean

CMakeFiles/robust_curve_fit.dir/depend:
	cd /dominant/Project_Dominant/test/transform/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /dominant/Project_Dominant/test/transform /dominant/Project_Dominant/test/transform /dominant/Project_Dominant/test/transform/build /dominant/Project_Dominant/test/transform/build /dominant/Project_Dominant/test/transform/build/CMakeFiles/robust_curve_fit.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/robust_curve_fit.dir/depend
