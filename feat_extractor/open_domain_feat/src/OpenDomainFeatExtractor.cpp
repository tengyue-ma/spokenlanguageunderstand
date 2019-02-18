/*************************************************************************
	> File Name: OpenDomainFeatExtractor.cpp
	> Author: TengyueMa
	> Mail: lumujin@icloud.com
	> Created Time: Mon 19 Mar 2018 01:57:44 PM CST
 ************************************************************************/

#include <iostream>
#include "OpenDomainFeatExtractor.h"
#include "OpenDomainFeatErrorCode.h"
#include "FeatExtractor.h"
#include "ul_log.h"
#include "Configure.h"
#include "spreg.h"
#include "SluLog.h"
using namespace std;

using namespace hobotlog;
LogModule * loginstance;

namespace feat_extractor{

extern "C" FeatExtractor* create_feat_extr(const string confpath, 
                                           PreProcessor* pre){
    OpenDomainFeatExtractor* ods_feat_extr = new OpenDomainFeatExtractor(confpath, pre);
    return dynamic_cast<FeatExtractor *>(ods_feat_extr);
}

extern "C" void delete_feat_extr(FeatExtractor* feat_extr){
    OpenDomainFeatExtractor* ods_feat_extr = dynamic_cast<OpenDomainFeatExtractor *>(feat_extr);

    //< delete me_feat_extr;
    ods_feat_extr = NULL;
}

//< 构造函数
OpenDomainFeatExtractor::OpenDomainFeatExtractor(const string confpath, 
                                                 PreProcessor* pre){
	int ret = 0;
	ret = init_conf(confpath, pre);
    if (ret < 0){
		cout << "failed to init open domain feat slot type ! "<< endl;
		cout << "error code: "<< ret << endl;
        exit(-1);
    }

	ret = init_ltp(pre);
    if (ret < 0){
		loginstance->error("[SLU]: failed to init open domain feat ltp: " + confpath);
		//loginstance->error("error code: " + (itoa)ret);
        exit(-1);
    }

	ret = init_slot_type();
    if (ret < 0){
		loginstance->error("[SLU]: failed to init open domain feat slot type: " + confpath);
		//loginstance->error("error code: ");
		exit(-1);
    }
	
	loginstance->info("[SLU]: construct OpenDomainFeatExtractor success: " + confpath);
	
}

//< 析构函数
OpenDomainFeatExtractor::~OpenDomainFeatExtractor(){
    this->pre = NULL;
    this->_slot_type = NULL;
}

int OpenDomainFeatExtractor::init_ltp(PreProcessor* pre){
    this->pre = pre;
    if (pre == NULL){
        cout << "LTP init error. pre is NULL." << endl;
        return ERR_OPEN_DOMAIN_FEAT_LTP_PRE_NULL;
    }
    return 0;
}

//< 这里pre没有使用
int OpenDomainFeatExtractor::init_conf(const string confpath, 
                                       PreProcessor* pre){
    comcfg::Configure config;
    int ret = config.load("./", confpath.c_str());
    if (ret < 0){
        cout << "failed to load config file in " << confpath << endl;
        return ERR_OPEN_DOMAIN_FEAT_LOAD_CONFIG;
    }
	
    //init log
    try {
        string log_file = config["OPEN_DOMAIN_FEAT_LOG_CONF"].to_cstr();
		if(NULL == loginstance){
			loginstance = new hobotlog::LogModule(log_file);
			loginstance->info("[SLU]: load open domain feat log conf, and init loginstance success!");
		}
    }catch(...){
        cout << "do not set log file in open domain feat conf: " << confpath << endl;
        return ERR_OPEN_DOMAIN_FEAT_NOT_SET_LOG_FILE;
    }
    
    //< load
    try {
        //< load slotname
        int slotname_size = static_cast<int>(config["SLOT_NAME"].size());
        if (slotname_size != 1){
            loginstance->error("[SLU]: failed to load open domain feat config file: " + confpath + ", the FEAT_EXTRACTOR number is wrong");
            return ERR_OPEN_DOMAIN_FEAT_CONFIG_FEAT_NUM;
        }
        this->nh = config["SLOT_NAME"][0]["SLOT_NAME_NH"].to_cstr();
        this->ns = config["SLOT_NAME"][0]["SLOT_NAME_NS"].to_cstr();
        this->ni = config["SLOT_NAME"][0]["SLOT_NAME_NI"].to_cstr();
        this->rh = config["SLOT_NAME"][0]["SLOT_NAME_RH"].to_cstr();
        this->rs = config["SLOT_NAME"][0]["SLOT_NAME_RS"].to_cstr();

        //< load代词关键词
        int word_rh_size = static_cast<int>(config["WORD_RH"][0]["WORD"].size());
        for (int i = 0; i < word_rh_size; i++){
            string word = config["WORD_RH"][0]["WORD"][i].to_cstr();
            this->rh_words.push_back(word);
        }

        int word_rs_size = static_cast<int>(config["WORD_RS"][0]["WORD"].size());
        for (int i = 0; i < word_rs_size; i++){
            string word = config["WORD_RS"][0]["WORD"][i].to_cstr();
            this->rs_words.push_back(word);
        }

        //< load slotname&词典地址
        int dict_size = static_cast<int>(config["DICT"].size());
        for (int i = 0; i < dict_size; i++){
            string dictname = config["DICT"][i]["DICT_NAME"].to_cstr();
            string dictpath = config["DICT"][i]["DICT_PATH"].to_cstr();
            this->vec_dict.push_back(pair<string, string>(dictname, dictpath));
        }

    } catch (...) {
        loginstance->error("[SLU]: failed to load open domain feat config file: " + confpath + ", conf content is wrong");
        return ERR_OPEN_DOMAIN_FEAT_CONFIG_CONTENT;
    }
	
	loginstance->info("[SLU]: load open domain feat config file success！");

    return 0;
}

int OpenDomainFeatExtractor::init_slot_type(){
    if (this->vec_dict.size() <= 0){
        this->_slot_type = NULL;
		loginstance->error("[SLU]: open domain feat dict is null！");
		loginstance->error("[SLU]: open domain feat slot type init fault！");
        return ERR_OPEN_DOMAIN_FEAT_INIT_SLOT_TYPE;
    }
    else {
        this->_slot_type = new slotType(this->vec_dict);
    }
	
	loginstance->info("[SLU]: open domain feat init slot type success！");

    return 0;
}

//< 判断字符串是否存在并返回位置，不存在返回-1
//< 或可直接用map
int hasLabel(string label, 
             vector<string> str_vec){
    for (size_t i = 0; i < str_vec.size(); i++){
        if (label == str_vec[i]){
            return i;
        }
    }

    return -1;
}

//< push to feat list
int OpenDomainFeatExtractor::pushfeat(feature::Feature& feats, 
                                      string default_name, 
                                      string value){
    if (_slot_type == NULL){
        string feat = default_name + " " + value;
    }

    string feat = _slot_type->getSlotname(value, default_name) + " " + value;
    feats.add_feature(feat);

    return 0;
}

int OpenDomainFeatExtractor::get_feat(feature::Feature& feats, 
                                      const string query){
    int ret = 0;
	
	vector<string> ws_vec;
    vector<string> ner_vec;
    vector<string> pos_vec;

    //< ltp deal
	ret = pre->wordseg(query, ws_vec);
    if (ret < 0){
        //< wordseg error;
		loginstance->error("[SLU]: failed to get pre wordseg while open domain get feat!");
        return ret;
    }
	ret = pre->postag(ws_vec, pos_vec);
    if (ret < 0){
        //< postag error;
		loginstance->error("[SLU]: failed to get pre postag while open domain get feat!");
        return ret;
    }
	ret = pre->ner(ws_vec, pos_vec, ner_vec);
    if (ret < 0){
        //< ner error;
		loginstance->error("[SLU]: failed to get pre ner while open domain get feat!");
        return ret;
    }

    string rh_rs = "r";
    string nh_labels[4] = {"S-Nh", "B-Nh", "I-Nh", "E-Nh"}; 
    string ns_labels[4] = {"S-Ns", "B-Ns", "I-Ns", "E-Ns"};
    string ni_labels[4] = {"S-Ni", "B-Ni", "I-Ni", "E-Ni"};
    vector<string> nh_label(nh_labels, nh_labels+4);
    vector<string> ns_label(ns_labels, ns_labels+4);
    vector<string> ni_label(ni_labels, ni_labels+4);

    //< 解析
    size_t count = ws_vec.size();
    for (size_t i = 0; i < count; i++){
        //< nh
        if (hasLabel(ner_vec[i], nh_label) != -1){
            pushfeat(feats, nh, ws_vec[i]);
        }
        //< ns
        else if (hasLabel(ner_vec[i], ns_label) != -1){
            pushfeat(feats, ns, ws_vec[i]);
        }
        //< ni => S-Ni
        else if (hasLabel(ner_vec[i], ni_label) == 0){
            pushfeat(feats, ni, ws_vec[i]);
        }
        //< ni => B-Ni --- E-ni
        else if (hasLabel(ner_vec[i], ni_label) == 1){
            string word = "";
            for (size_t j = i; j < count; j++){
                word += ws_vec[j];
                i = j;
                if (ner_vec[j] == ni_label[3]){
                    break;
                }
            }
            pushfeat(feats, ni, word);
        }
        //< 人称代词
        else if (hasLabel(ws_vec[i], rh_words) != -1 && pos_vec[i] == rh_rs){
            pushfeat(feats, rh, ws_vec[i]);
        }
        //< 地点代词
        else if (hasLabel(ws_vec[i], rs_words) != -1 && pos_vec[i] == rh_rs){
            pushfeat(feats, rs, ws_vec[i]);
        }
        //< 其他
        else {
            pushfeat(feats, "O", ws_vec[i]);
        }
    }
	
	loginstance->info("[SLU]: open domain get feat success!");

    return 0;
}

} //< namespace feat_extractor;
