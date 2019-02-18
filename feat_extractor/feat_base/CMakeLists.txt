#1.cmake verson
cmake_minimum_required(VERSION 2.8)

#2.project name
project(featbase)

set(CMAKE_CXX_STANDARD 11)

#3.head file directory
include_directories(
        inc/
        ../../third/utils/inc/
        ../../third/processor/inc/
)

#5.source directory
aux_source_directory(src DIR_SRCS)

#6.set output directory
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib/)

#7.generate so library
add_library(${PROJECT_NAME} SHARED ${DIR_SRCS})
