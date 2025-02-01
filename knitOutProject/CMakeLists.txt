set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
cmake_minimum_required(VERSION 3.8)
project(knitoutproject)

add_subdirectory(lib)
add_executable(knitoutproject MACOSX_BUNDLE knitoutproject.cpp)
