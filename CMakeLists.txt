cmake_minimum_required(VERSION 3.8)
project(executor)

set(CMAKE_C_STANDARD 11)

set(SOURCE_FILES main.c executor.c executor.h)
add_executable(executor ${SOURCE_FILES})