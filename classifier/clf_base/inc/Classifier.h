/*************************************************************************
	> File Name: Classifier.h
	> Author: TengyueMa
	> Mail: lumujin@icloud.com
	> Created Time: 五  2/ 2 12:43:17 2018
 ************************************************************************/

#ifndef _CLASSIFIER_H
#define _CLASSIFIER_H

#include <string>
#include <vector>
#include "FeatExtractor.h"

using std::vector;
using std::string;

namespace classify{

class Classifier{
public:
    /*
     * @brief 析构
     */ 
    virtual ~Classifier(){}

    /*
     * @brief 分类
     * @param query : 待分类数据
     * @param feat_ext : 特征提取方式
     * @param class_name : 分类n-best结果
     */
    virtual int classify(const string query,
                         vector<std::pair<string, float> >& class_name,
                         feat_extractor::FeatExtractor* feat_ext) = 0;

public:
    //< 分类器类型
    string clf_type;
};

} //< namespace classify

#endif
/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
