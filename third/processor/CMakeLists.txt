#1.cmake verson
cmake_minimum_required(VERSION 2.8)

#2.project name
project(processor)

set(CMAKE_CXX_STANDARD 11)

#3.head file directory
include_directories(
        inc/
        ../ltp-3.4.0/include/
)

#4.link directory
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../ltp-3.4.0/lib/)

#5.source directory
aux_source_directory(src DIR_SRCS)

#6.set output directory
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib/)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin/)


#7.generate executable and so library
add_executable(test ${DIR_SRCS})
add_library(${PROJECT_NAME} SHARED ${DIR_SRCS})

#8.set dependent libs
set(LIBS segmentor postagger ner parser boost_regex)

#9.add link library
target_link_libraries(test ${LIBS})
target_link_libraries(${PROJECT_NAME} ${LIBS})