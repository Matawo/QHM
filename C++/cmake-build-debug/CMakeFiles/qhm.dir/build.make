# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.13

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
CMAKE_COMMAND = /home/matawo/clion-2018.3.4/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /home/matawo/clion-2018.3.4/bin/cmake/linux/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/matawo/CLionProjects/QHM/C++

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/matawo/CLionProjects/QHM/C++/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/qhm.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/qhm.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/qhm.dir/flags.make

CMakeFiles/qhm.dir/main.cpp.o: CMakeFiles/qhm.dir/flags.make
CMakeFiles/qhm.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/matawo/CLionProjects/QHM/C++/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/qhm.dir/main.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/qhm.dir/main.cpp.o -c /home/matawo/CLionProjects/QHM/C++/main.cpp

CMakeFiles/qhm.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/qhm.dir/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/matawo/CLionProjects/QHM/C++/main.cpp > CMakeFiles/qhm.dir/main.cpp.i

CMakeFiles/qhm.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/qhm.dir/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/matawo/CLionProjects/QHM/C++/main.cpp -o CMakeFiles/qhm.dir/main.cpp.s

CMakeFiles/qhm.dir/Graph.cpp.o: CMakeFiles/qhm.dir/flags.make
CMakeFiles/qhm.dir/Graph.cpp.o: ../Graph.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/matawo/CLionProjects/QHM/C++/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/qhm.dir/Graph.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/qhm.dir/Graph.cpp.o -c /home/matawo/CLionProjects/QHM/C++/Graph.cpp

CMakeFiles/qhm.dir/Graph.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/qhm.dir/Graph.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/matawo/CLionProjects/QHM/C++/Graph.cpp > CMakeFiles/qhm.dir/Graph.cpp.i

CMakeFiles/qhm.dir/Graph.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/qhm.dir/Graph.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/matawo/CLionProjects/QHM/C++/Graph.cpp -o CMakeFiles/qhm.dir/Graph.cpp.s

# Object files for target qhm
qhm_OBJECTS = \
"CMakeFiles/qhm.dir/main.cpp.o" \
"CMakeFiles/qhm.dir/Graph.cpp.o"

# External object files for target qhm
qhm_EXTERNAL_OBJECTS =

qhm: CMakeFiles/qhm.dir/main.cpp.o
qhm: CMakeFiles/qhm.dir/Graph.cpp.o
qhm: CMakeFiles/qhm.dir/build.make
qhm: CMakeFiles/qhm.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/matawo/CLionProjects/QHM/C++/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable qhm"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/qhm.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/qhm.dir/build: qhm

.PHONY : CMakeFiles/qhm.dir/build

CMakeFiles/qhm.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/qhm.dir/cmake_clean.cmake
.PHONY : CMakeFiles/qhm.dir/clean

CMakeFiles/qhm.dir/depend:
	cd /home/matawo/CLionProjects/QHM/C++/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/matawo/CLionProjects/QHM/C++ /home/matawo/CLionProjects/QHM/C++ /home/matawo/CLionProjects/QHM/C++/cmake-build-debug /home/matawo/CLionProjects/QHM/C++/cmake-build-debug /home/matawo/CLionProjects/QHM/C++/cmake-build-debug/CMakeFiles/qhm.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/qhm.dir/depend

