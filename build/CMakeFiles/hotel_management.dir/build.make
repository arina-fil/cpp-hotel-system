# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.30

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

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "C:\Program Files\mingw64\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files\mingw64\bin\cmake.exe" -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = C:\Projects\c++_hotel

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:\Projects\c++_hotel\build

# Include any dependencies generated for this target.
include CMakeFiles/hotel_management.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/hotel_management.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/hotel_management.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/hotel_management.dir/flags.make

CMakeFiles/hotel_management.dir/main.cpp.obj: CMakeFiles/hotel_management.dir/flags.make
CMakeFiles/hotel_management.dir/main.cpp.obj: CMakeFiles/hotel_management.dir/includes_CXX.rsp
CMakeFiles/hotel_management.dir/main.cpp.obj: C:/Projects/c++_hotel/main.cpp
CMakeFiles/hotel_management.dir/main.cpp.obj: CMakeFiles/hotel_management.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=C:\Projects\c++_hotel\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/hotel_management.dir/main.cpp.obj"
	C:\PROGRA~1\mingw64\bin\C__~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/hotel_management.dir/main.cpp.obj -MF CMakeFiles\hotel_management.dir\main.cpp.obj.d -o CMakeFiles\hotel_management.dir\main.cpp.obj -c C:\Projects\c++_hotel\main.cpp

CMakeFiles/hotel_management.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/hotel_management.dir/main.cpp.i"
	C:\PROGRA~1\mingw64\bin\C__~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Projects\c++_hotel\main.cpp > CMakeFiles\hotel_management.dir\main.cpp.i

CMakeFiles/hotel_management.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/hotel_management.dir/main.cpp.s"
	C:\PROGRA~1\mingw64\bin\C__~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Projects\c++_hotel\main.cpp -o CMakeFiles\hotel_management.dir\main.cpp.s

# Object files for target hotel_management
hotel_management_OBJECTS = \
"CMakeFiles/hotel_management.dir/main.cpp.obj"

# External object files for target hotel_management
hotel_management_EXTERNAL_OBJECTS =

hotel_management.exe: CMakeFiles/hotel_management.dir/main.cpp.obj
hotel_management.exe: CMakeFiles/hotel_management.dir/build.make
hotel_management.exe: libhotel_system_core.a
hotel_management.exe: C:/Projects/c++_hotel/vcpkg/installed/x64-mingw-dynamic/lib/libpq.dll.a
hotel_management.exe: CMakeFiles/hotel_management.dir/linkLibs.rsp
hotel_management.exe: CMakeFiles/hotel_management.dir/objects1.rsp
hotel_management.exe: CMakeFiles/hotel_management.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=C:\Projects\c++_hotel\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable hotel_management.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\hotel_management.dir\link.txt --verbose=$(VERBOSE)
	C:\Windows\System32\WindowsPowerShell\v1.0\powershell.exe -noprofile -executionpolicy Bypass -file C:/Projects/c++_hotel/vcpkg/scripts/buildsystems/msbuild/applocal.ps1 -targetBinary C:/Projects/c++_hotel/build/hotel_management.exe -installedDir C:/Projects/c++_hotel/vcpkg/installed/x64-mingw-dynamic/bin -OutVariable out

# Rule to build all files generated by this target.
CMakeFiles/hotel_management.dir/build: hotel_management.exe
.PHONY : CMakeFiles/hotel_management.dir/build

CMakeFiles/hotel_management.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\hotel_management.dir\cmake_clean.cmake
.PHONY : CMakeFiles/hotel_management.dir/clean

CMakeFiles/hotel_management.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" C:\Projects\c++_hotel C:\Projects\c++_hotel C:\Projects\c++_hotel\build C:\Projects\c++_hotel\build C:\Projects\c++_hotel\build\CMakeFiles\hotel_management.dir\DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/hotel_management.dir/depend

