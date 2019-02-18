#!/bin/bash
cd ../
source export_so.sh

cd unit_test/

SRC_NAME=SemanticInfo
UNIT_TEST_CPP=./SemanticInfo/SemanticInfo_UnitTest.cpp
SRC_CPP=../work_flow/src/${SRC_NAME}.cpp
WORKFLOW=../work_flow/src/WorkFlow.cpp

INC="\
-I inc/ \
-I ../third/basic_lib/configure/include/ \
-I ../third/basic_lib/lib2-64/ullib/include/ \
-I ../third/basic_lib/lib2-64/bsl/include/ \
-I ../third/basic_lib/lib2-64/spreg/include/ \
-I ../third/hobotlog/include/ \
-I ../slu_log/inc/ \
-I ../third/utils/inc/ \
-I ../third/processor/inc/ \
-I ../work_flow/inc/ \
-I ../slu_log/inc/ \
"

LIB="\
-L ../third/basic_lib/configure/lib/ -lconfig \
-L ../third/basic_lib/lib2-64/ullib/lib/ -lullib \
-L ../third/basic_lib/lib2-64/bsl/lib/ -lbsl \
-L ../third/basic_lib/lib2-64/spreg/lib/ -lspreg \
-L ../third/hobotlog/lib/ -llogmodule -llog4cpp \
-L ../third/utils/lib/ -lutils \
-L ../third/processor/lib/ -lprocessor \
-L -lcrypto -lpthread -lm -lstdc++ -lrt -lssl -ldl"

g++ -std=c++11 ${SRC_CPP} ${UNIT_TEST_CPP} ${WORKFLOW} ${INC} ${LIB} -fprofile-arcs -ftest-coverage

./a.out
cp ${SRC_CPP} .
gcov ${SRC_NAME}.cpp >> tmp
head -n 3 tmp >> tmp3

cat ${SRC_NAME}.cpp.gcov >> tmp3
mv tmp3 ./SemanticInfo/code_coverage.txt
shopt -s extglob
rm tmp
rm *.gcov
rm *.gcda
rm *.gcno
rm a.out
rm ${SRC_NAME}.cpp
head -n 2 ./SemanticInfo/code_coverage.txt
