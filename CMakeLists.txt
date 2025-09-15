cmake_minimum_required(VERSION 3.11)
project(my_cache)

SET(SRCS src/main.cpp)
SET(HDRS_FLDR headers)

add_executable(cache ${SRCS})
target_include_directories(cache PUBLIC ${HDRS_FLDR})
