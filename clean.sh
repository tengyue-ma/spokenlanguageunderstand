#!/bin/bash
SEMANTIC_WORKDIR=`pwd`

echo -e "\n\n=============清除utils模块============="
cd ${SEMANTIC_WORKDIR}/third/utils
./clean.sh

echo -e "\n\n=============清除processor模块============="
cd ${SEMANTIC_WORKDIR}/third/processor
./clean.sh

echo -e "\n\n=============清除feat_base模块=============\n"
cd ${SEMANTIC_WORKDIR}/feat_extractor/feat_base
./clean.sh

echo -e "\n\n=============清除me_feat模块============="
cd ${SEMANTIC_WORKDIR}/feat_extractor/me_feat
./clean.sh

echo -e "\n\n=============清除onehot_feat模块============="
cd ${SEMANTIC_WORKDIR}/feat_extractor/onehot_feat
./clean.sh

echo -e "\n\n=============清除crf_feat模块============="
cd ${SEMANTIC_WORKDIR}/feat_extractor/crf_feat
./clean.sh

echo -e "\n\n=============清除calculator_feat模块============="
cd ${SEMANTIC_WORKDIR}/feat_extractor/calculator_feat
./clean.sh

echo -e "\n\n=============清除open_domain_feat模块============="
cd ${SEMANTIC_WORKDIR}/feat_extractor/open_domain_feat
./clean.sh

echo -e "\n\n=============清除clf_base模块=============\n"
cd ${SEMANTIC_WORKDIR}/classifier/clf_base
./clean.sh

echo -e "\n\n=============清除clf_nn模块=============\n"
cd ${SEMANTIC_WORKDIR}/classifier/clf_nn
./clean.sh

echo -e "\n\n=============清除ME模块============="
cd ${SEMANTIC_WORKDIR}/classifier/ME
./clean.sh

echo -e "\n\n=============清除intent_inform模块============="
cd ${SEMANTIC_WORKDIR}/classifier/intent_inform
./clean.sh

echo -e "\n\n=============清除ner_base模块=============\n"
cd ${SEMANTIC_WORKDIR}/slot_filling/ner_base
./clean.sh

echo -e "\n\n=============清除CRF模块============="
cd ${SEMANTIC_WORKDIR}/slot_filling/CRF
./clean.sh

echo -e "\n\n=============清除calculator模块=============\n"
cd ${SEMANTIC_WORKDIR}/slot_filling/calculator
./clean.sh

echo -e "\n\n=============清除open_domain模块=============\n"
cd ${SEMANTIC_WORKDIR}/slot_filling/open_domain
./clean.sh

echo -e "\n\n=============清除regex模块=============\n"
cd ${SEMANTIC_WORKDIR}/regex_v1.3
./clean.sh

echo -e "\n\n=============清除work_flow模块============="
cd ${SEMANTIC_WORKDIR}/work_flow
./clean.sh

echo -e "\n\n=============清除crf训练模块============="
cd ${SEMANTIC_WORKDIR}/train_model/crf_train
./clean.sh

echo -e "\n\n=============删除output文件夹============="
cd ${SEMANTIC_WORKDIR}
rm output -rf


echo -e "\n=============清除结束=============\n\n"


