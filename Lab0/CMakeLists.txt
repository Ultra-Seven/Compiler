cmake_minimum_required(VERSION 3.8)
project(Lab0)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES main.c prog1.c slp.c util.c)
add_executable(Lab0 ${SOURCE_FILES})