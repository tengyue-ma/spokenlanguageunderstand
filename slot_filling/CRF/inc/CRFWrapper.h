/*************************************************************************
	> File Name: CRFWrapper.h
	> Author: TengyueMa
	> Mail: lumujin@icloud.com
	> Created Time: 一  1/29 12:56:17 2018
 ************************************************************************/

#ifndef _CRF_WRAPPER_H
#define _CRF_WRAPPER_H

#include "crfpp.h"

#include <string>
#include <vector>
#include <map>

using namespace std;

namespace slot_filling{

class CRFWrapper{
public:
    /*
     * @brief 构造函数
     */
    CRFWrapper();

    /*
     * @brief 析构函数
     */
    ~CRFWrapper();

public:
    /*
     * @brief load model wrapper. 
     * @param filename:模型路径
     */
    int load_model(const std::string& filename);

    /*
     * @brief 返回model
     */
    inline CRFPP::Model* get_crf_model(){
        return p_crf_model_;
    }

private:
    CRFPP::Model* p_crf_model_;
};

} //< namespace slot_filling
#endif
/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
