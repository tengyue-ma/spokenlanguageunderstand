#!/bin/bash
cd ../
source export_so.sh

cd work_flow/
source export_so.sh

cd ../unit_test/

SRC_NAME=SLU
UNIT_TEST_CPP=./SLU/SLU_UnitTest.cpp
SRC_CPP=../work_flow/src/${SRC_NAME}.cpp

INC="\
-I ../unit_test//inc/ \
-I ../third/basic_lib/configure/include/ \
-I ../third/basic_lib/lib2-64/ullib/include/ \
-I ../third/basic_lib/lib2-64/bsl/include/ \
-I ../third/basic_lib/lib2-64/spreg/include/ \
-I ../third/hobotlog/include/ \
-I ../slu_log/inc/ \
-I ../third/utils/inc/ \
-I ../third/processor/inc/ \
-I ../feat_extractor/feat_base/inc/ \
-I ../feat_extractor/me_feat/inc/ \
-I ../feat_extractor/calculator_feat/inc/ \
-I ../feat_extractor/crf_feat/inc/ \
-I ../feat_extractor/onehot_feat/inc/ \
-I ../feat_extractor/open_domain_feat/inc/ \
-I ../classifier/ME/inc/ \
-I ../classifier/clf_base/inc/ \
-I ../classifier/clf_nn/inc/ \
-I ../classifier/intent_inform/inc/ \
-I ../slot_filling/ner_base/inc/ \
-I ../slot_filling/CRF/inc/ \
-I ../slot_filling/open_domain/inc/ \
-I ../work_flow/inc/ \
-I ../regex_v1.3/inc/ \
-I ../regex_v1.3/third/hiredis/ \
-I ../../classifier/clf_base/inc/"

LIB="\
-L ../third/basic_lib/configure/lib/ -lconfig \
-L ../third/basic_lib/lib2-64/ullib/lib/ -lullib \
-L ../third/basic_lib/lib2-64/bsl/lib/ -lbsl \
-L ../third/basic_lib/lib2-64/spreg/lib/ -lspreg \
-L ../third/hobotlog/lib/ -llogmodule -llog4cpp \
-L ../third/utils/lib/ -lutils \
-L ../feat_extractor/feat_base/lib/ -lfeatbase \
-L ../feat_extractor/me_feat/lib/ -lmefeat \
-L ../third/processor/lib/ -lprocessor \
-L ../work_flow/lib/ -lsemantic_parser \
-L ../classifier/clf_base/lib/ -lclfbase \
-L ../classifier/ME/lib/ -lme \
-L ../classifier/clf_nn/lib/ -lnn \
-L ../feat_extractor/crf_feat/lib -lcrffeat \
-L ../feat_extractor/onehot_feat/lib/ -lonehotfeat \
-L ../feat_extractor/open_domain_feat/lib/ -lopendomain_feat \
-L ../regex_v1.3/lib/ -lRegex \
-L ../classifier/intent_inform/lib/ -lintent \
-L -lcrypto -lpthread -lm -lstdc++ -lrt -lssl -ldl"

g++ -std=c++11 ${SRC_CPP} ${UNIT_TEST_CPP} ${INC} ${LIB} -fprofile-arcs -ftest-coverage

./a.out
cp ${SRC_CPP} .
gcov ${SRC_NAME}.cpp >> tmp
head -n 3 tmp >> tmp3
cat ${SRC_NAME}.cpp.gcov >> tmp3
mv tmp3 ./SLU/code_coverage.txt
shopt -s extglob
rm tmp
rm *.gcov
rm *.gcda
rm *.gcno
rm a.out
rm ${SRC_NAME}.cpp
head -n 2 ./SLU/code_coverage.txt

