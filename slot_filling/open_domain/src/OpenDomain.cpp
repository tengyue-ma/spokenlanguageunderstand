/*************************************************************************
	> File Name: OpenDomain.cpp
	> Author: TengyueMa
	> Mail: lumujin@icloud.com
	> Created Time: Mon 19 Mar 2018 07:35:11 PM CST
 ************************************************************************/

#include <iostream>
#include "utils.h"
#include "OpenDomain.h"
#include "logger.h"
#include "comm_def.h"
#include "ul_log.h"
#include "Configure.h"
#include "spreg.h"
using namespace std;
using namespace hobotlog;

//< log定义
LogModule * loginstance = NULL;

namespace slot_filling{

extern "C" SlotFilling* create_slot_filling(const string model_path, 
                                            const string log_path){
    OpenDomain* ods_ner = new OpenDomain(log_path);
    return dynamic_cast<SlotFilling *>(ods_ner);
}

extern "C" void delete_slot_filling(SlotFilling* ner){
    OpenDomain* ods_ner = dynamic_cast<OpenDomain *>(ner);
    //< delete ods_ner;
    ods_ner = NULL;
}

//< 构造函数
OpenDomain::OpenDomain(const string log_path){
    comcfg::Configure config;
    int ret = config.load("./", log_path.c_str());
    if (ret < 0){
        cout << "failed to load log conf in " << log_path << endl;
        exit(-1);
    }

    //< init log
    try {
        string log_file = config["OPEN_DOMAIN_LOG_CONF"].to_cstr();
        if (loginstance == NULL){
            loginstance = new LogModule(log_file);
        }
    }catch(...){
        cout << "failed to load log config." << endl;
        exit(-1);
    }
}

//< 析构函数
OpenDomain::~OpenDomain(){
}

//< 对外接口
int OpenDomain::parse(const string query, 
                      vector<_slot>& slots, 
                      feat_extractor::FeatExtractor* feat_ext){
    //< get feats
    feature::Feature feats;
    feat_ext->get_feat(feats, query);
    loginstance->info("[SLU]: get feature in opendomain.");
   
    vector<string> features = feats.feats;
 
    int idx = 0;
    for (size_t i = 0; i < features.size(); i++){
        vector<string> slot_vec;
        //< 空格分割
        utils::strSplitbysep(features[i], slot_vec, " ");

        /*
         * slots[0] == "person"
         * slots[1] == "刘德华"
         */
        _slot slot;
        slot.name = slot_vec[0];
        slot.value = slot_vec[1];
        vector<string> slot_value_split;
        utils::strSplit2simple(slot_vec[1], slot_value_split);
        size_t count = slot_value_split.size();
        slot.start = idx;
        slot.end = idx + count -1;
        idx += count;

        if (slot.name != "O"){
            slots.push_back(slot);
        }
    }

    return 0;
}

} //< namespace slot_filling;

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
