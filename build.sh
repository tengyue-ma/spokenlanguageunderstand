#!/bin/bash
SEMANTIC_WORKDIR=`pwd`


echo -e "\n\n=============开始编译utils模块=============\n"
cd ${SEMANTIC_WORKDIR}/third/utils
./build.sh

echo -e "\n\n=============开始编译processor模块=============\n"
cd ${SEMANTIC_WORKDIR}/third/processor
./build.sh

echo -e "\n\n=============开始编译feat_base模块=============\n"
cd ${SEMANTIC_WORKDIR}/feat_extractor/feat_base
./build.sh

echo -e "\n\n=============开始编译onehot_feat模块=============\n"
cd ${SEMANTIC_WORKDIR}/feat_extractor/onehot_feat/
./build.sh

echo -e "\n\n=============开始编译me_feat模块=============\n"
cd ${SEMANTIC_WORKDIR}/feat_extractor/me_feat
./build.sh

echo -e "\n\n=============开始编译crf_feat模块=============\n"
cd ${SEMANTIC_WORKDIR}/feat_extractor/crf_feat
./build.sh

echo -e "\n\n=============开始编译calculator_feat模块============="
cd ${SEMANTIC_WORKDIR}/feat_extractor/calculator_feat
./build.sh

echo -e "\n\n=============开始编译open_domain_feat模块============="
cd ${SEMANTIC_WORKDIR}/feat_extractor/open_domain_feat
./build.sh

echo -e "\n\n=============开始编译clf_base模块=============\n"
cd ${SEMANTIC_WORKDIR}/classifier/clf_base
./build.sh

echo -e "\n\n=============开始编译clf_nn模块=============\n"
cd ${SEMANTIC_WORKDIR}/classifier/clf_nn
./build.sh

echo -e "\n\n=============开始编译ME模块=============\n"
cd ${SEMANTIC_WORKDIR}/classifier/ME
./build.sh

echo -e "\n\n=============开始编译intent_inform模块=============\n"
cd ${SEMANTIC_WORKDIR}/classifier/intent_inform
./build.sh

echo -e "\n\n=============开始编译ner_base模块=============\n"
cd ${SEMANTIC_WORKDIR}/slot_filling/ner_base
./build.sh

echo -e "\n\n=============开始编译CRF模块=============\n"
cd ${SEMANTIC_WORKDIR}/slot_filling/CRF
./build.sh

echo -e "\n\n=============开始编译calculator模块=============\n"
cd ${SEMANTIC_WORKDIR}/slot_filling/calculator
./build.sh

echo -e "\n\n=============开始编译open_domain模块=============\n"
cd ${SEMANTIC_WORKDIR}/slot_filling/open_domain
./build.sh

echo -e "\n\n=============开始编译regex模块=============\n"
cd ${SEMANTIC_WORKDIR}/regex_v1.3
./build.sh

echo -e "\n\n=============开始编译work_flow模块=============\n"
cd ${SEMANTIC_WORKDIR}/work_flow
./build.sh

echo -e "\n\n=============开始编译CRF训练模块==============\n"
cd ${SEMANTIC_WORKDIR}/train_model/crf_train
./build.sh

echo -e "\n\n=============编译结束!======请检查每个模块是否有报错!=============\n\n"


