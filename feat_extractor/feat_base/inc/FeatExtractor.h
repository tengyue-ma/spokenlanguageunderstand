/*************************************************************************
	> File Name: FeatExtractor.h
	> Author: TengyueMa
	> Mail: lumujin@icloud.com
	> Created Time: 五  2/ 2 13:09:36 2018
 ************************************************************************/

#ifndef _FEATEXTRACTOR_H
#define _FEATEXTRACTOR_H

#include <string>
#include <vector>
#include "Feature.h"
#include "PreProcessor.h"

namespace feat_extractor{

class FeatExtractor{
public:
    /*
     * @brief 纯虚函数析构
     */
    virtual ~FeatExtractor(){};

    /*
     * @brief 初始化
     * @param confpath : 对应的配置文件 
     * @param pre : ltp接口
     */
    virtual int init_conf(const string confpath, 
                          PreProcessor* pre) = 0;
    
    /*
     * @brief 对外接口
     * @param feat : 要返回的特征信息
     * @param query : 待处理的query
     */
    virtual int get_feat(feature::Feature& feat, 
                         const string query) = 0;

public:
    //< 特征提取类型
    std::string ext_type;
};

} //< namespace feat_extractor

#endif
/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
