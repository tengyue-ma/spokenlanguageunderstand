#1.cmake verson
cmake_minimum_required(VERSION 2.8)

#2.project name
project(me)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-std=c++11 -o3")

set(UTILS_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../third/utils/)
set(PROCESSOR_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../third/processor/)
set(MEFEAT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../feat_extractor/me_feat/)
set(FEATBASE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../feat_extractor/feat_base/)
set(CLFBASE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../clf_base/)
set(ULLIB_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../third/basic_lib/lib2-64/ullib/)
set(BSL_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../third/basic_lib/lib2-64/bsl/)
set(SPREG_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../third/basic_lib/lib2-64/spreg/)
set(CONFIGURE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../third/basic_lib/configure/)
set(LOG_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../third/hobotlog/)
set(SLU_LOG_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../slu_log/)

#3.head file directory
include_directories(
        inc/
        ${CONFIGURE_PATH}/include/
        ${ULLIB_PATH}/include/
        ${BSL_PATH}/include/
        ${SPREG_PATH}/include/
        ${UTILS_PATH}/inc/
        ${PROCESSOR_PATH}/inc/
        ${FEATBASE_PATH}/inc/
        ${MEFEAT_PATH}/inc/
        ${CLFBASE_PATH}/inc/
        ${LOG_PATH}/include/
        ${SLU_LOG_PATH}/inc/
)

#4.link directory
link_directories(
        ${CONFIGURE_PATH}/lib/
        ${ULLIB_PATH}/lib/
        ${BSL_PATH}/lib/
        ${SPREG_PATH}/lib/
        ${UTILS_PATH}/lib/
        ${PROCESSOR_PATH}/lib/
        ${FEATBASE_PATH}/lib/
        ${MEFEAT_PATH}/lib/
        ${CLFBASE_PATH}/lib/
        ${LOG_PATH}/lib/
)

#5.source directory
aux_source_directory(src DIR_SRCS)

#6.set output directory
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib/)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin/)

#7.generate executable and so library
add_executable(me_test ${DIR_SRCS} test.cpp)
add_library(${PROJECT_NAME} SHARED ${DIR_SRCS})

#8.set dependent libs
set(LIBS
        config
        ullib
        bsl
        spreg
        utils
        processor
        featbase
        mefeat
        clfbase
        logmodule
        log4cpp
        pthread
        dl
        )

#9.add link library
target_link_libraries(me_test ${LIBS})
target_link_libraries(${PROJECT_NAME} ${LIBS})