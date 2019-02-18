/*************************************************************************
	> File Name: ClfFeatExtractor.cpp
	> Author: TengyueMa
	> Mail: lumujin@icloud.com
	> Created Time: 三  1/24 15:42:10 2018
 ************************************************************************/

#include <iostream>
#include <string>
#include <vector>
#include <stdlib.h>
#include "utils.h"
#include "Feature.h"
#include "MEFeatExtractor.h"
#include "MeFeatErrorCode.h"
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
    MEFeatExtractor* me_feat_extr = new MEFeatExtractor(confpath, pre);
    return dynamic_cast<FeatExtractor *>(me_feat_extr);
}

extern "C" void delete_feat_extr(FeatExtractor* feat_extr){
    MEFeatExtractor* me_feat_extr = dynamic_cast<MEFeatExtractor *>(feat_extr);
    //delete me_feat_extr;
    me_feat_extr = NULL;
}

//< 构造函数
MEFeatExtractor::MEFeatExtractor(const string confpath, 
                                 PreProcessor* pre){
    int ret = 0;
    ret = init_conf(confpath, pre);
    if (ret < 0){
        //< error.
        cout << "failed to load me feat config file in" << confpath << endl;
        exit(-1) ;
    } 
	
	loginstance->info("[SLU]: construct MEFeatExtractor success !" + confpath);
	
}

//< 析构函数
MEFeatExtractor::~MEFeatExtractor(){
    
}

//< ltp初始化
int MEFeatExtractor::init_ltp(PreProcessor* pre){
    this->pre = pre;
    if (pre == NULL){
        loginstance->error("[SLU]: Ltp pre is NULL");
        return ERR_MEFEAT_LTP_PRE_NULL;
    }
    
    return 0;
}

//< string2bool
bool transform2bool(string str){
    if (str == "TRUE"){
        return true;
    }
    else 
        return false;
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
int MEFeatExtractor::init_conf(const string confpath, 
                               PreProcessor* pre){
    int ret = 0;

    comcfg::Configure config;
    ret = config.load("./", confpath.c_str());
    if (ret < 0){
        cout << "failed to load me feat config file in" << confpath << endl;
        return ERR_MEFEAT_LOAD_CONFIG;
    }

    //init log
    try {
        string log_file = config["ME_FEAT_LOG_CONF"].to_cstr();
		if(NULL == loginstance){
			loginstance = new hobotlog::LogModule(log_file);
			loginstance->info("[SLU]: load me feat log conf, and init loginstance success!");
		}
    }catch(...){
        cout << "do not set log file in me feat conf: " << confpath << endl;
        return ERR_MEFEAT_NOT_SET_LOG_FILE;
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
            loginstance->error("[SLU]: failed to load me feat config file: " + confpath + ", the FEAT_EXTRACTOR number is wrong");
            return ERR_MEFEAT_CONFIG_FEAT_NUM;
        }
        
        this->use_q2feat = transform2bool(config["FEAT_EXTRACTOR"][0]["QUERY_FEAT"].to_cstr());
        this->use_p2feat = transform2bool(config["FEAT_EXTRACTOR"][0]["PATTERN_FEAT"].to_cstr());
        this->use_wsg = transform2bool(config["FEAT_EXTRACTOR"][0]["WSG_FEAT"].to_cstr());
        this->start = transform2int(config["FEAT_EXTRACTOR"][0]["NGRAM_START"].to_cstr());
        this->end = transform2int(config["FEAT_EXTRACTOR"][0]["NGRAM_END"].to_cstr());

        //< transform
    } catch(...) {
        loginstance->error("[SLU]: failed to load me feat config file: " + confpath + ", conf content is wrong");
        return ERR_MEFEAT_CONFIG_CONTENT;
    }

    loginstance->info("[SLU]: load me feat config file success！");

    return 0;
}

