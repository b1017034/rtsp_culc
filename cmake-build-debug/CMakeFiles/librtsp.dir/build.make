# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.19

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
CMAKE_COMMAND = /root/.local/share/JetBrains/Toolbox/apps/CLion/ch-0/211.7442.42/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /root/.local/share/JetBrains/Toolbox/apps/CLion/ch-0/211.7442.42/bin/cmake/linux/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /root/Documents/rtsp_culc

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /root/Documents/rtsp_culc/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/librtsp.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/librtsp.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/librtsp.dir/flags.make

CMakeFiles/librtsp.dir/librtsp/src/rtsp.c.o: CMakeFiles/librtsp.dir/flags.make
CMakeFiles/librtsp.dir/librtsp/src/rtsp.c.o: ../librtsp/src/rtsp.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/Documents/rtsp_culc/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/librtsp.dir/librtsp/src/rtsp.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/librtsp.dir/librtsp/src/rtsp.c.o -c /root/Documents/rtsp_culc/librtsp/src/rtsp.c

CMakeFiles/librtsp.dir/librtsp/src/rtsp.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/librtsp.dir/librtsp/src/rtsp.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /root/Documents/rtsp_culc/librtsp/src/rtsp.c > CMakeFiles/librtsp.dir/librtsp/src/rtsp.c.i

CMakeFiles/librtsp.dir/librtsp/src/rtsp.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/librtsp.dir/librtsp/src/rtsp.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /root/Documents/rtsp_culc/librtsp/src/rtsp.c -o CMakeFiles/librtsp.dir/librtsp/src/rtsp.c.s

CMakeFiles/librtsp.dir/librtsp/src/rtsp_client.c.o: CMakeFiles/librtsp.dir/flags.make
CMakeFiles/librtsp.dir/librtsp/src/rtsp_client.c.o: ../librtsp/src/rtsp_client.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/Documents/rtsp_culc/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/librtsp.dir/librtsp/src/rtsp_client.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/librtsp.dir/librtsp/src/rtsp_client.c.o -c /root/Documents/rtsp_culc/librtsp/src/rtsp_client.c

CMakeFiles/librtsp.dir/librtsp/src/rtsp_client.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/librtsp.dir/librtsp/src/rtsp_client.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /root/Documents/rtsp_culc/librtsp/src/rtsp_client.c > CMakeFiles/librtsp.dir/librtsp/src/rtsp_client.c.i

CMakeFiles/librtsp.dir/librtsp/src/rtsp_client.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/librtsp.dir/librtsp/src/rtsp_client.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /root/Documents/rtsp_culc/librtsp/src/rtsp_client.c -o CMakeFiles/librtsp.dir/librtsp/src/rtsp_client.c.s

CMakeFiles/librtsp.dir/librtsp/src/rtsp_client_session.c.o: CMakeFiles/librtsp.dir/flags.make
CMakeFiles/librtsp.dir/librtsp/src/rtsp_client_session.c.o: ../librtsp/src/rtsp_client_session.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/Documents/rtsp_culc/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object CMakeFiles/librtsp.dir/librtsp/src/rtsp_client_session.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/librtsp.dir/librtsp/src/rtsp_client_session.c.o -c /root/Documents/rtsp_culc/librtsp/src/rtsp_client_session.c

CMakeFiles/librtsp.dir/librtsp/src/rtsp_client_session.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/librtsp.dir/librtsp/src/rtsp_client_session.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /root/Documents/rtsp_culc/librtsp/src/rtsp_client_session.c > CMakeFiles/librtsp.dir/librtsp/src/rtsp_client_session.c.i

CMakeFiles/librtsp.dir/librtsp/src/rtsp_client_session.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/librtsp.dir/librtsp/src/rtsp_client_session.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /root/Documents/rtsp_culc/librtsp/src/rtsp_client_session.c -o CMakeFiles/librtsp.dir/librtsp/src/rtsp_client_session.c.s

CMakeFiles/librtsp.dir/librtsp/src/rtsp_server.c.o: CMakeFiles/librtsp.dir/flags.make
CMakeFiles/librtsp.dir/librtsp/src/rtsp_server.c.o: ../librtsp/src/rtsp_server.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/Documents/rtsp_culc/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building C object CMakeFiles/librtsp.dir/librtsp/src/rtsp_server.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/librtsp.dir/librtsp/src/rtsp_server.c.o -c /root/Documents/rtsp_culc/librtsp/src/rtsp_server.c

CMakeFiles/librtsp.dir/librtsp/src/rtsp_server.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/librtsp.dir/librtsp/src/rtsp_server.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /root/Documents/rtsp_culc/librtsp/src/rtsp_server.c > CMakeFiles/librtsp.dir/librtsp/src/rtsp_server.c.i

CMakeFiles/librtsp.dir/librtsp/src/rtsp_server.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/librtsp.dir/librtsp/src/rtsp_server.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /root/Documents/rtsp_culc/librtsp/src/rtsp_server.c -o CMakeFiles/librtsp.dir/librtsp/src/rtsp_server.c.s

