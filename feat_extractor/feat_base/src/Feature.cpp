/*************************************************************************
	> File Name: Feature.cpp
	> Author: TengyueMa
	> Mail: lumujin@icloud.com
	> Created Time: 五  2/ 2 15:56:52 2018
 ************************************************************************/

#include <iostream>
#include "Feature.h"

using namespace std;

namespace feature{

//< 构造函数
Feature::Feature(){
}

//< 析构函数
Feature::~Feature(){
}

//< add feature
int Feature::add_feature(const string feature){
    feats.push_back(feature);

    return 0;
}

int Feature::add_feature(vector<string> vec){
    for (size_t i = 0; i < vec.size(); i++){
        if (add_feature(vec[i]) != 0){
            //< 返回error;
            
            return -1;
        }
    }

    return 0;
}

//< 打印Feature信息
ostream& operator<<(ostream& out, const Feature& feat){
    for (size_t i = 0; i < feat.feats.size(); i++){
        out << feat.feats[i] << endl;
    }

    return out;
}

}//< namespace feature;

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
