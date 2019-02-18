#!/bin/bash

SRC_NAME=utils
UNIT_TEST_CPP=utils_UnitTest.cpp
SRC_CPP=../../third/utils/src/${SRC_NAME}.cpp

INC="\
-I ../inc/ \
-I ../../third/utils/inc/ "

LIB="\
-L ../../third/basic_lib/configure/lib/ -lconfig \
-L -lcrypto -lpthread -lm -lstdc++ -lrt -lssl -ldl "


g++ -std=c++11 ${SRC_CPP}  ${UNIT_TEST_CPP} ${INC} ${LIB} -fprofile-arcs -ftest-coverage

./a.out
cp ${SRC_CPP} .
gcov ${SRC_NAME}.cpp >> tmp
head -n 3 tmp>> tmp3
rm tmp
cat ${SRC_NAME}.cpp.gcov >> tmp3
mv tmp3 code_covage.txt
shopt -s extglob
rm *.gcov
rm *.gcda
rm *.gcno
rm a.out
rm ${SRC_NAME}.cpp
head -n 2 code_covage.txt

