#!/bin/bash
cd ../../
source export_so.sh

cd unit_test/slot_filling_CRF

SRC_NAME=SlotTagger
UNIT_TEST_CPP=CRF_UnitTest.cpp
SRC_CPP=../../slot_filling/CRF/src/${SRC_NAME}.cpp
OTHER_CPP=../../slot_filling/CRF/src/CRFWrapper.cpp
OTHER_NAME=CRFWrapper

INC="\
-I ../inc \
-I ../../third/basic_lib/configure/include/ \
-I ../../third/basic_lib/lib2-64/ullib/include/ \
-I ../../third/basic_lib/lib2-64/bsl/include/ \
-I ../../third/basic_lib/lib2-64/spreg/include/ \
-I ../../third/hobotlog/include/ \
-I ../../slu_log/inc/ \
-I ../../third/utils/inc/ \
-I ../../third/processor/inc/ \
-I ../../feat_extractor/feat_base/inc/ \
-I ../../feat_extractor/crf_feat/inc/ \
-I ../../slot_filling/ner_base/inc/ \
-I ../../slot_filling/CRF/crf/include/
-I ../../slot_filling/CRF/inc/ "

LIB="\
-L ../../third/basic_lib/configure/lib/ -lconfig \
-L ../../third/basic_lib/lib2-64/ullib/lib/ -lullib \
-L ../../third/basic_lib/lib2-64/bsl/lib/ -lbsl \
-L ../../third/basic_lib/lib2-64/spreg/lib/ -lspreg \
-L ../../third/hobotlog/lib/ -llogmodule -llog4cpp \
-L ../../third/utils/lib/ -lutils \
-L ../../third/processor/lib/ -lprocessor \
-L ../../feat_extractor/feat_base/lib/ -lfeatbase \
-L ../../feat_extractor/crf_feat/lib/ -lcrffeat \
-L ../../slot_filling/ner_base/lib/ -lnerbase \
-L ../../slot_filling/CRF/crf/lib/ -lcrf++ \
-L -llcrypto -lpthread -lm -lstdc++ -lrt -lssl -ldl"

g++ -std=c++11 ${SRC_CPP} ${UNIT_TEST_CPP} ${OTHER_CPP} ${INC} ${LIB} -fprofile-arcs -ftest-coverage

./a.out
cp ${SRC_CPP} .
gcov ${SRC_NAME}.cpp >> tmp
head -n 3 tmp >> tmp3
rm tmp
cat ${SRC_NAME}.cpp.gcov >> tmp3
mv tmp3 code_coverage_${SRC_NAME}.txt

./a.out
cp ${OTHER_CPP} .
gcov ${OTHER_NAME}.cpp >> tmp
head -n 3 tmp >> tmp3
rm tmp
cat ${OTHER_NAME}.cpp.gcov >> tmp3
mv tmp3 code_coverage_${OTHER_NAME}.txt

head -n 2 code_coverage_${SRC_NAME}.txt
head -n 2 code_coverage_${OTHER_NAME}.txt

shopt -s extglob
rm *.gcov
rm *.gcda
rm *.gcno
rm a.out
rm ${SRC_NAME}.cpp
rm ${OTHER_NAME}.cpp


