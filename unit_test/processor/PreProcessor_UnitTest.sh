#!/bin/bash

SRC_NAME=PreProcessor
UNIT_TEST_CPP=PreProcessor_UnitTest.cpp
SRC_CPP=../../third/processor/src/${SRC_NAME}.cpp

SEMANTIC_WORKDIR=/mnt/sdd/haoqiang.liang/spokenlanguageunderstand

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:${SEMANTIC_WORKDIR}/third/ltp-3.4.0/lib/

INC="\
-I ../inc \
-I ../../third/processor/inc/ \
-I ../../third/ltp-3.4.0/include/ \
-I ../../third/basic_lib/configure/include/ \
-I ../../third/basic_lib/lib2-64/ullib/include/ \
-I ../../third/basic_lib/lib2-64/bsl/include/ \
-I ../../third/basic_lib/lib2-64/spreg/include/ \
-I ../../third/hobotlog/include/ \
-I ../../slu_log/inc/ \
-I ../../third/utils/inc/ \
-I ../../feat_extractor/feat_base/inc/ \
-I ../../feat_extractor/open_domain_feat/inc/"

LIB="\
-L ../../third/ltp-3.4.0/lib/ -lsegmentor -lboost_regex  -lpostagger -lner -lparser \
-L -lcrypto -lpthread -lm -lstdc++ -lrt -lssl -ldl
"

g++ -std=c++11 ${SRC_CPP} ${UNIT_TEST_CPP} ${INC} ${LIB} -fprofile-arcs -ftest-coverage

./a.out
cp ${SRC_CPP} .
gcov ${SRC_NAME}.cpp >> tmp
head -n 3 tmp >> tmp3
rm tmp
cat ${SRC_NAME}.cpp.gcov >> tmp3
mv tmp3 code_coverage.txt
shopt -s extglob
rm *.gcov
rm *.gcda
rm *.gcno
rm a.out
rm ${SRC_NAME}.cpp
head -n 2 code_coverage.txt
