/*************************************************************************
	> File Name: MEFeatExtractor.h
	> Author: TengyueMa
	> Mail: lumujin@icloud.com
	> Created Time: 一  1/22 13:30:54 2018
 ************************************************************************/

#ifndef _ME_FEATEXTRACTOR_H
#define _ME_FEATEXTRACTOR_H

#include <string>
#include <vector>
#include "Feature.h"
#include "PreProcessor.h"
#include "FeatExtractor.h"

using std::vector;
using std::string;
using namespace feature;

namespace feat_extractor{

class MEFeatExtractor:public FeatExtractor{
public:
    /*
     * @brief 构造函数
     * @param confpath : 配置文件路径
     * @param pre : ltp接口
     */
    MEFeatExtractor(const string confpath, 
                    PreProcessor* pre);

    /*
     * @brief 析构函数
     */
    ~MEFeatExtractor();

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
     * @brief get feat
     */
    int get_feat(feature::Feature& feats, 
                 const string query);

private:
    /*
     * @brief 分词
     * @param query : 待分词句子
     * @param ws_vec : 分词结果
     */
    int get_wsg(string query, 
                vector<string>& ws_vec);

    /*
     * @brief 单字切分
     * @param query : 待切分句子
     * @param sip_vec : 切分结果
     */
    int get_sip(string query, 
                vector<string>& sip_vec);

    /*
     * @brief get ngram
     * @param start - end n长度范围
     */
    int get_ngram(vector<string>& res, 
                  vector<string> ws_vec, 
                  const size_t start, 
                  const size_t end);

    /*
     * @brief get query;
     */
    string get_query(string query);

    /*
     * @brief get pattern
     */
    vector<string> get_pattern(string query);

public:
    //< 特征提取类型
    string ext_type;

private:
    //< ltp
    //< 分词&词性标注&命名实体识别&句法分析
    PreProcessor* pre; 
    /*
     * @param use_q2feat:query特征(默认使用)
     * @param use_p2feat:pattern特征(默认不使用)
     * @param use_wsg:是否分词(默认为单字)
     * @param start & end:n-gram start-end(默认为1-1)
     */
    bool use_q2feat;
    bool use_p2feat;
    bool use_wsg;
    size_t start;
    size_t end;
};

} //< namespace feat_extractor;
#endif
/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
