cmake_minimum_required(VERSION 3.16)
project(my_cache)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)

SET(HDRS_FLDR headers)

add_executable(cache src/main.cpp)
add_executable(ideal src/ideal_cache.cpp)

target_include_directories(cache PUBLIC ${HDRS_FLDR})
target_include_directories(ideal PUBLIC ${HDRS_FLDR})

enable_testing()

SET(TESTS_DIR ${CMAKE_SOURCE_DIR}/tests)
file(GLOB testFiles ${TESTS_DIR}/*.dat)

foreach(file ${testFiles})
    add_test(NAME ${file}
             COMMAND bash -c "${CMAKE_SOURCE_DIR}/run_test.sh ./cache ${file}"
             WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
    set_tests_properties(${file} PROPERTIES DEPENDS cache)
endforeach()
