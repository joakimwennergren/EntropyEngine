# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.28

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
CMAKE_SOURCE_DIR = /home/joakim-wennergren/CLionProjects/EntropyEngine/build/_deps/vma-subbuild

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/joakim-wennergren/CLionProjects/EntropyEngine/build/_deps/vma-subbuild

# Utility rule file for vma-populate.

# Include any custom commands dependencies for this target.
include CMakeFiles/vma-populate.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/vma-populate.dir/progress.make

CMakeFiles/vma-populate: CMakeFiles/vma-populate-complete

CMakeFiles/vma-populate-complete: vma-populate-prefix/src/vma-populate-stamp/vma-populate-install
CMakeFiles/vma-populate-complete: vma-populate-prefix/src/vma-populate-stamp/vma-populate-mkdir
CMakeFiles/vma-populate-complete: vma-populate-prefix/src/vma-populate-stamp/vma-populate-download
CMakeFiles/vma-populate-complete: vma-populate-prefix/src/vma-populate-stamp/vma-populate-update
CMakeFiles/vma-populate-complete: vma-populate-prefix/src/vma-populate-stamp/vma-populate-patch
CMakeFiles/vma-populate-complete: vma-populate-prefix/src/vma-populate-stamp/vma-populate-configure
CMakeFiles/vma-populate-complete: vma-populate-prefix/src/vma-populate-stamp/vma-populate-build
CMakeFiles/vma-populate-complete: vma-populate-prefix/src/vma-populate-stamp/vma-populate-install
CMakeFiles/vma-populate-complete: vma-populate-prefix/src/vma-populate-stamp/vma-populate-test
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --blue --bold --progress-dir=/home/joakim-wennergren/CLionProjects/EntropyEngine/build/_deps/vma-subbuild/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Completed 'vma-populate'"
	/usr/bin/cmake -E make_directory /home/joakim-wennergren/CLionProjects/EntropyEngine/build/_deps/vma-subbuild/CMakeFiles
	/usr/bin/cmake -E touch /home/joakim-wennergren/CLionProjects/EntropyEngine/build/_deps/vma-subbuild/CMakeFiles/vma-populate-complete
	/usr/bin/cmake -E touch /home/joakim-wennergren/CLionProjects/EntropyEngine/build/_deps/vma-subbuild/vma-populate-prefix/src/vma-populate-stamp/vma-populate-done

vma-populate-prefix/src/vma-populate-stamp/vma-populate-update:
.PHONY : vma-populate-prefix/src/vma-populate-stamp/vma-populate-update

vma-populate-prefix/src/vma-populate-stamp/vma-populate-build: vma-populate-prefix/src/vma-populate-stamp/vma-populate-configure
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --blue --bold --progress-dir=/home/joakim-wennergren/CLionProjects/EntropyEngine/build/_deps/vma-subbuild/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "No build step for 'vma-populate'"
	cd /home/joakim-wennergren/CLionProjects/EntropyEngine/build/_deps/vma-build && /usr/bin/cmake -E echo_append
	cd /home/joakim-wennergren/CLionProjects/EntropyEngine/build/_deps/vma-build && /usr/bin/cmake -E touch /home/joakim-wennergren/CLionProjects/EntropyEngine/build/_deps/vma-subbuild/vma-populate-prefix/src/vma-populate-stamp/vma-populate-build

vma-populate-prefix/src/vma-populate-stamp/vma-populate-configure: vma-populate-prefix/tmp/vma-populate-cfgcmd.txt
vma-populate-prefix/src/vma-populate-stamp/vma-populate-configure: vma-populate-prefix/src/vma-populate-stamp/vma-populate-patch
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --blue --bold --progress-dir=/home/joakim-wennergren/CLionProjects/EntropyEngine/build/_deps/vma-subbuild/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "No configure step for 'vma-populate'"
	cd /home/joakim-wennergren/CLionProjects/EntropyEngine/build/_deps/vma-build && /usr/bin/cmake -E echo_append
	cd /home/joakim-wennergren/CLionProjects/EntropyEngine/build/_deps/vma-build && /usr/bin/cmake -E touch /home/joakim-wennergren/CLionProjects/EntropyEngine/build/_deps/vma-subbuild/vma-populate-prefix/src/vma-populate-stamp/vma-populate-configure

