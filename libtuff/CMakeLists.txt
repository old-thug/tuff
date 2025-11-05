cmake_minimum_required(VERSION 3.20)
project(tuff C)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if (MSVC)
    add_compile_options(/W4 /permissive-)
else()
    add_compile_options(-Wall -ggdb -Wno-unused-variable)
endif()

file(GLOB_RECURSE LIB_SOURCES
    "${PROJECT_SOURCE_DIR}/*.c"
)

add_library(${PROJECT_NAME} STATIC SHARED ${LIB_SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/libtuff
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin"
)
