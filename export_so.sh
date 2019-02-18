#!/bin/bash
SEMANTIC_WORKDIR=`pwd`
#basic depend libs
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:${SEMANTIC_WORKDIR}/third/ltp-3.4.0/lib/
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:${SEMANTIC_WORKDIR}/third/processor/lib/
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:${SEMANTIC_WORKDIR}/third/utils/lib/
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:${SEMANTIC_WORKDIR}/third/hobotlog/lib/
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:${SEMANTIC_WORKDIR}/feat_extractor/feat_base/lib
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:${SEMANTIC_WORKDIR}/feat_extractor/me_feat/lib
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:${SEMANTIC_WORKDIR}/feat_extractor/crf_feat/lib
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:${SEMANTIC_WORKDIR}/feat_extractor/onehot_feat/lib
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:${SEMANTIC_WORKDIR}/feat_extractor/calculator_feat/lib
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:${SEMANTIC_WORKDIR}/feat_extractor/open_domain_feat/lib
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:${SEMANTIC_WORKDIR}/classifier/clf_base/lib
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:${SEMANTIC_WORKDIR}/classifier/ME/lib
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:${SEMANTIC_WORKDIR}/classifier/intent_inform/lib
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:${SEMANTIC_WORKDIR}/classifier/clf_nn/lib/
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:${SEMANTIC_WORKDIR}/slot_filling/ner_base/lib
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:${SEMANTIC_WORKDIR}/slot_filling/CRF/lib
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:${SEMANTIC_WORKDIR}/slot_filling/calculator/lib
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:${SEMANTIC_WORKDIR}/slot_filling/open_domain/lib
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:${SEMANTIC_WORKDIR}/regex_v1.3/lib/
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:${SEMANTIC_WORKDIR}/regex_v1.3/third/hiredis
