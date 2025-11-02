cmake_minimum_required(VERSION 3.20)
project(TuffC C)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if (MSVC)
    add_compile_options(/W4 /permissive-)
else()
    add_compile_options(-Wall -ggdb)
endif()

file(GLOB_RECURSE COMPILER_SOURCES
    "${CMAKE_SOURCE_DIR}/compiler/*.c"
)

add_executable(tuffc ${COMPILER_SOURCES})

target_include_directories(tuffc PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/compiler
)

set_target_properties(tuffc PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin"
)
