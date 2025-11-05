cmake_minimum_required(VERSION 3.20)
project(TuffC C)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(libtuff)
add_subdirectory(tuffc)
add_subdirectory(tuffy)
