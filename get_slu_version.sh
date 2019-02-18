#!/bin/bash

echo -e "=============将版本所需资源输出到version目录!=============\n"

SLU_VERSION=slu2.2.5

rm ${SLU_VERSION} -rf
mkdir -p ${SLU_VERSION}

shopt -s extglob
SEMANTIC_WORKDIR=`pwd`
OUTPUT_DIR=${SEMANTIC_WORKDIR}/${SLU_VERSION}

# 所有的目录
INC_SLU=${OUTPUT_DIR}/inc/slu/
NLP_DATA_SLU=${OUTPUT_DIR}/nlp_data/slu/
NLP_DATA_SLU_LIBS=${OUTPUT_DIR}/nlp_data/slu/libs/
LIBS=${OUTPUT_DIR}/libs

echo -e "=============1、创建version目录及其子目录……=============\n"
mkdir -p ${SLU_VERSION}
cd ${SLU_VERSION} 

mkdir -p inc/slu
mkdir -p nlp_data/slu/
mkdir -p nlp_data/slu/libs
mkdir -p libs/

cd ${SEMANTIC_WORKDIR}
echo -e "=============2、拷贝inc/slu里面的头文件……=============\n"
cd classifier
cp clf_base/inc/ ${INC_SLU} -rf --parents
cd ../feat_extractor
cp feat_base/inc/ ${INC_SLU} -rf --parents
cd ../slot_filling
cp ner_base/inc/ ${INC_SLU} -rf --parents
cd ../
cp regex_v1.3/inc/ ${INC_SLU} -rf --parents
cp slu_log/inc/ ${INC_SLU} -rf --parents
cp work_flow/inc/ ${INC_SLU} -rf --parents
cp regex_v1.3/third/hiredis/*.h ${INC_SLU} -rf --parents
cp regex_v1.3/third/hiredis/adapters/*.h ${INC_SLU} -rf --parents
cp regex_v1.3/third/hiredis/examples/*.h ${INC_SLU} -rf --parents

echo -e "=============3、拷贝inc/slu/third/里面的头文件和库……=============\n"
cp third/basic_lib/configure/include/ ${INC_SLU} -rf --parents
cp third/basic_lib/lib2-64/bsl/include/ ${INC_SLU} -rf --parents
cp third/basic_lib/lib2-64/spreg/include/ ${INC_SLU} -rf --parents
cp third/basic_lib/lib2-64/ullib/include/ ${INC_SLU} -rf --parents
cp third/crf/include/ ${INC_SLU} -rf --parents
cp third/hobotlog/include/ ${INC_SLU} -rf --parents
cp third/ltp-3.4.0/include/ ${INC_SLU} -rf --parents
cp third/processor/inc/ ${INC_SLU} -rf --parents
cp third/utils/inc/ ${INC_SLU} -rf --parents

echo -e "=============4、拷贝nlp_data/slu/里面的资源……=============\n"
cp data/conf ${NLP_DATA_SLU} -rf
cp data/dict ${NLP_DATA_SLU} -rf
cp data/model ${NLP_DATA_SLU} -rf

echo -e "=============5、拷贝nlp_data/slu/libs/, 需要dlopen打开的动态库……=============\n"
cp feat_extractor/calculator_feat/lib/libcalculator_feat.so ${NLP_DATA_SLU_LIBS} -rf
cp feat_extractor/crf_feat/lib/libcrffeat.so ${NLP_DATA_SLU_LIBS} -rf
cp feat_extractor/me_feat/lib/libmefeat.so ${NLP_DATA_SLU_LIBS} -rf
cp feat_extractor/onehot_feat/lib/libonehotfeat.so ${NLP_DATA_SLU_LIBS} -rf
cp feat_extractor/open_domain_feat/lib/libopendomain_feat.so ${NLP_DATA_SLU_LIBS} -rf

cp classifier/intent_inform/lib/libintent.so ${NLP_DATA_SLU_LIBS} -rf
cp classifier/ME/lib/libme.so ${NLP_DATA_SLU_LIBS} -rf
cp classifier/clf_nn/lib/libnn.so ${NLP_DATA_SLU_LIBS} -rf

cp slot_filling/calculator/lib/libcalculator.so ${NLP_DATA_SLU_LIBS} -rf
cp slot_filling/CRF/lib/libcrf.so ${NLP_DATA_SLU_LIBS} -rf
cp slot_filling/ner_base/lib/libnerbase.so ${NLP_DATA_SLU_LIBS} -rf
cp slot_filling/open_domain/lib/libopendomain.so ${NLP_DATA_SLU_LIBS} -rf

echo -e "=============6、拷贝最外层的libs/……=============\n"
cp third/basic_lib/lib2-64/ullib/lib/* ${LIBS} -rf
cp third/basic_lib/lib2-64/bsl/lib/* ${LIBS} -rf
cp third/basic_lib/lib2-64/spreg/lib/* ${LIBS} -rf
cp third/basic_lib/configure/lib/* ${LIBS} -rf
cp third/hobotlog/lib/* ${LIBS} -rf
cp third/crf/lib/* ${LIBS} -rf
cp third/processor/lib/* ${LIBS} -rf
cp third/utils/lib/* ${LIBS} -rf
cp third/ltp-3.4.0/lib/* ${LIBS} -rf
cp regex_v1.3/third/hiredis/libhiredis.* ${LIBS} -rf
cp regex_v1.3/lib/* ${LIBS} -rf
cp work_flow/lib/* ${LIBS} -rf

cp feat_extractor/feat_base/lib/* ${LIBS} -rf
cp slot_filling/ner_base/lib/* ${LIBS} -rf
cp classifier/clf_base/lib/* ${LIBS} -rf
cp classifier/clf_nn/tensorflow/lib/* ${LIBS} -rf

echo -e "=============7、批量修改nlp_data/slu/conf里面的配置, “../../data/”和“../data/”一律修改为 “../nlp_data/slu/”……=============\n"
cd ${NLP_DATA_SLU}/conf
sed -i "s/..\/..\/data\//..\/nlp_data\/slu\//g" `grep "../../data/" -rl ./`
sed -i "s/..\/data\//..\/nlp_data\/slu\//g" `grep "../data/" -rl ./`

echo -e "=============8、批量修改resource.conf里面*.so的路径”……=============\n"
# ../feat_extractor/crf_feat/lib/
# ../feat_extractor/me_feat/lib/
# ../feat_extractor/onehot_feat/lib/ 
# ../feat_extractor/open_domain_feat/lib/
# ../classifier/ME/lib/
# ../classifier/clf_nn/lib/
# ../classifier/intent_inform/lib/
# ../slot_filling/open_domain/lib/
# ../slot_filling/CRF/lib/
#     
# ../nlp_data/slu/libs/
sed -i "s/..\/feat_extractor\/crf_feat\/lib\//..\/nlp_data\/slu\/libs\//g" resource.conf
sed -i "s/..\/feat_extractor\/me_feat\/lib\//..\/nlp_data\/slu\/libs\//g" resource.conf
sed -i "s/..\/feat_extractor\/onehot_feat\/lib\//..\/nlp_data\/slu\/libs\//g" resource.conf
sed -i "s/..\/feat_extractor\/open_domain_feat\/lib\//..\/nlp_data\/slu\/libs\//g" resource.conf
sed -i "s/..\/classifier\/ME\/lib\//..\/nlp_data\/slu\/libs\//g" resource.conf
sed -i "s/..\/classifier\/clf_nn\/lib\//..\/nlp_data\/slu\/libs\//g" resource.conf
sed -i "s/..\/classifier\/intent_inform\/lib\//..\/nlp_data\/slu\/libs\//g" resource.conf
sed -i "s/..\/slot_filling\/open_domain\/lib\//..\/nlp_data\/slu\/libs\//g" resource.conf
sed -i "s/..\/slot_filling\/CRF\/lib\//..\/nlp_data\/slu\/libs\//g" resource.conf


