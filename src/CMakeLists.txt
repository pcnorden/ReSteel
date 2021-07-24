cmake_minimum_required(VERSION 3.10)

# FOR THE LOVE OF GOD, DON'T USE THIS AS A GOOD EXAMPLE
#	This is me being really out of touch with CMake and
#	how to write good CMakeLists scripts!

# Set the project name
project(ReSteel)

# Specify the C++ standard and enforce it
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add the executable files, some skipped while main is being written somewhat
#add_executable(ReSteel CPU_Utilization.cpp draw_helper.cpp)
add_executable(ReSteel main.cpp)

# Link the .hxx files in the directory
target_include_directories(ReSteel PUBLIC "${PROJECT_BINARY_DIR}")

# Link HIDAPI library with a really crude setup, won't work on windows
# so needs some improvement, but works on several of my computers.
# SERIOUS TODO:
target_link_libraries(ReSteel "${CMAKE_SYSROOT}/usr/lib/libhidapi-hidraw.so")