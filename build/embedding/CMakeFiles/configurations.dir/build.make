# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.5

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
CMAKE_SOURCE_DIR = /home/stef/Documents/CollectiveLevyWalk

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/stef/Documents/CollectiveLevyWalk/build

# Include any dependencies generated for this target.
include embedding/CMakeFiles/configurations.dir/depend.make

# Include the progress variables for this target.
include embedding/CMakeFiles/configurations.dir/progress.make

# Include the compile flags for this target's objects.
include embedding/CMakeFiles/configurations.dir/flags.make

embedding/CMakeFiles/configurations.dir/configurations.cpp.o: embedding/CMakeFiles/configurations.dir/flags.make
embedding/CMakeFiles/configurations.dir/configurations.cpp.o: ../embedding/configurations.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/stef/Documents/CollectiveLevyWalk/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object embedding/CMakeFiles/configurations.dir/configurations.cpp.o"
	cd /home/stef/Documents/CollectiveLevyWalk/build/embedding && /usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/configurations.dir/configurations.cpp.o -c /home/stef/Documents/CollectiveLevyWalk/embedding/configurations.cpp

embedding/CMakeFiles/configurations.dir/configurations.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/configurations.dir/configurations.cpp.i"
	cd /home/stef/Documents/CollectiveLevyWalk/build/embedding && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/stef/Documents/CollectiveLevyWalk/embedding/configurations.cpp > CMakeFiles/configurations.dir/configurations.cpp.i

embedding/CMakeFiles/configurations.dir/configurations.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/configurations.dir/configurations.cpp.s"
	cd /home/stef/Documents/CollectiveLevyWalk/build/embedding && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/stef/Documents/CollectiveLevyWalk/embedding/configurations.cpp -o CMakeFiles/configurations.dir/configurations.cpp.s

embedding/CMakeFiles/configurations.dir/configurations.cpp.o.requires:

.PHONY : embedding/CMakeFiles/configurations.dir/configurations.cpp.o.requires

embedding/CMakeFiles/configurations.dir/configurations.cpp.o.provides: embedding/CMakeFiles/configurations.dir/configurations.cpp.o.requires
	$(MAKE) -f embedding/CMakeFiles/configurations.dir/build.make embedding/CMakeFiles/configurations.dir/configurations.cpp.o.provides.build
.PHONY : embedding/CMakeFiles/configurations.dir/configurations.cpp.o.provides

embedding/CMakeFiles/configurations.dir/configurations.cpp.o.provides.build: embedding/CMakeFiles/configurations.dir/configurations.cpp.o


embedding/CMakeFiles/configurations.dir/configurations_automoc.cpp.o: embedding/CMakeFiles/configurations.dir/flags.make
embedding/CMakeFiles/configurations.dir/configurations_automoc.cpp.o: embedding/configurations_automoc.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/stef/Documents/CollectiveLevyWalk/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object embedding/CMakeFiles/configurations.dir/configurations_automoc.cpp.o"
	cd /home/stef/Documents/CollectiveLevyWalk/build/embedding && /usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/configurations.dir/configurations_automoc.cpp.o -c /home/stef/Documents/CollectiveLevyWalk/build/embedding/configurations_automoc.cpp

embedding/CMakeFiles/configurations.dir/configurations_automoc.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/configurations.dir/configurations_automoc.cpp.i"
	cd /home/stef/Documents/CollectiveLevyWalk/build/embedding && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/stef/Documents/CollectiveLevyWalk/build/embedding/configurations_automoc.cpp > CMakeFiles/configurations.dir/configurations_automoc.cpp.i

embedding/CMakeFiles/configurations.dir/configurations_automoc.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/configurations.dir/configurations_automoc.cpp.s"
	cd /home/stef/Documents/CollectiveLevyWalk/build/embedding && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/stef/Documents/CollectiveLevyWalk/build/embedding/configurations_automoc.cpp -o CMakeFiles/configurations.dir/configurations_automoc.cpp.s

embedding/CMakeFiles/configurations.dir/configurations_automoc.cpp.o.requires:

.PHONY : embedding/CMakeFiles/configurations.dir/configurations_automoc.cpp.o.requires

embedding/CMakeFiles/configurations.dir/configurations_automoc.cpp.o.provides: embedding/CMakeFiles/configurations.dir/configurations_automoc.cpp.o.requires
	$(MAKE) -f embedding/CMakeFiles/configurations.dir/build.make embedding/CMakeFiles/configurations.dir/configurations_automoc.cpp.o.provides.build
.PHONY : embedding/CMakeFiles/configurations.dir/configurations_automoc.cpp.o.provides

embedding/CMakeFiles/configurations.dir/configurations_automoc.cpp.o.provides.build: embedding/CMakeFiles/configurations.dir/configurations_automoc.cpp.o


# Object files for target configurations
configurations_OBJECTS = \
"CMakeFiles/configurations.dir/configurations.cpp.o" \
"CMakeFiles/configurations.dir/configurations_automoc.cpp.o"

# External object files for target configurations
configurations_EXTERNAL_OBJECTS =

embedding/configurations: embedding/CMakeFiles/configurations.dir/configurations.cpp.o
embedding/configurations: embedding/CMakeFiles/configurations.dir/configurations_automoc.cpp.o
embedding/configurations: embedding/CMakeFiles/configurations.dir/build.make
embedding/configurations: embedding/CMakeFiles/configurations.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/stef/Documents/CollectiveLevyWalk/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable configurations"
	cd /home/stef/Documents/CollectiveLevyWalk/build/embedding && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/configurations.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
embedding/CMakeFiles/configurations.dir/build: embedding/configurations

.PHONY : embedding/CMakeFiles/configurations.dir/build

embedding/CMakeFiles/configurations.dir/requires: embedding/CMakeFiles/configurations.dir/configurations.cpp.o.requires
embedding/CMakeFiles/configurations.dir/requires: embedding/CMakeFiles/configurations.dir/configurations_automoc.cpp.o.requires

.PHONY : embedding/CMakeFiles/configurations.dir/requires

embedding/CMakeFiles/configurations.dir/clean:
	cd /home/stef/Documents/CollectiveLevyWalk/build/embedding && $(CMAKE_COMMAND) -P CMakeFiles/configurations.dir/cmake_clean.cmake
.PHONY : embedding/CMakeFiles/configurations.dir/clean

embedding/CMakeFiles/configurations.dir/depend:
	cd /home/stef/Documents/CollectiveLevyWalk/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/stef/Documents/CollectiveLevyWalk /home/stef/Documents/CollectiveLevyWalk/embedding /home/stef/Documents/CollectiveLevyWalk/build /home/stef/Documents/CollectiveLevyWalk/build/embedding /home/stef/Documents/CollectiveLevyWalk/build/embedding/CMakeFiles/configurations.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : embedding/CMakeFiles/configurations.dir/depend

