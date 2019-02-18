/*************************************************************************
	> File Name: OneHotFeatExtractor.cpp
	> Author: TengyueMa
	> Mail: lumujin@icloud.com
	> Created Time: Thu 08 Nov 2018 01:45:32 PM CST
 ************************************************************************/

#include <iostream>
#include <string>
#include <vector>
#include <stdlib.h>
#include <fstream>
#include "utils.h"
#include "Feature.h"
#include "OneHotFeatExtractor.h"
#include "OneHotErrorCode.h"
#include "ul_log.h"
#include "Configure.h"
#include "spreg.h"
#include "SluLog.h"

using namespace std;
using namespace utils;
using namespace feature;

using namespace hobotlog;
LogModule * loginstance;

namespace feat_extractor{

extern "C" FeatExtractor* create_feat_extr(const string confpath, 
                                           PreProcessor* pre){
    OneHotFeatExtractor* onehot_feat_extr = new OneHotFeatExtractor(confpath, pre);
    return dynamic_cast<FeatExtractor *>(onehot_feat_extr);
}

extern "C" void delete_feat_extr(FeatExtractor* feat_extr){
    OneHotFeatExtractor* onehot_feat_extr = dynamic_cast<OneHotFeatExtractor *>(feat_extr);
    //< delete_feat_extr
    onehot_feat_extr = NULL;
}

//< 构造函数
OneHotFeatExtractor::OneHotFeatExtractor(const string confpath, 
                                         PreProcessor* pre){
    int ret = 0;
    ret = init_conf(confpath, pre);
    if (ret < 0){
        //< error.
        cout << "failed to load onehot conf config file in " << confpath << endl;
        exit(-1);
    }

    loginstance->info("[SLU]: construct OneHotFeatExtractor success !" + confpath);
}

//< 析构函数
OneHotFeatExtractor::~OneHotFeatExtractor(){
    //< nothing;
}

//< ltp初始化
int OneHotFeatExtractor::init_ltp(PreProcessor* pre){
    this->pre = pre;
    if (pre == NULL){
        loginstance->error("[SLU]: Ltp pre is NULL");
        return ERR_ONEHOT_LTP_PRE_NULL;
    }

    return 0;
}

//< string2bool
bool transform2bool(string str){
    if (str == "TRUE"){
        return true;
    }
    else {
        return false;
    }
}

//< string2int
int transform2int(string str){
    int res = string2int(str);
    if (res < 0){
        return 0;
    }
    return res;
}

//< conf初始化
int OneHotFeatExtractor::init_conf(const string confpath, 
                                   PreProcessor* pre){
    int ret = 0;

    comcfg::Configure config;
    ret = config.load("./", confpath.c_str());
    if (ret < 0){
        cout << "failed to load onehot feat config file in" << confpath << endl;
        return ERR_ONEHOT_FEAT_LOAD_CONFIG;
    }

    //< init Log
    try {
        string log_file = config["ONEHOT_FEAT_LOG_CONF"].to_cstr();
        if (NULL == loginstance){
            loginstance = new hobotlog::LogModule(log_file);
			loginstance->info("[SLU]: load onehot feat log conf, and init loginstance success!");
        }
    }catch(...){
        cout << "do not set log file in onehot feat conf: " << confpath << endl;
        return ERR_ONEHOT_FEAT_NOT_SET_LOG_FILE;   
    }
    
    ret = init_ltp(pre);
    if (ret < 0){
        loginstance->error("[SLU]: failed to load LTP in me feat function init_conf()");
        return ret; 
    }

    //< load
    try {
        int init_size = static_cast<int>(config["FEAT_EXTRACTOR"].size());
        if (init_size != 1){
            loginstance->error("[SLU]: failed to load onehot feat config file: " + confpath + ", the FEAT_EXTRACTOR number is wrong");
            return ERR_ONEHOT_FEAT_CONFIG_FEAT_NUM;
        }

        this->use_q2feat = transform2bool(config["FEAT_EXTRACTOR"][0]["QUERY_FEAT"].to_cstr());
        this->use_p2feat = transform2bool(config["FEAT_EXTRACTOR"][0]["PATTERN_FEAT"].to_cstr());
        this->use_wsg = transform2bool(config["FEAT_EXTRACTOR"][0]["WSG_FEAT"].to_cstr());
        this->seq_size = transform2int(config["FEAT_EXTRACTOR"][0]["SEQUENCE_SIZE"].to_cstr());
        this->default_pad = config["FEAT_EXTRACTOR"][0]["DEFAULT_PAD"].to_cstr();
        this->default_unk = config["FEAT_EXTRACTOR"][0]["DEFAULT_UNK"].to_cstr();
        //< word map
        string dict_path = config["FEAT_EXTRACTOR"][0]["DICT_PATH"].to_cstr();
        int ret = load_dict(this->word_map, dict_path);
        if (ret < 0){
            loginstance->error("[SLU]: failed to load onehot feat config file: " + dict_path);
            return ERR_ONEHOT_FEAT_LOAD_DICT;
        }
        this->tensor_type = config["FEAT_EXTRACTOR"][0]["TENSOR_TYPE"].to_cstr();
    } catch(...){
        loginstance->error("[SLU]: failed to load onehot feat config file: " + confpath + ", conf content is wrong");
        return ERR_ONEHOT_FEAT_CONFIG_CONTENT;
    }

    loginstance->info("[SLU]: load onehot config file success!");

    return 0;
}

//< load dict
int OneHotFeatExtractor::load_dict(map<string, int>& word_map, 
                                   const string dict_path){
    ifstream dict;
    //< 打开文件
    dict.open(dict_path);

    if (!dict){
        loginstance->error("[SLU]:failed to open dict file : " + dict_path);
        return ERR_ONEHOT_FEAT_DICT_OPEN;
    }

    string line;
    int i = 1;
    while(getline(dict, line)){
        this->word_map[line] = i;
        i++;
    }
    this->map_size = i-1;

    //< 关闭文件
    dict.close();

    loginstance->info("[SLU]: load dict success! dict name: " + dict_path);

    return 0;
}

//< 特征提取
int OneHotFeatExtractor::get_feat(feature::Feature& feats, 
                                  const string query){
    int ret = 0;
    //< feature;
    vector<string> feature_vec;
    //< query 切分结果
    vector<string> q2ws_vec;
    //< pattern 切分结果
    vector<string> p2ws_vec;

    //< 如果使用QUERY_FEAT
    if (use_q2feat){
        //< 如果分词作为基本单元
        if (use_wsg){
            //< 获取query&分词结果
            ret = get_wsg(get_query(query), q2ws_vec);
            if (ret < 0){
                loginstance->error("[SLU]: failed to get wsg when get onehot feat use query feat! query: " + query);
                return ret;
            }
        }
        //< 单字切分作为基本单元
        else {
            //< 获取query&分词结果
            ret = get_sip(get_query(query), q2ws_vec);
            if (ret < 0){
                loginstance->error("[SLU]: failed to get sip when get onehot feat use query feat!query: " + query);
                return ret;
            }
        }
    }

    //< 如果使用PATTERN_FEAT
    if (use_p2feat){
        vector<string> pattern_vec = get_pattern(query);
        //< 如果分词作为基本单元
        //< 获取pattern&分词结果
        for (int i = 0; i < pattern_vec.size(); i++){
            //< 如果是 [artist]
            if (pattern_vec[i][0] == '[' && pattern_vec[i][pattern_vec[i].length()-1] == ']'){
                //< 直接push
                p2ws_vec.push_back(pattern_vec[i]);
            }
            else {
                //< 同query to feat
                vector<string> ws_vec;
                //< 如果分词作为基本单元
                if (use_wsg){
                    ret = get_wsg(pattern_vec[i], ws_vec);
                    if (ret < 0){
                        loginstance->error("[SLU]: failed to get wsg when get onehot feat use pattern feat! query : " + query);
                        return ret;
                    }
                }
                else {
                    ret = get_sip(pattern_vec[i], ws_vec);
                    if (ret < 0){
                        loginstance->error("[SLU]: failed to get sip when get onehot feat use pattern feat! query: " + query);
                        return ret;
                    }
                }
                //< 追加到p2ws_vec中
                p2ws_vec.insert(p2ws_vec.end(), ws_vec.begin(), ws_vec.end());
            }
        } //< for
    }

    //< word feat vec
    vector<string> word_features = q2ws_vec;
    word_features.insert(word_features.end(), p2ws_vec.begin(), p2ws_vec.end());

    //< get pad_seq
    vector<string> pad_seq_vec;
    ret = pad_seq(pad_seq_vec, word_features);
    if (ret < 0 || pad_seq_vec.size() != this->seq_size){
        loginstance->error("[SLU]: failed to get pad_seq when get onehot feat use pattern feat! query: " + query);
    }

    for (int i = 0; i < pad_seq_vec.size(); i++){
        string onehot_feature = onehot_mapping(pad_seq_vec[i]);
        feature_vec.push_back(onehot_feature);
    }

    ret = feats.add_feature(feature_vec);
    if (ret < 0){
        loginstance->error("[SLU]: failed to add onehot feature! query : " + query);
        return ERR_ADD_ONEHOT_FEATURE;
    }

    loginstance->info("[SLU]: get onehot feature and add feature success！query: " + query);

    return 0;
}

//< 分词
//< @param query : 待分词句子
//< @param ws_vec : 分词结果
int OneHotFeatExtractor::get_wsg(string query, 
                         vector<string>& ws_vec){
    int ret = 0;
    if (pre == NULL){
        //< PreProcessor not init;
        loginstance->error("[SLU]: PreProcessor is null!");
        return ERR_PREPROCESSOR_NULL;
    }
    ret = pre->wordseg(query, ws_vec);
    if (ret < 0){
        //< wordseg error;
        loginstance->error("[SLU]: failed to wordseg!");
        return ERR_WORD_SEGMENTATION;
    }

    return 0;
}

//< 单字切分
//< @param query : 待切分句子
//< @param ws_vec : 切分结果
int OneHotFeatExtractor::get_sip(string query, 
                         vector<string>& sip_vec){
    if (!strSplit2simple(query, sip_vec)){
        //< split str error;
        loginstance->error("[SLU]: failed to split query!");
        return ERR_SPLIT_TO_SIMPLE_WOED;
    }

    return 0;
}

//< get query
//< @param query : input
//< case: [刘德华:artist]的歌放一下 => 刘德华的歌放一下
string OneHotFeatExtractor::get_query(string query){
    string res_query;

    //< 先单字切分防止出现汉字某些字节与字符冲突导致错误
    vector<string> ws_vec;
    int ret = get_sip(query, ws_vec);
    if (ret < 0){
        return "";
    }

    //< get raw_query
    bool flag = false;
    for (size_t i = 0; i < ws_vec.size(); i++){
        if (ws_vec[i] == "["){
            flag = true;
        }
        else if (ws_vec[i] == ":" && flag){
            size_t j = i;
            while (j < ws_vec.size() && ws_vec[j] != "]"){
                j++;
            }
            if (j != ws_vec.size()){
                i = j;
            }
        }
        else {
            res_query += ws_vec[i];
        }
    }

    return res_query;
}

//< get pattern
//< @param query : input
//< vector<string> : output
vector<string> OneHotFeatExtractor::get_pattern(string query){
    vector<string> res_vec;
    string chr;

    //< 先单字切分防止出现汉字某些字节与字符冲突导致错误
    vector<string> ws_vec;
    int ret = get_sip(query, ws_vec);
    if (ret < 0){
        return res_vec;
    }

    //bool flag = false;
    for (size_t i = 0; i < ws_vec.size(); i++){
        if (ws_vec[i] == "["){
            //< 上一个短句追加到res_vec;
            if (chr != ""){
                res_vec.push_back(chr);
                chr = "";
            }

            //flag = true;
            chr += ws_vec[i];
            size_t j = i+1;
            while(j < ws_vec.size() && ws_vec[j] != ":"){
                j++;
            }
            if (j != ws_vec.size()){
                i = j+1;
            }
            for ( ; i < ws_vec.size() && ws_vec[i] != "]"; i++){
                chr += ws_vec[i];
            }
            chr += ws_vec[i];
            
            //< 当前label信息追加到res_vec
            res_vec.push_back(chr);
            chr = "";
        }
        else {
            chr += ws_vec[i]; 
        }
    }
    if (chr != ""){
        res_vec.push_back(chr);
    }

    return res_vec;
}

/*
 * @brief pad_seq
 * @input :
 *      周 杰 伦 的 青 花 瓷
 * @output :
 *      <PAD> <PAD> <PAD> 周 杰 伦 的 青 花 瓷
 */
int OneHotFeatExtractor::pad_seq(vector<string>& res, 
                                 vector<string> ws_vec){
    vector<string> pad_vec;

    //< 计算长度差
    int pad_count = this->seq_size - ws_vec.size();

    //< 如果输入长度小于等于定义长度
    //< 补pad
    if (pad_count >= 0){
        for (int i = 0; i < pad_count; i++){
            pad_vec.push_back(default_pad);
        }
        pad_vec.insert(pad_vec.end(), ws_vec.begin(), ws_vec.end());
    }
    //< 如果输入长度大于定义长度 截断
    else {
        //< 可优化
        for (int i = 0; i < this->seq_size; i++){
            pad_vec.push_back(ws_vec[i]);
        }
    }

    res = pad_vec;

    return 0;
}

//< onehot 映射
string OneHotFeatExtractor::onehot_mapping(string word){
    string onehot;
    //< 获取词表长度
    int length = this->map_size;
    //< 获取某个词对应的下标
    //< word_map从1开始 返回0表示不存在
    int idx = word_map[word]-1;
    //< 如果词不存在 那么当unk处理
    if (idx == -1){
        idx = word_map[default_unk]-1;
    }
    if (this->tensor_type == "INDEX"){
        onehot = to_string(idx);
    }
    else if (this->tensor_type == "VECTOR"){
        for (int i = 0; i < length; i++){
            if (idx == i){
                onehot += "1 ";
            }
            else {
                onehot += "0 ";
            }
        }
    }

    return onehot;
}

} //< namespace feat_extractor;
/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