//< 特征提取
//< @brief get_feat
//< @param feats:返回的特征值
int MEFeatExtractor::get_feat(feature::Feature& feats, 
                              const string query){
    int ret = 0;
    //< feature;
    vector<string> feature_vec;
    //< query 切分结果
    vector<string> q2ws_vec;
    //< pattern 切分结果
    vector<string> p2ws_vec;

    //< 如果使用query feat
    if (use_q2feat){
        //< 分词作为基本单元
        if (use_wsg){
            //< 获取query&分词失败
            ret = get_wsg(get_query(query), q2ws_vec);
            if ( ret < 0){
                loginstance->error("[SLU]: failed to get wsg when get me feat use query feat!query: " + query);
                return ret;
            }
        }
        //< 单字切分作为基本单元
        else {
            //< 获取query&切分失败
            ret = get_sip(get_query(query), q2ws_vec);
            if ( ret < 0){
                loginstance->error("[SLU]: failed to get sip when get me feat use query feat!query: " + query);
                return ret;
            }
        }
        //< 提取n-gram特征
        ret = get_ngram(feature_vec, q2ws_vec, start, end);
        if ( ret < 0){
            loginstance->error("[SLU]: failed to get ngram feature when get me feat use query feat!query: " + query);
            return ret;
        }
    }
    if (use_p2feat){
        //< 获取pattern&分词失败
        vector<string> pattern_vec = get_pattern(query);
        for (size_t i = 0; i < pattern_vec.size(); i++){
            if (pattern_vec[i][0] == '[' && pattern_vec[i][pattern_vec[i].length()-1] == ']'){
                p2ws_vec.push_back(pattern_vec[i]);
            }
            else {
                //< 同query to feat
                vector<string> ws_vec;
                if (use_wsg){
                    ret = get_wsg(pattern_vec[i], ws_vec);
                    if (ret < 0){
                        loginstance->error("[SLU]: failed to get wsg when get me feat use pattern feat!query: " + query);
                        return ret;
                    }
                }
                else {
                    ret = get_sip(pattern_vec[i], ws_vec);
                    if (ret < 0){
                        loginstance->error("[SLU]: failed to get sip when get me feat use pattern feat!query: " + query);
                        return ret;
                    }
                }
    
                //< 追加到p2ws_vec中
                p2ws_vec.insert(p2ws_vec.end(), ws_vec.begin(), ws_vec.end());
            }
        }

        //< 提取n-gram特征
        ret = get_ngram(feature_vec, p2ws_vec, start, end);
        if (ret < 0){
            loginstance->error("[SLU]: failed to get ngram feature when get me feat use pattern feat!query: " + query);
            return ret;
        }
    }

    //< 添加特征到feats
    ret = feats.add_feature(feature_vec);
    if(ret < 0){
        loginstance->error("[SLU]: failed to add me feature!query: " + query);
        return ERR_ADD_ME_FEATURE;
    }

    loginstance->info("[SLU]: get me feature and add feature success！query: " + query);

    return 0;
}

//< 分词
//< @param query : 待分词句子
//< @param ws_vec : 分词结果
int MEFeatExtractor::get_wsg(string query, 
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
int MEFeatExtractor::get_sip(string query, 
                             vector<string>& sip_vec){
    if (!strSplit2simple(query, sip_vec)){
        //< split str error;
        loginstance->error("[SLU]: failed to split query!");
        return ERR_SPLIT_TO_SIMPLE_WOED;
    }

    return 0;
}

//< get ngram
//< @param res : 返回结果
//< @param ws_vec : 待处理数据
//< @param start - end n元长度范围
int MEFeatExtractor::get_ngram(vector<string>& res, 
                               vector<string> ws_vec, 
                               const size_t start, 
                               const size_t end){
    //< 取start - end长度之间length
    for (size_t len = start; len <= end; len++){
        //< 对于每个length
        //< 给出vector中满足条件的所有组合
        for (size_t i = 0; i+len-1 < ws_vec.size(); i++){
            string str;
            //< 一个组合
            for (size_t j = i; j < i+len; j++){
                str += ws_vec[j];
            }
            //< 添加到vector
            res.push_back(str);
        }
    }

    if (res.size() == 0){
        //< something error;
        loginstance->error("[SLU]: failed to get me ngram feature!");
        return ERR_GET_NGRAM_FEATURE;
    }

    return 0;
}

//< get query
//< @param query : input
//< case: [刘德华:artist]的歌放一下 => 刘德华的歌放一下
string MEFeatExtractor::get_query(string query){
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
vector<string> MEFeatExtractor::get_pattern(string query){
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

} //< namespace feat_extractor;
/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
