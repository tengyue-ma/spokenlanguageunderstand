/*************************************************************************
	> File Name: intent_inform.h
	> Author: TengyueMa
	> Mail: lumujin@icloud.com
	> Created Time: Mon 02 Apr 2018 04:38:28 PM CST
 ************************************************************************/

#ifndef _INTENT_INFORM_H
#define _INTENT_INFORM_H

#include "Classifier.h"

namespace classify{

class Intent_inform:public Classifier{
public:
    Intent_inform(const string model_path, 
                  const string log_path){
        //< 假的应用
        this->intent = model_path;
    };

    ~Intent_inform(){};

    /*
     * @brief 返回默认intent信息
     * @param query 待分类的query信息 在这里没用到
     * @param class_name intent结果
     * @param feat_ext 特征提取器 在这里没用到
     */
    int classify(const string query, 
                 vector<std::pair<string, float> >& class_name, 
                 feat_extractor::FeatExtractor* feat_ext);

private:
    //< 默认intent
    string intent;
};

} // namespace ciIlassify

#endif
