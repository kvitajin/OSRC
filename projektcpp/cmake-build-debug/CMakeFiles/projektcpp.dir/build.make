# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.18

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

#Suppress display of executed commands.
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
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /tmp/tmp.93UDAros2A

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /tmp/tmp.93UDAros2A/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/projektcpp.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/projektcpp.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/projektcpp.dir/flags.make

CMakeFiles/projektcpp.dir/main.cpp.o: CMakeFiles/projektcpp.dir/flags.make
CMakeFiles/projektcpp.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/tmp/tmp.93UDAros2A/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/projektcpp.dir/main.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/projektcpp.dir/main.cpp.o -c /tmp/tmp.93UDAros2A/main.cpp

CMakeFiles/projektcpp.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/projektcpp.dir/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /tmp/tmp.93UDAros2A/main.cpp > CMakeFiles/projektcpp.dir/main.cpp.i

CMakeFiles/projektcpp.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/projektcpp.dir/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /tmp/tmp.93UDAros2A/main.cpp -o CMakeFiles/projektcpp.dir/main.cpp.s

# Object files for target projektcpp
projektcpp_OBJECTS = \
"CMakeFiles/projektcpp.dir/main.cpp.o"

# External object files for target projektcpp
projektcpp_EXTERNAL_OBJECTS =

projektcpp: CMakeFiles/projektcpp.dir/main.cpp.o
projektcpp: CMakeFiles/projektcpp.dir/build.make
projektcpp: CMakeFiles/projektcpp.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/tmp/tmp.93UDAros2A/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable projektcpp"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/projektcpp.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/projektcpp.dir/build: projektcpp

.PHONY : CMakeFiles/projektcpp.dir/build

CMakeFiles/projektcpp.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/projektcpp.dir/cmake_clean.cmake
.PHONY : CMakeFiles/projektcpp.dir/clean

CMakeFiles/projektcpp.dir/depend:
	cd /tmp/tmp.93UDAros2A/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /tmp/tmp.93UDAros2A /tmp/tmp.93UDAros2A /tmp/tmp.93UDAros2A/cmake-build-debug /tmp/tmp.93UDAros2A/cmake-build-debug /tmp/tmp.93UDAros2A/cmake-build-debug/CMakeFiles/projektcpp.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/projektcpp.dir/depend
