/*************************************************************************
	> File Name: FeatExtractor.cpp
	> Author: TengyueMa
	> Mail: lumujin@icloud.com
	> Created Time: 五  2/ 2 13:59:24 2018
 ************************************************************************/

#include <iostream>
#include "FeatExtractor.h"
using namespace std;

namespace feat_extractor{

int FeatExtractor::get_feat(feature::Feature& feat, const string query){
    feat.add_feature(query);
    return 0;
}

} //< namespace feat_extractor;

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
