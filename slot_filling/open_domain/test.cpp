/*************************************************************************
	> File Name: test.cpp
	> Author: TengyueMa
	> Mail: lumujin@icloud.com
	> Created Time: 一  1/22 21:02:38 2018
 ************************************************************************/

#include <iostream>
#include <string>
#include <vector>
#include <dlfcn.h>
#include <unistd.h>
#include "FeatExtractor.h"
#include "SlotFilling.h"
#include "PreProcessor.h"
#include "FeatExtractor.h"
#include "Feature.h"

using namespace std;
using namespace feat_extractor;
using namespace slot_filling;
using namespace feature;

int main(int argc, char ** argv){
    string feat_so = "../../feat_extractor/open_domain_feat/lib/libopendomain_feat.so";
    string ner_so = "lib/libopendomain.so";
    string conf = "./openDomain.conf";
    string log_conf = "./log_path.conf";

    PreProcessor* pre = new PreProcessor();

    //< 初始化
    pre->create_wsg_engine("../../data/model/ltp_model/cws.model");
    pre->create_pos_engine("../../data/model/ltp_model/pos.model");
    pre->create_ner_engine("../../data/model/ltp_model/ner.model");
    pre->create_pas_engine("../../data/model/ltp_model/parser.model");
    
    //< input
    string query;
    if (argc >= 2){
        query = string(argv[1]);
    }
    else {
        return -1;
    }

    typedef FeatExtractor* (*CREATE_FEAT_EXTR)(const string, PreProcessor*);

    void *handle = dlopen(feat_so.c_str(), RTLD_NOW);
    CREATE_FEAT_EXTR create_feat_extr = reinterpret_cast<CREATE_FEAT_EXTR>(dlsym(handle, "create_feat_extr"));

    //< 开放域语义解析接口
    FeatExtractor* feat_ext = create_feat_extr(conf, pre);

    Feature feats;
    feat_ext->get_feat(feats, query);
    cout << feats << endl;

    //< ner
    typedef SlotFilling* (*CREATE_SLOT_FILLING)(const string, const string);
    void* handle_ner = dlopen(ner_so.c_str(), RTLD_NOW);
    if (!handle_ner){
        cout << dlerror() << endl;
    }
    CREATE_SLOT_FILLING create_slot_filling = reinterpret_cast<CREATE_SLOT_FILLING>(dlsym(handle_ner, "create_slot_filling"));
    SlotFilling* ner = NULL;
    ner = create_slot_filling("null", log_conf);

    vector<_slot> slots;
    ner->parse(query, slots, feat_ext);

    for (size_t i = 0; i < slots.size(); i++){
        cout << slots[i].name << " : " << slots[i].value << endl;
    }

    typedef FeatExtractor* (*DELETE_FEAT_EXTR)(FeatExtractor *);
    DELETE_FEAT_EXTR delete_feat_extr = reinterpret_cast<DELETE_FEAT_EXTR>(dlsym(handle, "delete_feat_extr"));
    delete_feat_extr(feat_ext);

    dlclose(handle);
    dlclose(handle_ner);

    return 0;
}

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
