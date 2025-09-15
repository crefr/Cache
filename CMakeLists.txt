cmake_minimum_required(VERSION 3.11)
project(my_cache)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

SET(SRCS src/main.cpp)
SET(HDRS_FLDR headers)

add_executable(cache ${SRCS})
target_include_directories(cache PUBLIC ${HDRS_FLDR})

