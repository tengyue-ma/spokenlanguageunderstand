#1.cmake verson
cmake_minimum_required(VERSION 2.8)

#2.project name
project(Regex)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DEXPORT")
set(CMAKE_CXX_FLAGS "-std=c++11 -o3")

set(HIREDIS_PATH ${CMAKE_CURRENT_SOURCE_DIR}/third/hiredis/)
set(LOG_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../third/hobotlog/)


#3.head file directory
include_directories(
        inc/
        ${HIREDIS_PATH}/
        ${LOG_PATH}/include/
)

#4.link directory
link_directories(
        ${HIREDIS_PATH}/
        ${LOG_PATH}/lib/
)

#5.source directory
aux_source_directory(src DIR_SRCS)

#6.set output directory
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib/)

#7.generate executable and so library
add_library(${PROJECT_NAME} SHARED ${DIR_SRCS})

#8.set dependent libs
set(LIBS
        hiredis
        logmodule
        log4cpp
        pthread
        )

#9.add link library
#target_link_libraries(test ${LIBS})
target_link_libraries(${PROJECT_NAME} ${LIBS})