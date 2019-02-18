#!/bin/bash
cd ../../ 
source export_so.sh

cd unit_test/intent_inform

SRC_NAME=intent_inform
UNIT_TEST_CPP=intent_inform_UnitTest.cpp
SRC_CPP=../../classifier/intent_inform/src/${SRC_NAME}.cpp

INC="\
-I ../inc \
-I ../../third/basic_lib/configure/include/ \
-I ../../third/basic_lib/lib2-64/ullib/include/ \
-I ../../third/basic_lib/lib2-64/bsl/include/ \
-I ../../third/basic_lib/lib2-64/spreg/include/ \
-I ../../third/hobotlog/include/ \
-I ../../slu_log/inc/ \
-I ../../third/utils/inc/ \
-I ../../classifier/clf_base/inc/ \
-I ../../third/processor/inc/ \
-I ../../feat_extractor/feat_base/inc/ \
-I ../../feat_extractor/me_feat/inc/ \
-I ../../classifier/intent_inform/inc/ "

LIB="\
-L ../../third/basic_lib/configure/lib/ -lconfig \
-L ../../third/basic_lib/lib2-64/ullib/lib/ -lullib \
-L ../../third/basic_lib/lib2-64/bsl/lib/ -lbsl \
-L ../../third/basic_lib/lib2-64/spreg/lib/ -lspreg \
-L ../../third/hobotlog/lib/ -llogmodule -llog4cpp \
-L ../../third/utils/lib/ -lutils \
-L ../../feat_extractor/feat_base/lib/ -lfeatbase \
-L ../../feat_extractor/me_feat/lib/ -lmefeat \
-L ../../third/processor/lib/ -lprocessor \
-L -lcrypto -lpthread -lm -lstdc++ -lrt -lssl -ldl"

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

