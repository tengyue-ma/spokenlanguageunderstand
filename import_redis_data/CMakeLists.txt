#1.cmake verson
cmake_minimum_required(VERSION 2.8)

#2.project name
project(importRedisData)

set(HIREDIS_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../regex_v1.3/third/hiredis/)

#3.head file directory
include_directories(
        ${HIREDIS_PATH}/
)

#4.link directory
link_directories(
        ${HIREDIS_PATH}/
)

#6.set output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

#7.generate executable and so library
add_executable(query_redis src/query_redis.cc)
add_executable(clear_redis src/clear_redis.cc)

#8.set dependent libs
set(LIBS
        hiredis
        )

#9.add link library
target_link_libraries(query_redis ${LIBS})
target_link_libraries(clear_redis ${LIBS})