vma-populate-prefix/src/vma-populate-stamp/vma-populate-download: vma-populate-prefix/src/vma-populate-stamp/vma-populate-gitinfo.txt
vma-populate-prefix/src/vma-populate-stamp/vma-populate-download: vma-populate-prefix/src/vma-populate-stamp/vma-populate-mkdir
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --blue --bold --progress-dir=/home/joakim-wennergren/CLionProjects/EntropyEngine/build/_deps/vma-subbuild/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Performing download step (git clone) for 'vma-populate'"
	cd /home/joakim-wennergren/CLionProjects/EntropyEngine/build/_deps && /usr/bin/cmake -P /home/joakim-wennergren/CLionProjects/EntropyEngine/build/_deps/vma-subbuild/vma-populate-prefix/tmp/vma-populate-gitclone.cmake
	cd /home/joakim-wennergren/CLionProjects/EntropyEngine/build/_deps && /usr/bin/cmake -E touch /home/joakim-wennergren/CLionProjects/EntropyEngine/build/_deps/vma-subbuild/vma-populate-prefix/src/vma-populate-stamp/vma-populate-download

vma-populate-prefix/src/vma-populate-stamp/vma-populate-install: vma-populate-prefix/src/vma-populate-stamp/vma-populate-build
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --blue --bold --progress-dir=/home/joakim-wennergren/CLionProjects/EntropyEngine/build/_deps/vma-subbuild/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "No install step for 'vma-populate'"
	cd /home/joakim-wennergren/CLionProjects/EntropyEngine/build/_deps/vma-build && /usr/bin/cmake -E echo_append
	cd /home/joakim-wennergren/CLionProjects/EntropyEngine/build/_deps/vma-build && /usr/bin/cmake -E touch /home/joakim-wennergren/CLionProjects/EntropyEngine/build/_deps/vma-subbuild/vma-populate-prefix/src/vma-populate-stamp/vma-populate-install

vma-populate-prefix/src/vma-populate-stamp/vma-populate-mkdir:
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --blue --bold --progress-dir=/home/joakim-wennergren/CLionProjects/EntropyEngine/build/_deps/vma-subbuild/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Creating directories for 'vma-populate'"
	/usr/bin/cmake -Dcfgdir= -P /home/joakim-wennergren/CLionProjects/EntropyEngine/build/_deps/vma-subbuild/vma-populate-prefix/tmp/vma-populate-mkdirs.cmake
	/usr/bin/cmake -E touch /home/joakim-wennergren/CLionProjects/EntropyEngine/build/_deps/vma-subbuild/vma-populate-prefix/src/vma-populate-stamp/vma-populate-mkdir

vma-populate-prefix/src/vma-populate-stamp/vma-populate-patch: vma-populate-prefix/src/vma-populate-stamp/vma-populate-patch-info.txt
vma-populate-prefix/src/vma-populate-stamp/vma-populate-patch: vma-populate-prefix/src/vma-populate-stamp/vma-populate-update
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --blue --bold --progress-dir=/home/joakim-wennergren/CLionProjects/EntropyEngine/build/_deps/vma-subbuild/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "No patch step for 'vma-populate'"
	/usr/bin/cmake -E echo_append
	/usr/bin/cmake -E touch /home/joakim-wennergren/CLionProjects/EntropyEngine/build/_deps/vma-subbuild/vma-populate-prefix/src/vma-populate-stamp/vma-populate-patch

