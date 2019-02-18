#!/bin/bash

cd ../ 
source export_so.sh h

cd unit_test/

SRC_NAME=WorkFlow 
UNIT_TEST_CPP=./WorkFlow/WorkFlow_UnitTest.cpp
SRC_CPP=../work_flow/src/${SRC_NAME}.cpp

INC="\
-I ../unit_test/inc/ \
-I ../work_flow/inc/ \
-I ../third/basic_lib/lib2-64/spreg/include/ \
-I ../third/hobotlog/include/ \
-I ../slu_log/inc/ \
-I ../third/basic_lib/configure/include/ \
-I ../third/basic_lib/lib2-64/bsl/include/ \
-I ../third/basic_lib/lib2-64/ullib/include/ \
"

LIB="\
-L ../third/basic_lib/configure/lib/ -lconfig \
-L ../third/basic_lib/lib2-64/bsl/lib/ -lbsl \
-L ../third/basic_lib/lib2-64/spreg/lib/ -lspreg \
-L ../third/basic_lib/lib2-64/ullib/lib/ -lullib \
-L ../third/hobotlog/lib/ -llogmodule -llog4cpp \
-L -lcrypto -lpthread -lm -lstdc++ -lrt -lssl -ldl
"

g++ -std=c++11 ${SRC_CPP} ${UNIT_TEST_CPP} ${INC} ${LIB} -fprofile-arcs -ftest-coverage
./a.out
cp ${SRC_CPP} .
gcov ${SCR_NAME}.cpp >> tmp
head -n 3 tmp >> tmp3
rm tmp
cat ${SRC_NAME}.cpp.gcov >> tmp3
mv tmp3 ./WorkFlow/code_coverage.txt
shopt -s extglob
rm *.gcov
rm *.gcda
rm *.gcno
rm a.out
rm ${SRC_NAME}.cpp
ead -n 2 ./WorkFlow/code_coverage.txt
