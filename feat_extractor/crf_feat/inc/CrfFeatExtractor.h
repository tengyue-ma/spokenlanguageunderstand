/*************************************************************************
	> File Name: NerFeatExtractor.h
	> Author: TengyueMa
	> Mail: lumujin@icloud.com
	> Created Time: 一  1/29 15:34:20 2018
 ************************************************************************/

#ifndef _NER_FEATEXTRACTOR_H
#define _NER_FEATEXTRACTOR_H

#include <string>
#include <vector>
#include <map>
#include "utils.h"
#include "Feature.h"
#include "Trie_tree.h"
#include "FeatExtractor.h"
#include "PreProcessor.h"

using namespace std;

namespace feat_extractor{

class CrfFeatExtractor:public FeatExtractor{
public:
    /*
     * @brief 构造函数
     */
    CrfFeatExtractor(const string confpath, 
                     PreProcessor* pre);

    /*
     * @brief 析构函数
     */
    ~CrfFeatExtractor();

public:
    /*
     * @brief load conf
     */
    int init_conf(const string confpath, 
                  PreProcessor* pre);

    /*
     * @brief load dict & build trie tree;
     */
    int load_dict(const string dictname, 
                  const string dictpath);
    int load_dict(const vector<pair<string, string> > dictname_path);

    /*
     * @brief get dict feature
     */
    int get_dict_feature(vector<string>& seq_vec, 
                         const vector<string> dict_name_vec);

    /*
     * @brief 对外接口
     */
    int get_feat(feature::Feature& feat, 
                 const string query);

private:
    /*
     * @brief get feature
     */
    int getfeat_dict(const vector<string> seq_vec, 
                     vector<string>& tags, 
                     TrieTree* trie, 
                     const string featname);

    /*
     * @brief cat feature
     */
    int cat_feat(vector<string>& seq_vec, 
                 const vector<string>& tags);

private:
    //< 特征提取类型
    std::string ext_type;
    vector<string> dictname_vec;
    //< tries
    map<string, TrieTree *> trie_map;
};

} //< namespace feat_extractor;
#endif
/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
