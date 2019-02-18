/*************************************************************************
	> File Name: crf_wrapper.cpp
	> Author: TengyueMa
	> Mail: lumujin@icloud.com
	> Created Time: 一  1/29 13:01:39 2018
 ************************************************************************/

#include <iostream>
#include <string>
#include <vector>
#include "CRFWrapper.h"
#include "CrfErrorCode.h"
#include "SluLog.h"

using namespace std;
using namespace hobotlog;

//< log定义
LogModule * loginstance = NULL;

namespace slot_filling{

//< 构造函数
CRFWrapper::CRFWrapper(){
    this->p_crf_model_ = NULL;
}

//< 析构函数
CRFWrapper::~CRFWrapper(){
    if (!p_crf_model_){
        return;
    }
    delete p_crf_model_;
}

//< load model wrapper.
int CRFWrapper::load_model(const string& filename){
    string params = "";
    params.append("-m ");
    params.append(filename);
    params.append(" -v 3");
    p_crf_model_ = CRFPP::createModel(params.c_str());

    if (!p_crf_model_){
		loginstance->error("[SLU]: failed to create crf model!");
        return ERR_CRF_CREAT_CRF_MODEL;
    }
	
	loginstance->info("[SLU]: create crf model success!");
	
    return 0;
}

} //< namespace slot_filling
/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
