cmake_minimum_required(VERSION 3.5)

# Build type
set (CMAKE_EXPORT_COMPILE_COMMANDS ON)

# C++
set (CMAKE_CXX_STANDARD 23)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

# Project declaration
project(Jenjin)

add_subdirectory(megasource/libs/)
add_subdirectory(engine)
add_subdirectory(jenjin)
