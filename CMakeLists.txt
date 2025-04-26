cmake_minimum_required(VERSION 3.10)
project(SimpleRenderer)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(SOURCES src/main.cpp src/glad.c)

add_executable(SimpleRenderer ${SOURCES})
target_include_directories(SimpleRenderer PRIVATE include/)

find_package(glfw3 REQUIRED)

target_link_libraries(SimpleRenderer PRIVATE glfw)
