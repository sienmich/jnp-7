cmake_minimum_required(VERSION 3.15)
project(jnp_7)

set(CMAKE_CXX_STANDARD 20)

include_directories(.)

SET(GCC_COVERAGE_COMPILE_FLAGS "-Wall -Wextra -pedantic -O3 -std=c++2a -fconcepts")

add_definitions(${GCC_COVERAGE_COMPILE_FLAGS})


add_executable(jnp_7
        bmp.cc
        bmp.h
        color.cc
        color.h
        coordinate.cc
        coordinate.h
        functional.h
        images.h
        images.cc
        example.cc
        )



#g++ -Wall -Wextra -pedantic -O3 -std=c++2a -fconcepts