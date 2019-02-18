#1.cmake verson
cmake_minimum_required(VERSION 2.8)

#2.project name
project(utils)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DEXPORT")
set(CMAKE_CXX_FLAGS "-std=c++11 -O3")

#3.head file directory
include_directories(
        inc/
)

#4.set output directory
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib/)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin/)

#5.generate executable and so library
add_executable(test src/utils.cpp test.cpp)
add_library(${PROJECT_NAME} SHARED src/utils.cpp)
