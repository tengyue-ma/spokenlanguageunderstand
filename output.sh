#!/bin/bash

echo -e "\n\n=============将所有.so、.a和.h输出到output目录!=============\n\n"

shopt -s extglob
SEMANTIC_WORKDIR=`pwd`
OUTPUT_DIR=${SEMANTIC_WORKDIR}/output

#1、创建output目录
mkdir -p output

#2、拷贝当前文件夹下所有东西(除了output), 到output目录
cp !(output) output -rf

#3、删除掉output文件夹里面，除了include/inc/lib以外，所有的文件

cd ${OUTPUT_DIR}
rm -rf !(classifier|data|feat_extractor|regex_v1.3|slot_filling|slu_log|third|work_flow)

cd ${OUTPUT_DIR}/classifier/clf_base
rm -rf !(lib|inc)
cd ${OUTPUT_DIR}/classifier/intent_inform
rm -rf !(lib|inc)
cd ${OUTPUT_DIR}/classifier/ME
rm -rf !(lib|inc)

cd ${OUTPUT_DIR}/feat_extractor/calculator_feat
rm -rf !(lib|inc)
cd ${OUTPUT_DIR}/feat_extractor/crf_feat
rm -rf !(lib|inc)
cd ${OUTPUT_DIR}/feat_extractor/feat_base
rm -rf !(lib|inc)
cd ${OUTPUT_DIR}/feat_extractor/me_feat
rm -rf !(lib|inc)
cd ${OUTPUT_DIR}/feat_extractor/open_domain_feat
rm -rf !(lib|inc)

cd ${OUTPUT_DIR}/regex_v1.3
rm -rf !(lib|inc|third)

cd ${OUTPUT_DIR}/slot_filling/calculator
rm -rf !(lib|inc)
cd ${OUTPUT_DIR}/slot_filling/CRF
rm -rf !(lib|inc)
cd ${OUTPUT_DIR}/slot_filling/ner_base
rm -rf !(lib|inc)
cd ${OUTPUT_DIR}/slot_filling/open_domain
rm -rf !(lib|inc)

cd ${OUTPUT_DIR}/slu_log
rm -rf !(lib|inc)

cd ${OUTPUT_DIR}/third/basic_lib/configure
rm -rf !(lib|include)

cd ${OUTPUT_DIR}/third/hobotlog
rm -rf !(lib|include)

cd ${OUTPUT_DIR}/third/processor
rm -rf !(lib|inc)

cd ${OUTPUT_DIR}/third/utils
rm -rf !(lib|inc)

cd ${OUTPUT_DIR}/work_flow
rm -rf !(lib|inc)



