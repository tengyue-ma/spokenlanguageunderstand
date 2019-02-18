/*************************************************************************
	> File Name: Feature.h
	> Author: TengyueMa
	> Mail: lumujin@icloud.com
	> Created Time: 五  2/ 2 15:30:20 2018
 ************************************************************************/

#ifndef _FEATURE_H
#define _FEATURE_H

#include <string>
#include <vector>
#include <iostream>

using namespace std;

namespace feature{

class Feature{
    /*
     * @brief 直接用操作符输出
     */
    friend std::ostream& operator<<(std::ostream& out, const Feature& feat);

public:
    /*
     * @brief 构造函数
     */
    Feature();

    /*
     * @brief 析构
     */
    ~Feature();

public:
    /*
     * @brief add feature
     */
    int add_feature(const string feature);
    int add_feature(vector<string> vec);

public:
    //< 特征信息 为了保证可扩展性
    //< 定义所有的特征信息都用vector<string>表示
    //< 分类&解析模块自行 设计对应的分割方法
    //< ME: feature:weight 组
    //< CRF:feature dictfeat dictfeat 组
    vector<string> feats;
};

} //< namespace feature;

#endif
/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