vma-populate-prefix/src/vma-populate-stamp/vma-populate-update:
.PHONY : vma-populate-prefix/src/vma-populate-stamp/vma-populate-update

vma-populate-prefix/src/vma-populate-stamp/vma-populate-test: vma-populate-prefix/src/vma-populate-stamp/vma-populate-install
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --blue --bold --progress-dir=/home/joakim-wennergren/CLionProjects/EntropyEngine/build/_deps/vma-subbuild/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "No test step for 'vma-populate'"
	cd /home/joakim-wennergren/CLionProjects/EntropyEngine/build/_deps/vma-build && /usr/bin/cmake -E echo_append
	cd /home/joakim-wennergren/CLionProjects/EntropyEngine/build/_deps/vma-build && /usr/bin/cmake -E touch /home/joakim-wennergren/CLionProjects/EntropyEngine/build/_deps/vma-subbuild/vma-populate-prefix/src/vma-populate-stamp/vma-populate-test

vma-populate-prefix/src/vma-populate-stamp/vma-populate-update: vma-populate-prefix/tmp/vma-populate-gitupdate.cmake
vma-populate-prefix/src/vma-populate-stamp/vma-populate-update: vma-populate-prefix/src/vma-populate-stamp/vma-populate-update-info.txt
vma-populate-prefix/src/vma-populate-stamp/vma-populate-update: vma-populate-prefix/src/vma-populate-stamp/vma-populate-download
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --blue --bold --progress-dir=/home/joakim-wennergren/CLionProjects/EntropyEngine/build/_deps/vma-subbuild/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Performing update step for 'vma-populate'"
	cd /home/joakim-wennergren/CLionProjects/EntropyEngine/build/_deps/vma-src && /usr/bin/cmake -Dcan_fetch=YES -P /home/joakim-wennergren/CLionProjects/EntropyEngine/build/_deps/vma-subbuild/vma-populate-prefix/tmp/vma-populate-gitupdate.cmake

vma-populate: CMakeFiles/vma-populate
vma-populate: CMakeFiles/vma-populate-complete
vma-populate: vma-populate-prefix/src/vma-populate-stamp/vma-populate-build
vma-populate: vma-populate-prefix/src/vma-populate-stamp/vma-populate-configure
vma-populate: vma-populate-prefix/src/vma-populate-stamp/vma-populate-download
vma-populate: vma-populate-prefix/src/vma-populate-stamp/vma-populate-install
vma-populate: vma-populate-prefix/src/vma-populate-stamp/vma-populate-mkdir
vma-populate: vma-populate-prefix/src/vma-populate-stamp/vma-populate-patch
vma-populate: vma-populate-prefix/src/vma-populate-stamp/vma-populate-test
vma-populate: vma-populate-prefix/src/vma-populate-stamp/vma-populate-update
vma-populate: CMakeFiles/vma-populate.dir/build.make
.PHONY : vma-populate

# Rule to build all files generated by this target.
CMakeFiles/vma-populate.dir/build: vma-populate
.PHONY : CMakeFiles/vma-populate.dir/build

CMakeFiles/vma-populate.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/vma-populate.dir/cmake_clean.cmake
.PHONY : CMakeFiles/vma-populate.dir/clean

CMakeFiles/vma-populate.dir/depend:
	cd /home/joakim-wennergren/CLionProjects/EntropyEngine/build/_deps/vma-subbuild && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/joakim-wennergren/CLionProjects/EntropyEngine/build/_deps/vma-subbuild /home/joakim-wennergren/CLionProjects/EntropyEngine/build/_deps/vma-subbuild /home/joakim-wennergren/CLionProjects/EntropyEngine/build/_deps/vma-subbuild /home/joakim-wennergren/CLionProjects/EntropyEngine/build/_deps/vma-subbuild /home/joakim-wennergren/CLionProjects/EntropyEngine/build/_deps/vma-subbuild/CMakeFiles/vma-populate.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/vma-populate.dir/depend

