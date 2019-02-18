#1.cmake verson
cmake_minimum_required(VERSION 2.8)

#2.project name
project(nerbase)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-std=c++11 -o3")

set(UTILS_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../third/utils/)
set(PROCESSOR_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../third/processor/)
set(FEATBASE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../feat_extractor/feat_base/)

#3.head file directory
include_directories(
        inc/
        ${UTILS_PATH}/inc/
        ${PROCESSOR_PATH}/inc/
        ${FEATBASE_PATH}/inc/
)

#4.link directory
link_directories(
        ${UTILS_PATH}/lib/
        ${PROCESSOR_PATH}/lib/
        ${FEATBASE_PATH}/lib/
)

#5.source directory
aux_source_directory(src DIR_SRCS)

#6.set output directory
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib/)

#7.generate executable and so library
add_library(${PROJECT_NAME} SHARED ${DIR_SRCS})

#8.set dependent libs
set(LIBS
        utils
        processor
        featbase
        crypto
        pthread
        m
        stdc++
        rt
        ssl
        dl
        )

#9.add link library
target_link_libraries(${PROJECT_NAME} ${LIBS})