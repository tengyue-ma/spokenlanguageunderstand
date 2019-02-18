/*************************************************************************
	> File Name: PreProcessor.cpp
	> Author: TengyueMa
	> Mail: lumujin@icloud.com
	> Created Time: 一  1/22 20:14:11 2018
 ************************************************************************/

#include <iostream>
#include "PreProcessor.h"
#include "segment_dll.h"
#include "postag_dll.h"
#include "ner_dll.h"
#include "parser_dll.h"
using namespace std;

//< 构造函数
PreProcessor::PreProcessor(){
    //< 初始化为NULL
    _wsg_engine = NULL;
    _pos_engine = NULL;
    _ner_engine = NULL;
    _pas_engine = NULL;
}

//< 析构函数
PreProcessor::~PreProcessor(){
    //< 释放空间
    if (_wsg_engine != NULL){
        segmentor_release_segmentor(_wsg_engine);
    }

    if (_pos_engine != NULL){
        postagger_release_postagger(_pos_engine);
    }

    if (_ner_engine != NULL){
        ner_release_recognizer(_ner_engine);
    }
    
    if (_pas_engine != NULL){
        parser_release_parser(_pas_engine);
    }
}

//< ----------
//< 模型初始化
//< ----------
//< 分词 
//< @param wsg_mdl_path : 模型路径
int PreProcessor::create_wsg_engine(string wsg_mdl_path){
    _wsg_engine = segmentor_create_segmentor(wsg_mdl_path.c_str());
    if (_wsg_engine == NULL){
        return -1;
    }

    return 0;
}

//< 分词 
//< @param wsg_mdl_path : 模型路径
//< @param word_dict_path : 外部词典路径
int PreProcessor::create_wsg_engine(string wsg_mdl_path, 
                                    string word_dict_path){
    _wsg_engine = segmentor_create_segmentor(wsg_mdl_path.c_str(), word_dict_path.c_str());
    if (_wsg_engine == NULL){
        return -1;
    }

    return 0;
}

//< 词性标注
//< @param pos_mdl_path : 模型路径
int PreProcessor::create_pos_engine(string pos_mdl_path){
    _pos_engine = postagger_create_postagger(pos_mdl_path.c_str());
    if (_pos_engine == NULL){
        return -1;
    }

    return 0;
}

//< 词性标注
//< @param pos_mdl_path : 模型路径
//< @param pos_dict_path : 外部词性词典
int PreProcessor::create_pos_engine(string pos_mdl_path, 
                                    string pos_dict_path){
    _pos_engine = postagger_create_postagger(pos_mdl_path.c_str(), pos_dict_path.c_str());
    if (_pos_engine == NULL){
        return -1;
    }

    return 0;
}

//< 命名实体识别
//< @param ner_mdl_path : 模型路径
int PreProcessor::create_ner_engine(string ner_mdl_path){
    _ner_engine = ner_create_recognizer(ner_mdl_path.c_str());
    if (_ner_engine == NULL){
        return -1;
    }

    return 0;
}

//< 依存句法分析
//< @param pas_mdl_path : 模型路径
int PreProcessor::create_pas_engine(string pas_mdl_path){
    _pas_engine = parser_create_parser(pas_mdl_path.c_str());
    if (_pas_engine == NULL){
        return -1;
    }

    return 0;
}

//< --------
//< 模型调用
//< --------
//< 分词
//< @param query : 待分词文本
//< @param ws_vec : 分词结果
int PreProcessor::wordseg(string query, 
                          vector<string>& ws_vec){
    int len = 0;
    len = segmentor_segment(_wsg_engine, query, ws_vec);

    if (len < 0){
        //< wordseg error.
        return -1;
    }

    return 0;
}

//< 词性标注
//< @param ws_vec : 分词结果
//< @param pos_vec : 词性标注结果
int PreProcessor::postag(vector<string>& ws_vec, 
                         vector<string>& pos_vec){
    int ret = 0;
    ret = postagger_postag(_pos_engine, ws_vec, pos_vec);

    if (ret < 0){
        //< postag error.
        return -1;
    }

    return 0;
}

//< 命名实体识别
//< @param ws_vec : 分词结果
//< @param pos_vec : 词性标注结果
//< @param ner_vec : 命名实体识别结果
int PreProcessor::ner(vector<string>& ws_vec, 
                      vector<string>& pos_vec, 
                      vector<string>& ner_vec){
    int ret = 0;
    ret = ner_recognize(_ner_engine, ws_vec, pos_vec, ner_vec);

    if (ret < 0){
        //< ner error.
        return -1;
    }

    return 0;
}

//< 依存句法分析
//< @param ws_vec : 分词结果
//< @param pos_vec : 词性标注结果
//< @param heads_vec : 结果依存弧 heads[i]代表第i个词的父亲节点的编号
//< @param pas_vec : 结果依存弧关系类型
int PreProcessor::parse(vector<string>& ws_vec, 
                        vector<string>& pos_vec, 
                        vector<int>& heads_vec, 
                        vector<string>& pas_vec){
    int ret = 0;
    ret = parser_parse(_pas_engine, ws_vec, pos_vec, heads_vec, pas_vec);

    if (ret < 0){
        //< parser error.
        return -1;
    }

    return 0;
}

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
