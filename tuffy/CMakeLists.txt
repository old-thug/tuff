cmake_minimum_required(VERSION 3.10)
project(tuffy C)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if (MSVC)
    add_compile_options(/W4 /permissive-)
else()
    add_compile_options(-Wall -ggdb -Wno-unused-variable)
endif()

file(GLOB_RECURSE INTERPRETER_SOURCES
    "${PROJECT_SOURCE_DIR}/*.c"
)

add_executable(tuffy ${INTERPRETER_SOURCES})
target_include_directories(tuffy PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/.
    ${CMAKE_SOURCE_DIR}/libtuff
    ${CMAKE_SOURCE_DIR}/tuffy
)
target_link_libraries(tuffy PRIVATE tuff)

set_target_properties(tuffy PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin"
)
