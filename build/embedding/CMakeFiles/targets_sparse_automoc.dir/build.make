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

# Utility rule file for targets_sparse_automoc.

# Include the progress variables for this target.
include embedding/CMakeFiles/targets_sparse_automoc.dir/progress.make

embedding/CMakeFiles/targets_sparse_automoc:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/stef/Documents/CollectiveLevyWalk/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Automatic moc for target targets_sparse"
	cd /home/stef/Documents/CollectiveLevyWalk/build/embedding && /usr/bin/cmake -E cmake_autogen /home/stef/Documents/CollectiveLevyWalk/build/embedding/CMakeFiles/targets_sparse_automoc.dir/ Debug

targets_sparse_automoc: embedding/CMakeFiles/targets_sparse_automoc
targets_sparse_automoc: embedding/CMakeFiles/targets_sparse_automoc.dir/build.make

.PHONY : targets_sparse_automoc

# Rule to build all files generated by this target.
embedding/CMakeFiles/targets_sparse_automoc.dir/build: targets_sparse_automoc

.PHONY : embedding/CMakeFiles/targets_sparse_automoc.dir/build

embedding/CMakeFiles/targets_sparse_automoc.dir/clean:
	cd /home/stef/Documents/CollectiveLevyWalk/build/embedding && $(CMAKE_COMMAND) -P CMakeFiles/targets_sparse_automoc.dir/cmake_clean.cmake
.PHONY : embedding/CMakeFiles/targets_sparse_automoc.dir/clean

embedding/CMakeFiles/targets_sparse_automoc.dir/depend:
	cd /home/stef/Documents/CollectiveLevyWalk/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/stef/Documents/CollectiveLevyWalk /home/stef/Documents/CollectiveLevyWalk/embedding /home/stef/Documents/CollectiveLevyWalk/build /home/stef/Documents/CollectiveLevyWalk/build/embedding /home/stef/Documents/CollectiveLevyWalk/build/embedding/CMakeFiles/targets_sparse_automoc.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : embedding/CMakeFiles/targets_sparse_automoc.dir/depend

