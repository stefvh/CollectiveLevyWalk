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
include controllers/footbot_clw/CMakeFiles/footbot_clw.dir/depend.make

# Include the progress variables for this target.
include controllers/footbot_clw/CMakeFiles/footbot_clw.dir/progress.make

# Include the compile flags for this target's objects.
include controllers/footbot_clw/CMakeFiles/footbot_clw.dir/flags.make

controllers/footbot_clw/CMakeFiles/footbot_clw.dir/footbot_clw.cpp.o: controllers/footbot_clw/CMakeFiles/footbot_clw.dir/flags.make
controllers/footbot_clw/CMakeFiles/footbot_clw.dir/footbot_clw.cpp.o: ../controllers/footbot_clw/footbot_clw.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/stef/Documents/CollectiveLevyWalk/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object controllers/footbot_clw/CMakeFiles/footbot_clw.dir/footbot_clw.cpp.o"
	cd /home/stef/Documents/CollectiveLevyWalk/build/controllers/footbot_clw && /usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/footbot_clw.dir/footbot_clw.cpp.o -c /home/stef/Documents/CollectiveLevyWalk/controllers/footbot_clw/footbot_clw.cpp

controllers/footbot_clw/CMakeFiles/footbot_clw.dir/footbot_clw.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/footbot_clw.dir/footbot_clw.cpp.i"
	cd /home/stef/Documents/CollectiveLevyWalk/build/controllers/footbot_clw && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/stef/Documents/CollectiveLevyWalk/controllers/footbot_clw/footbot_clw.cpp > CMakeFiles/footbot_clw.dir/footbot_clw.cpp.i

controllers/footbot_clw/CMakeFiles/footbot_clw.dir/footbot_clw.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/footbot_clw.dir/footbot_clw.cpp.s"
	cd /home/stef/Documents/CollectiveLevyWalk/build/controllers/footbot_clw && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/stef/Documents/CollectiveLevyWalk/controllers/footbot_clw/footbot_clw.cpp -o CMakeFiles/footbot_clw.dir/footbot_clw.cpp.s

controllers/footbot_clw/CMakeFiles/footbot_clw.dir/footbot_clw.cpp.o.requires:

.PHONY : controllers/footbot_clw/CMakeFiles/footbot_clw.dir/footbot_clw.cpp.o.requires

controllers/footbot_clw/CMakeFiles/footbot_clw.dir/footbot_clw.cpp.o.provides: controllers/footbot_clw/CMakeFiles/footbot_clw.dir/footbot_clw.cpp.o.requires
	$(MAKE) -f controllers/footbot_clw/CMakeFiles/footbot_clw.dir/build.make controllers/footbot_clw/CMakeFiles/footbot_clw.dir/footbot_clw.cpp.o.provides.build
.PHONY : controllers/footbot_clw/CMakeFiles/footbot_clw.dir/footbot_clw.cpp.o.provides

controllers/footbot_clw/CMakeFiles/footbot_clw.dir/footbot_clw.cpp.o.provides.build: controllers/footbot_clw/CMakeFiles/footbot_clw.dir/footbot_clw.cpp.o


controllers/footbot_clw/CMakeFiles/footbot_clw.dir/footbot_clw_automoc.cpp.o: controllers/footbot_clw/CMakeFiles/footbot_clw.dir/flags.make
controllers/footbot_clw/CMakeFiles/footbot_clw.dir/footbot_clw_automoc.cpp.o: controllers/footbot_clw/footbot_clw_automoc.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/stef/Documents/CollectiveLevyWalk/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object controllers/footbot_clw/CMakeFiles/footbot_clw.dir/footbot_clw_automoc.cpp.o"
	cd /home/stef/Documents/CollectiveLevyWalk/build/controllers/footbot_clw && /usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/footbot_clw.dir/footbot_clw_automoc.cpp.o -c /home/stef/Documents/CollectiveLevyWalk/build/controllers/footbot_clw/footbot_clw_automoc.cpp

controllers/footbot_clw/CMakeFiles/footbot_clw.dir/footbot_clw_automoc.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/footbot_clw.dir/footbot_clw_automoc.cpp.i"
	cd /home/stef/Documents/CollectiveLevyWalk/build/controllers/footbot_clw && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/stef/Documents/CollectiveLevyWalk/build/controllers/footbot_clw/footbot_clw_automoc.cpp > CMakeFiles/footbot_clw.dir/footbot_clw_automoc.cpp.i

