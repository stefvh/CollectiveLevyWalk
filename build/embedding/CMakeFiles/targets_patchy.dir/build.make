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
include embedding/CMakeFiles/targets_patchy.dir/depend.make

# Include the progress variables for this target.
include embedding/CMakeFiles/targets_patchy.dir/progress.make

# Include the compile flags for this target's objects.
include embedding/CMakeFiles/targets_patchy.dir/flags.make

embedding/CMakeFiles/targets_patchy.dir/targets_patchy.cpp.o: embedding/CMakeFiles/targets_patchy.dir/flags.make
embedding/CMakeFiles/targets_patchy.dir/targets_patchy.cpp.o: ../embedding/targets_patchy.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/stef/Documents/CollectiveLevyWalk/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object embedding/CMakeFiles/targets_patchy.dir/targets_patchy.cpp.o"
	cd /home/stef/Documents/CollectiveLevyWalk/build/embedding && /usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/targets_patchy.dir/targets_patchy.cpp.o -c /home/stef/Documents/CollectiveLevyWalk/embedding/targets_patchy.cpp

embedding/CMakeFiles/targets_patchy.dir/targets_patchy.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/targets_patchy.dir/targets_patchy.cpp.i"
	cd /home/stef/Documents/CollectiveLevyWalk/build/embedding && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/stef/Documents/CollectiveLevyWalk/embedding/targets_patchy.cpp > CMakeFiles/targets_patchy.dir/targets_patchy.cpp.i

embedding/CMakeFiles/targets_patchy.dir/targets_patchy.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/targets_patchy.dir/targets_patchy.cpp.s"
	cd /home/stef/Documents/CollectiveLevyWalk/build/embedding && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/stef/Documents/CollectiveLevyWalk/embedding/targets_patchy.cpp -o CMakeFiles/targets_patchy.dir/targets_patchy.cpp.s

embedding/CMakeFiles/targets_patchy.dir/targets_patchy.cpp.o.requires:

.PHONY : embedding/CMakeFiles/targets_patchy.dir/targets_patchy.cpp.o.requires

embedding/CMakeFiles/targets_patchy.dir/targets_patchy.cpp.o.provides: embedding/CMakeFiles/targets_patchy.dir/targets_patchy.cpp.o.requires
	$(MAKE) -f embedding/CMakeFiles/targets_patchy.dir/build.make embedding/CMakeFiles/targets_patchy.dir/targets_patchy.cpp.o.provides.build
.PHONY : embedding/CMakeFiles/targets_patchy.dir/targets_patchy.cpp.o.provides

embedding/CMakeFiles/targets_patchy.dir/targets_patchy.cpp.o.provides.build: embedding/CMakeFiles/targets_patchy.dir/targets_patchy.cpp.o


embedding/CMakeFiles/targets_patchy.dir/targets_patchy_automoc.cpp.o: embedding/CMakeFiles/targets_patchy.dir/flags.make
embedding/CMakeFiles/targets_patchy.dir/targets_patchy_automoc.cpp.o: embedding/targets_patchy_automoc.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/stef/Documents/CollectiveLevyWalk/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object embedding/CMakeFiles/targets_patchy.dir/targets_patchy_automoc.cpp.o"
	cd /home/stef/Documents/CollectiveLevyWalk/build/embedding && /usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/targets_patchy.dir/targets_patchy_automoc.cpp.o -c /home/stef/Documents/CollectiveLevyWalk/build/embedding/targets_patchy_automoc.cpp

embedding/CMakeFiles/targets_patchy.dir/targets_patchy_automoc.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/targets_patchy.dir/targets_patchy_automoc.cpp.i"
	cd /home/stef/Documents/CollectiveLevyWalk/build/embedding && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/stef/Documents/CollectiveLevyWalk/build/embedding/targets_patchy_automoc.cpp > CMakeFiles/targets_patchy.dir/targets_patchy_automoc.cpp.i

embedding/CMakeFiles/targets_patchy.dir/targets_patchy_automoc.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/targets_patchy.dir/targets_patchy_automoc.cpp.s"
	cd /home/stef/Documents/CollectiveLevyWalk/build/embedding && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/stef/Documents/CollectiveLevyWalk/build/embedding/targets_patchy_automoc.cpp -o CMakeFiles/targets_patchy.dir/targets_patchy_automoc.cpp.s

embedding/CMakeFiles/targets_patchy.dir/targets_patchy_automoc.cpp.o.requires:

.PHONY : embedding/CMakeFiles/targets_patchy.dir/targets_patchy_automoc.cpp.o.requires

embedding/CMakeFiles/targets_patchy.dir/targets_patchy_automoc.cpp.o.provides: embedding/CMakeFiles/targets_patchy.dir/targets_patchy_automoc.cpp.o.requires
	$(MAKE) -f embedding/CMakeFiles/targets_patchy.dir/build.make embedding/CMakeFiles/targets_patchy.dir/targets_patchy_automoc.cpp.o.provides.build
.PHONY : embedding/CMakeFiles/targets_patchy.dir/targets_patchy_automoc.cpp.o.provides

embedding/CMakeFiles/targets_patchy.dir/targets_patchy_automoc.cpp.o.provides.build: embedding/CMakeFiles/targets_patchy.dir/targets_patchy_automoc.cpp.o


# Object files for target targets_patchy
targets_patchy_OBJECTS = \
"CMakeFiles/targets_patchy.dir/targets_patchy.cpp.o" \
"CMakeFiles/targets_patchy.dir/targets_patchy_automoc.cpp.o"

# External object files for target targets_patchy
targets_patchy_EXTERNAL_OBJECTS =

embedding/targets_patchy: embedding/CMakeFiles/targets_patchy.dir/targets_patchy.cpp.o
embedding/targets_patchy: embedding/CMakeFiles/targets_patchy.dir/targets_patchy_automoc.cpp.o
embedding/targets_patchy: embedding/CMakeFiles/targets_patchy.dir/build.make
embedding/targets_patchy: embedding/CMakeFiles/targets_patchy.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/stef/Documents/CollectiveLevyWalk/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable targets_patchy"
	cd /home/stef/Documents/CollectiveLevyWalk/build/embedding && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/targets_patchy.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
embedding/CMakeFiles/targets_patchy.dir/build: embedding/targets_patchy

.PHONY : embedding/CMakeFiles/targets_patchy.dir/build

embedding/CMakeFiles/targets_patchy.dir/requires: embedding/CMakeFiles/targets_patchy.dir/targets_patchy.cpp.o.requires
embedding/CMakeFiles/targets_patchy.dir/requires: embedding/CMakeFiles/targets_patchy.dir/targets_patchy_automoc.cpp.o.requires

.PHONY : embedding/CMakeFiles/targets_patchy.dir/requires

embedding/CMakeFiles/targets_patchy.dir/clean:
	cd /home/stef/Documents/CollectiveLevyWalk/build/embedding && $(CMAKE_COMMAND) -P CMakeFiles/targets_patchy.dir/cmake_clean.cmake
.PHONY : embedding/CMakeFiles/targets_patchy.dir/clean

embedding/CMakeFiles/targets_patchy.dir/depend:
	cd /home/stef/Documents/CollectiveLevyWalk/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/stef/Documents/CollectiveLevyWalk /home/stef/Documents/CollectiveLevyWalk/embedding /home/stef/Documents/CollectiveLevyWalk/build /home/stef/Documents/CollectiveLevyWalk/build/embedding /home/stef/Documents/CollectiveLevyWalk/build/embedding/CMakeFiles/targets_patchy.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : embedding/CMakeFiles/targets_patchy.dir/depend

