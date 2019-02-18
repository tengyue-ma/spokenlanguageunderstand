#!/bin/bash
cd ../../
source export_so.sh

cd unit_test/regex

UNIT_TEST_CPP=regex_UnitTest.cpp

INC="\
-I ../inc \
-I ../../third/hobotlog/include/ \
-I ../../slu_log/inc/ \
-I ../../regex_v1.3/inc/ \
-I ../../regex_v1.3/third/hiredis/"

LIB="\
-L ../../third/hobotlog/lib/ -llogmodule -llog4cpp \
-L ../../regex_v1.3/third/hiredis/ -lhiredis \
-L -lcrypto -lpthread -lm -lstdc++ -lrt -lssl -ldl"

ALL_SRC_CPP="\
../../regex_v1.3/src/charset.cc \
../../regex_v1.3/src/cJSON.cc \
../../regex_v1.3/src/domain_lex.cc \
../../regex_v1.3/src/expression.cc \
../../regex_v1.3/src/group.cc \
../../regex_v1.3/src/link.cc \
../../regex_v1.3/src/nfa.cc \
../../regex_v1.3/src/nfa_char_union.cc \
../../regex_v1.3/src/redis_wrapper.cc \
../../regex_v1.3/src/semantic.cc \
../../regex_v1.3/src/spregex.cc \
../../regex_v1.3/src/state.cc \
../../regex_v1.3/src/utils.cc \
../../regex_v1.3/src/variable.cc "

g++ -std=c++11 ${ALL_SRC_CPP} ${UNIT_TEST_CPP} ${INC} ${LIB} -fprofile-arcs -ftest-coverage

./a.out
cp ../../regex_v1.3/src/spregex.cc .
gcov spregex.cc >> tmp
head -n 3 tmp >> tmp3
rm tmp
cat spregex.cc.gcov >> tmp3
mv tmp3 code_coverage/spregex.txt

cp ../../regex_v1.3/src/domain_lex.cc .
gcov domain_lex.cc >> tmp
head -n 3 tmp >> tmp3
rm tmp
cat domain_lex.cc.gcov >> tmp3
mv tmp3 code_coverage/domain_lex.txt

cp ../../regex_v1.3/src/nfa.cc .
gcov nfa.cc >> tmp
head -n 3 tmp >> tmp3
rm tmp
cat nfa.cc.gcov >> tmp3
mv tmp3 code_coverage/nfa.txt

cp ../../regex_v1.3/src/nfa_char_union.cc .
gcov nfa_char_union.cc >> tmp
head -n 3 tmp >> tmp3
rm tmp
cat nfa_char_union.cc.gcov >> tmp3
mv tmp3 code_coverage/nfa_char_union.txt

cp ../../regex_v1.3/src/semantic.cc .
gcov semantic.cc >> semantic_tmp
sed -n '73,75p' semantic_tmp >> tmp3            #输出指定行
cat semantic.cc.gcov >> tmp3
mv tmp3 code_coverage/semantic.txt

cp ../../regex_v1.3/src/charset.cc .
gcov charset.cc >> tmp
head -n 3 tmp >> tmp3
rm tmp
cat charset.cc.gcov >> tmp3
mv tmp3 code_coverage/charset.txt

cp ../../regex_v1.3/src/cJSON.cc .
gcov cJSON.cc >> tmp
head -n 3 tmp >> tmp3
rm tmp
cat cJSON.cc.gcov >> tmp3
mv tmp3 code_coverage/cJSON.txt

cp ../../regex_v1.3/src/expression.cc .
gcov expression.cc >> tmp
head -n 3 tmp >> tmp3
rm tmp
cat expression.cc.gcov >> tmp3
mv tmp3 code_coverage/expression.txt

cp ../../regex_v1.3/src/group.cc .
gcov group.cc >> group_tmp
sed -n '13,15p' group_tmp >> tmp3
cat group.cc.gcov >> tmp3
mv tmp3 code_coverage/group.txt

cp ../../regex_v1.3/src/link.cc .
gcov link.cc >> link_tmp
sed -n '49,51p' link_tmp >> tmp3
cat link.cc.gcov >> tmp3
mv tmp3 code_coverage/link.txt

cp ../../regex_v1.3/src/redis_wrapper.cc .
gcov redis_wrapper.cc >> tmp
head -n 3 tmp >> tmp3
rm tmp
cat redis_wrapper.cc.gcov >> tmp3
mv tmp3 code_coverage/redis_wrapper.txt

cp ../../regex_v1.3/src/state.cc .
gcov state.cc >> state_tmp
sed -n '69,71p' state_tmp >> tmp3
cat state.cc.gcov >> tmp3
mv tmp3 code_coverage/state.txt

cp ../../regex_v1.3/src/utils.cc .
gcov utils.cc >> tmp
head -n 3 tmp >> tmp3
rm tmp
cat utils.cc.gcov >> tmp3
mv tmp3 code_coverage/utils.txt

cp ../../regex_v1.3/src/variable.cc .
gcov variable.cc >> variable_tmp
sed -n '13,15p' variable_tmp >> tmp3
cat variable.cc.gcov >> tmp3
mv tmp3 code_coverage/variable.txt

# 有loginstance
head -n 2 code_coverage/spregex.txt
head -n 2 code_coverage/domain_lex.txt

# 核心代码
head -n 2 code_coverage/nfa.txt

# 其他代码(<100%)
head -n 2 code_coverage/charset.txt
head -n 2 code_coverage/utils.txt
head -n 2 code_coverage/semantic.txt
head -n 2 code_coverage/link.txt
head -n 2 code_coverage/state.txt

# 100%
head -n 2 code_coverage/group.txt
head -n 2 code_coverage/variable.txt
head -n 2 code_coverage/redis_wrapper.txt
head -n 2 code_coverage/nfa_char_union.txt
head -n 2 code_coverage/expression.txt

# 第三方，不需要测
#head -n 2 code_coverage/cJSON.txt           #21.82%


shopt -s extglob
rm *.gcov
rm *.gcda
rm *.gcno
rm a.out
rm *.cc

rm semantic_tmp
rm link_tmp
rm group_tmp
rm state_tmp
rm variable_tmp



