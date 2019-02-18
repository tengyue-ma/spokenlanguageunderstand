/*************************************************************************
	> File Name: OpenDomainFeatExtractor.h
	> Author: TengyueMa
	> Mail: lumujin@icloud.com
	> Created Time: Mon 19 Mar 2018 11:47:20 AM CST
 ************************************************************************/

#ifndef _OPENDOMAINFEATEXTRACTOR_H
#define _OPENDOMAINFEATEXTRACTOR_H

#include <string>
#include <vector>
#include "Feature.h"
#include "PreProcessor.h"
#include "FeatExtractor.h"
#include "slotType.h"

using std::vector;
using std::string;
using namespace feature;

namespace feat_extractor{

class OpenDomainFeatExtractor:public FeatExtractor{
public:
    /*
     * @brief 构造函数
     */
    OpenDomainFeatExtractor(const string confpath, 
                            PreProcessor* pre);

    /*
     * @brief 析构函数
     */
    ~OpenDomainFeatExtractor();

public:
    /*
     * @brief 初始化ltp
     */
    int init_ltp(PreProcessor* pre);

    /*
     * @brief 初始化conf
     */
    int init_conf(const string confpath, 
                  PreProcessor* pre);

    /*
     * @brief 初始化slot type
     */
    int init_slot_type();

    /*
     * @brief get feat
     */
    int get_feat(feature::Feature& feats, 
                 const string query);

private:
    /*
     * @brief res push to feat list
     */
    int pushfeat(feature::Feature& feats, 
                 string default_name, 
                 string value);

public:
    //< 特征提取类型
    string ext_type;

private:
    //< ltp
    //< 分词&词性标注&命名实体识别&句法分析
    PreProcessor* pre;

    //< slot type
    slotType* _slot_type;

    /*
     * @brief 相关的配置信息
     */
    //< slot_name;
    //< 人名
    string nh;
    //< 地名
    string ns;
    //< 机构名
    string ni;
    //< 人称代词
    string rh;
    //< 地点代词
    string rs;

    //< 人称&地点代词关键词(量小没用map)
    vector<string> rh_words;
    vector<string> rs_words;

    //< slotname&词典地址
    vector<pair<string, string> > vec_dict;
};

} //< namespace feat_extractor;

#endif
/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
