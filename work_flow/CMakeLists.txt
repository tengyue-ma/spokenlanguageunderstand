#1.cmake verson
cmake_minimum_required(VERSION 2.8)

#2.project name
project(semantic_parser)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DEXPORT")
set(CMAKE_CXX_FLAGS "-std=c++11 -O3")
SET(CMAKE_BUILD_TYPE "Debug")
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb")
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

set(ULLIB_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../third/basic_lib/lib2-64/ullib/)
set(BSL_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../third/basic_lib/lib2-64/bsl/)
set(SPREG_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../third/basic_lib/lib2-64/spreg/)
set(CONFIGURE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../third/basic_lib/configure/)
set(LOG_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../third/hobotlog/)
set(SLU_LOG_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../slu_log/)
set(CRF_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../third/crf/)
set(PROCESSOR_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../third/processor/)
set(UTILS_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../third/utils/)
set(LTP_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../third/ltp-3.4.0/)
set(FEAT_BASE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../feat_extractor/feat_base/)
set(CLASSIFIER_BASE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../classifier/clf_base/)
set(NER_BASE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../slot_filling/ner_base/)
set(REGEX_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../regex_v1.3/)

#3.head file directory
include_directories(
        inc/
        ${CONFIGURE_PATH}/include/
        ${ULLIB_PATH}/include/
        ${BSL_PATH}/include/
        ${SPREG_PATH}/include/
        ${LOG_PATH}/include/
        ${SLU_LOG_PATH}/inc/
        ${LTP_PATH}/include/
        ${CRF_PATH}/include/
        ${UTILS_PATH}/inc/
        ${PROCESSOR_PATH}/inc/
        ${FEAT_BASE_PATH}/inc/
        ${CLASSIFIER_BASE_PATH}/inc/
        ${NER_BASE_PATH}/inc/
        ${REGEX_PATH}/inc/
        ${REGEX_PATH}/third/hiredis/
)

#4.link directory
link_directories(
        ${PROCESSOR_PATH}/lib/
        ${LOG_PATH}/lib/
        ${CRF_PATH}/lib/
        ${UTILS_PATH}/lib/
        ${CONFIGURE_PATH}/lib/
        ${ULLIB_PATH}/lib/
        ${BSL_PATH}/lib/
        ${SPREG_PATH}/lib/
        ${LTP_PATH}/lib/
        ${REGEX_PATH}/third/hiredis/
        ${REGEX_PATH}/lib/
)

#5.source directory
aux_source_directory(src DIR_SRCS)

#6.set output directory
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib/)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin/)

#7.generate executable and .a library
add_executable(slu_test ${DIR_SRCS} test.cpp)
add_executable(slu_test1 ${DIR_SRCS} test1.cpp)
add_library(${PROJECT_NAME} ${DIR_SRCS})

#8.set dependent libs
set(LIBS
        processor
        logmodule
        log4cpp
        crf
        utils
        config
        ullib
        bsl
        spreg
        segmentor
        postagger
        ner
        parser
        boost_regex
        crypto
        pthread
        m
        stdc++
        rt
        ssl
        dl
        hiredis
        Regex
        )

#9.add link library
target_link_libraries(slu_test ${LIBS})
target_link_libraries(slu_test1 ${LIBS})
target_link_libraries(${PROJECT_NAME} ${LIBS})
