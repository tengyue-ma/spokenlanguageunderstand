/*************************************************************************
	> File Name: NerFeatExtractor.cpp
	> Author: TengyueMa
	> Mail: lumujin@icloud.com
	> Created Time: 一  1/29 15:46:02 2018
 ************************************************************************/

#include <iostream>
#include "utils.h"
#include "PreProcessor.h"
#include "CrfFeatExtractor.h"
#include "CrfFeatErrorCode.h"
#include "ul_log.h"
#include "Configure.h"
#include "spreg.h"
#include "SluLog.h"

using namespace std;
using namespace utils;

LogModule * loginstance;

namespace feat_extractor{

extern "C" FeatExtractor* create_feat_extr(const string confpath, 
                                           PreProcessor* pre){
    if (pre == NULL){
        //< nothing;
    }
    CrfFeatExtractor* crf_feat_extr = new CrfFeatExtractor(confpath, pre);
    return dynamic_cast<FeatExtractor *>(crf_feat_extr);
}

extern "C" void delete_feat_extr(FeatExtractor* feat_extr){
    CrfFeatExtractor* crf_feat_extr = dynamic_cast<CrfFeatExtractor *>(feat_extr);
    //delete crf_feat_extr;
    crf_feat_extr = NULL;
}

//< 构造函数
CrfFeatExtractor::CrfFeatExtractor(const string confpath, 
                                   PreProcessor* pre){
    int ret = init_conf(confpath, pre);
    if (ret < 0){
        cout << "failed to load crf feat config file in" << confpath << endl;
        exit(-1);
    }
}

//< load conf
int CrfFeatExtractor::init_conf(const string confpath, 
                                PreProcessor* pre){
    //< not use
    if (pre == NULL){
        cout << "pre == NULL" << endl;
    }
    comcfg::Configure config;
    int ret = config.load("./", confpath.c_str());
    if (ret < 0){
        cout << "failed to load crf feat config file in " << confpath << endl;
        return ERR_CRFFEAT_LOAD_CONFIG;
    }

    //init log
    try {
        string log_file = config["CRF_FEAT_LOG_CONF"].to_cstr();
		if(NULL == loginstance){
			loginstance = new hobotlog::LogModule(log_file);
			loginstance->info("[SLU]: load crf feat log conf,and init loginstance success!");
		}
    }catch(...){
        cout << "do not set log file in crf feat conf: " << confpath << endl;
        return ERR_CRFFEAT_NOT_SET_LOG_FILE;
    }

    //< load
    vector<pair<string, string> > dictlist;

    try {
        int dict_size;
        try {
            dict_size = static_cast<int>(config["DICT_FEATURE"].size());
        } catch(...){
            dict_size = 0;
        }

        for (int i = 0; i < dict_size; i++){
            string dict_name = config["DICT_FEATURE"][i]["DICT_NAME"].to_cstr();
            string dict_path = config["DICT_FEATURE"][i]["DICT_PATH"].to_cstr();
            dictlist.push_back(pair<string, string>(dict_name, dict_path));
            dictname_vec.push_back(dict_name);
        }
    } catch(...) {
        loginstance->error("[SLU]: failed to load crf feat config file: " + confpath + ", conf content is wrong");
        return ERR_CRFEAT_CONFIG_CONTENT;
    }

    loginstance->info("\n[SLU]: load crf feat config file success！config name:" + confpath);
    
    ret = load_dict(dictlist);
    if(ret < 0) {
        loginstance->error("[SLU]: failed to load crf feat dict");
        return ret;
    }

    return 0;
}

//< 析构函数
CrfFeatExtractor::~CrfFeatExtractor(){
    
}

//< 对外接口
int CrfFeatExtractor::get_feat(feature::Feature& feat, 
                               const string query){
    vector<string> encode_label;
    string query_c = query;
    //< 编码
    encode(query_c, encode_label);
    vector<string> seq_vec;
    strSplit2simple(query_c, seq_vec);
    get_dict_feature(seq_vec, dictname_vec);
   
    //< cat特征提取结果和编码结果
    int ret = cat_feat(seq_vec, encode_label);
    if (ret < 0){
        loginstance->error("[SLU]: failed to cat crf feat!");
        return ret;
    }
    
    feat.feats = seq_vec;

    return 0;
}

//< load dict & build trie tree
int CrfFeatExtractor::load_dict(const string dictname, 
                                const string dictpath){
    TrieTree* trie = new TrieTree(dictpath);
    trie_map[dictname] = trie;

    return 0;
}

int CrfFeatExtractor::load_dict(const vector<pair<string, string> > dictname_path){
    for (size_t i = 0; i < dictname_path.size(); i++){
        load_dict(dictname_path[i].first, dictname_path[i].second);
    }

    return 0;
}

//< get dict feature
//< @param seq_vec : 分割后的字符串 & 返回特征结果
//< 待匹配的特征辞典名称
int CrfFeatExtractor::get_dict_feature(vector<string>& seq_vec, 
                                       const vector<string> dict_name_vec){
    int ret = 0;
    vector<string> cha_vec = seq_vec;
    size_t count = dict_name_vec.size();
    if (count == 0){
        vector<string> tags;
        tags.clear();
        tags.resize(seq_vec.size(), "O");
        ret = cat_feat(seq_vec, tags);
        if (ret < 0){
            loginstance->error("[SLU]: failed to cat crf feat!");
            return ret;
        }
        return 0;
    }

    for (size_t i = 0; i < count; i++){
        //< 匹配
        vector<string> tags;
        ret = getfeat_dict(cha_vec, tags, trie_map[dict_name_vec[i]], dict_name_vec[i]);
        if(ret < 0){
            loginstance->error("[SLU]: failed to get crf dict feat!");
            return ret;
        }

        //< cat dictfeature
        ret = cat_feat(seq_vec, tags);
        if (ret < 0){
            loginstance->error("[SLU]: failed to cat crf feat!");
            return ret;
        }
    }

    return 0;
}

//< 获取指定辞典的特征
int CrfFeatExtractor::getfeat_dict(const vector<string> seq_vec, 
                                   vector<string>& tags, 
                                   TrieTree* trie, 
                                   const string featname){
    int length = int(seq_vec.size());
    tags.clear();
    tags.resize(length, "O");

    for (int i = 0; i < length; i++){
        int len = trie->match(seq_vec, i);
        if (len <= 0){
            continue;
        }
        if (tags[i] != "O" || tags[i+len-1] != "O"){
            continue;    
        }
        for (int j = 0; j < len; j++){
            tags[i+j] = featname + "+i";
            if (j == 0){
                tags[i+j] = featname + "+b";
            }
            if (j == len-1){
                tags[i+j] = featname + "+e";
            }
            if (len == 1){
                tags[i+j] = featname + "+s";
            }
        }
        i += len-1;
    }

    return 0;
}

//< cat feature
int CrfFeatExtractor::cat_feat(vector<string>& seq_vec, 
                               const vector<string>& tags){
    int ret = seq_vec.size() != tags.size();
    if (ret < 0){
        loginstance->error("[SLU]: failed to cat crf feat!");
        return ERR_CRFFEAT_SEQ_TAGS_UNEQUAL;
    }
    size_t count = seq_vec.size();
    for (size_t i = 0; i < count; i++){
        seq_vec[i] += " " + tags[i];
    }

    return 0;
}

//< 真实标注编码
int CrfFeatExtractor::encode(string& query, 
                             vector<string>& encode_label){
    bool flag = true;
    string query_no_label;
    string value;
    string label;
    vector<string> query_vec;
    strSplit2simple(query, query_vec);
    for (size_t i = 0; i < query_vec.size(); i++){
        if (flag && query_vec[i] == "["){
            flag = false;
        }
        else if (!flag && query_vec[i] == ":"){
            //< idx
            size_t idx = i;
            //< 获取label
            for (size_t j = i+1; j < query_vec.size(); j++){
                if (query_vec[j] == "]"){
                    idx = j;
                    break;
                }
                else {
                    label += query_vec[j];
                }
            }
            //< 编码
            vector<string> seq_vec;
            strSplit2simple(value, seq_vec);
            for (size_t j = 0; j < seq_vec.size(); j++){
                if (j == 0){
                    encode_label.push_back(label+"+b");
                }
                else if (j == seq_vec.size()-1){
                    encode_label.push_back(label+"+e");
                }
                else {
                    encode_label.push_back(label+"+i");
                }
            }
            //< query_no_label 赋值
            query_no_label += value;
            //< value&label 清空
            value = "";
            label = "";

            //< 更新下标位置
            i = idx;
            flag = true;
        }
        else if (!flag && query_vec[i] != ":"){
            value += query_vec[i];
        }
        else {
            query_no_label += query_vec[i];
            encode_label.push_back("O");
        }
    }
    query = query_no_label;

    return 0;
}

} //< namespace slot_filling

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