CMakeFiles/librtsp.dir/librtsp/src/rtsp_server_request.c.o: CMakeFiles/librtsp.dir/flags.make
CMakeFiles/librtsp.dir/librtsp/src/rtsp_server_request.c.o: ../librtsp/src/rtsp_server_request.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/Documents/rtsp_culc/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building C object CMakeFiles/librtsp.dir/librtsp/src/rtsp_server_request.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/librtsp.dir/librtsp/src/rtsp_server_request.c.o -c /root/Documents/rtsp_culc/librtsp/src/rtsp_server_request.c

CMakeFiles/librtsp.dir/librtsp/src/rtsp_server_request.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/librtsp.dir/librtsp/src/rtsp_server_request.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /root/Documents/rtsp_culc/librtsp/src/rtsp_server_request.c > CMakeFiles/librtsp.dir/librtsp/src/rtsp_server_request.c.i

CMakeFiles/librtsp.dir/librtsp/src/rtsp_server_request.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/librtsp.dir/librtsp/src/rtsp_server_request.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /root/Documents/rtsp_culc/librtsp/src/rtsp_server_request.c -o CMakeFiles/librtsp.dir/librtsp/src/rtsp_server_request.c.s

CMakeFiles/librtsp.dir/librtsp/src/rtsp_server_session.c.o: CMakeFiles/librtsp.dir/flags.make
CMakeFiles/librtsp.dir/librtsp/src/rtsp_server_session.c.o: ../librtsp/src/rtsp_server_session.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/Documents/rtsp_culc/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building C object CMakeFiles/librtsp.dir/librtsp/src/rtsp_server_session.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/librtsp.dir/librtsp/src/rtsp_server_session.c.o -c /root/Documents/rtsp_culc/librtsp/src/rtsp_server_session.c

CMakeFiles/librtsp.dir/librtsp/src/rtsp_server_session.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/librtsp.dir/librtsp/src/rtsp_server_session.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /root/Documents/rtsp_culc/librtsp/src/rtsp_server_session.c > CMakeFiles/librtsp.dir/librtsp/src/rtsp_server_session.c.i

CMakeFiles/librtsp.dir/librtsp/src/rtsp_server_session.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/librtsp.dir/librtsp/src/rtsp_server_session.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /root/Documents/rtsp_culc/librtsp/src/rtsp_server_session.c -o CMakeFiles/librtsp.dir/librtsp/src/rtsp_server_session.c.s

# Object files for target librtsp
librtsp_OBJECTS = \
"CMakeFiles/librtsp.dir/librtsp/src/rtsp.c.o" \
"CMakeFiles/librtsp.dir/librtsp/src/rtsp_client.c.o" \
"CMakeFiles/librtsp.dir/librtsp/src/rtsp_client_session.c.o" \
"CMakeFiles/librtsp.dir/librtsp/src/rtsp_server.c.o" \
"CMakeFiles/librtsp.dir/librtsp/src/rtsp_server_request.c.o" \
"CMakeFiles/librtsp.dir/librtsp/src/rtsp_server_session.c.o"

# External object files for target librtsp
librtsp_EXTERNAL_OBJECTS =

liblibrtsp.so: CMakeFiles/librtsp.dir/librtsp/src/rtsp.c.o
liblibrtsp.so: CMakeFiles/librtsp.dir/librtsp/src/rtsp_client.c.o
liblibrtsp.so: CMakeFiles/librtsp.dir/librtsp/src/rtsp_client_session.c.o
liblibrtsp.so: CMakeFiles/librtsp.dir/librtsp/src/rtsp_server.c.o
liblibrtsp.so: CMakeFiles/librtsp.dir/librtsp/src/rtsp_server_request.c.o
liblibrtsp.so: CMakeFiles/librtsp.dir/librtsp/src/rtsp_server_session.c.o
liblibrtsp.so: CMakeFiles/librtsp.dir/build.make
liblibrtsp.so: liblibpomp.so
liblibrtsp.so: liblibfutils.so
liblibrtsp.so: liblibulog.so
liblibrtsp.so: CMakeFiles/librtsp.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/root/Documents/rtsp_culc/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Linking C shared library liblibrtsp.so"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/librtsp.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/librtsp.dir/build: liblibrtsp.so

.PHONY : CMakeFiles/librtsp.dir/build

CMakeFiles/librtsp.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/librtsp.dir/cmake_clean.cmake
.PHONY : CMakeFiles/librtsp.dir/clean

CMakeFiles/librtsp.dir/depend:
	cd /root/Documents/rtsp_culc/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /root/Documents/rtsp_culc /root/Documents/rtsp_culc /root/Documents/rtsp_culc/cmake-build-debug /root/Documents/rtsp_culc/cmake-build-debug /root/Documents/rtsp_culc/cmake-build-debug/CMakeFiles/librtsp.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/librtsp.dir/depend

