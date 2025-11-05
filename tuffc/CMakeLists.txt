cmake_minimum_required(VERSION 3.10)
project(tuffc C)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if (MSVC)
    add_compile_options(/W4 /permissive-)
else()
    add_compile_options(-Wall -ggdb -Wno-unused-variable)
endif()

file(GLOB_RECURSE COMPILER_SOURCES
    "${PROJECT_SOURCE_DIR}/*.c"
)

add_executable(tuffc ${COMPILER_SOURCES})
target_include_directories(tuffc PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/.
    ${CMAKE_SOURCE_DIR}/libtuff
    ${CMAKE_SOURCE_DIR}/tuffc
)
target_link_libraries(tuffc PRIVATE tuff)

set_target_properties(tuffc PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin"
)