controllers/footbot_clw/CMakeFiles/footbot_clw.dir/footbot_clw_automoc.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/footbot_clw.dir/footbot_clw_automoc.cpp.s"
	cd /home/stef/Documents/CollectiveLevyWalk/build/controllers/footbot_clw && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/stef/Documents/CollectiveLevyWalk/build/controllers/footbot_clw/footbot_clw_automoc.cpp -o CMakeFiles/footbot_clw.dir/footbot_clw_automoc.cpp.s

controllers/footbot_clw/CMakeFiles/footbot_clw.dir/footbot_clw_automoc.cpp.o.requires:

.PHONY : controllers/footbot_clw/CMakeFiles/footbot_clw.dir/footbot_clw_automoc.cpp.o.requires

controllers/footbot_clw/CMakeFiles/footbot_clw.dir/footbot_clw_automoc.cpp.o.provides: controllers/footbot_clw/CMakeFiles/footbot_clw.dir/footbot_clw_automoc.cpp.o.requires
	$(MAKE) -f controllers/footbot_clw/CMakeFiles/footbot_clw.dir/build.make controllers/footbot_clw/CMakeFiles/footbot_clw.dir/footbot_clw_automoc.cpp.o.provides.build
.PHONY : controllers/footbot_clw/CMakeFiles/footbot_clw.dir/footbot_clw_automoc.cpp.o.provides

controllers/footbot_clw/CMakeFiles/footbot_clw.dir/footbot_clw_automoc.cpp.o.provides.build: controllers/footbot_clw/CMakeFiles/footbot_clw.dir/footbot_clw_automoc.cpp.o


# Object files for target footbot_clw
footbot_clw_OBJECTS = \
"CMakeFiles/footbot_clw.dir/footbot_clw.cpp.o" \
"CMakeFiles/footbot_clw.dir/footbot_clw_automoc.cpp.o"

# External object files for target footbot_clw
footbot_clw_EXTERNAL_OBJECTS =

controllers/footbot_clw/libfootbot_clw.so: controllers/footbot_clw/CMakeFiles/footbot_clw.dir/footbot_clw.cpp.o
controllers/footbot_clw/libfootbot_clw.so: controllers/footbot_clw/CMakeFiles/footbot_clw.dir/footbot_clw_automoc.cpp.o
controllers/footbot_clw/libfootbot_clw.so: controllers/footbot_clw/CMakeFiles/footbot_clw.dir/build.make
controllers/footbot_clw/libfootbot_clw.so: controllers/footbot_ilw/libfootbot_ilw.so
controllers/footbot_clw/libfootbot_clw.so: controllers/footbot_clw/CMakeFiles/footbot_clw.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/stef/Documents/CollectiveLevyWalk/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX shared library libfootbot_clw.so"
	cd /home/stef/Documents/CollectiveLevyWalk/build/controllers/footbot_clw && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/footbot_clw.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
controllers/footbot_clw/CMakeFiles/footbot_clw.dir/build: controllers/footbot_clw/libfootbot_clw.so

.PHONY : controllers/footbot_clw/CMakeFiles/footbot_clw.dir/build

controllers/footbot_clw/CMakeFiles/footbot_clw.dir/requires: controllers/footbot_clw/CMakeFiles/footbot_clw.dir/footbot_clw.cpp.o.requires
controllers/footbot_clw/CMakeFiles/footbot_clw.dir/requires: controllers/footbot_clw/CMakeFiles/footbot_clw.dir/footbot_clw_automoc.cpp.o.requires

.PHONY : controllers/footbot_clw/CMakeFiles/footbot_clw.dir/requires

controllers/footbot_clw/CMakeFiles/footbot_clw.dir/clean:
	cd /home/stef/Documents/CollectiveLevyWalk/build/controllers/footbot_clw && $(CMAKE_COMMAND) -P CMakeFiles/footbot_clw.dir/cmake_clean.cmake
.PHONY : controllers/footbot_clw/CMakeFiles/footbot_clw.dir/clean

controllers/footbot_clw/CMakeFiles/footbot_clw.dir/depend:
	cd /home/stef/Documents/CollectiveLevyWalk/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/stef/Documents/CollectiveLevyWalk /home/stef/Documents/CollectiveLevyWalk/controllers/footbot_clw /home/stef/Documents/CollectiveLevyWalk/build /home/stef/Documents/CollectiveLevyWalk/build/controllers/footbot_clw /home/stef/Documents/CollectiveLevyWalk/build/controllers/footbot_clw/CMakeFiles/footbot_clw.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : controllers/footbot_clw/CMakeFiles/footbot_clw.dir/depend

