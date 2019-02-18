/*************************************************************************
	> File Name: OpenDomain_UnitTest.cpp
	> Author: leilei.zhou
	> Mail: leilei.zhou@horizon.ai
	> Created Time: 2018.12.12
 ************************************************************************/


#define CATCH_CONFIG_MAIN  
#include "catch.hpp"
#include <dlfcn.h>
#include <unistd.h>
#include <stdlib.h>
#include <fstream>
#include <cstring>
#include <map>
#include <vector>
#include <string>
#include "Feature.h"
#include "FeatExtractor.h"
#include "PreProcessor.h"
#include "OpenDomain.h"
using namespace std;
using namespace feat_extractor;
using namespace slot_filling;

extern "C" SlotFilling* create_slot_filling(const string model_path,const string log_path);
extern "C" void delete_slot_filling(SlotFilling* ner);

TEST_CASE ("SlotFilling", "OpenDomain") {
    
    //< 初始化ltp
	PreProcessor* pre = new PreProcessor();
    pre->create_wsg_engine("../../data/model/ltp_model/cws.model");
    pre->create_pos_engine("../../data/model/ltp_model/pos.model");
    pre->create_ner_engine("../../data/model/ltp_model/ner.model");
    pre->create_pas_engine("../../data/model/ltp_model/parser.model");

    //< 指定特征提取库, 
    string ner_feat_so = "../../feat_extractor/open_domain_feat/lib/libopendomain_feat.so";
	string dict_conf = "openDomain.conf";
    typedef FeatExtractor* (*CREATE_FEAT_EXTR)(const string, PreProcessor*);
    void* handle_ner_feat = dlopen(ner_feat_so.c_str(), RTLD_NOW);
    if (!handle_ner_feat){
        cout << dlerror() << endl;
        exit(-1);
    }
    CREATE_FEAT_EXTR create_feat_extr = reinterpret_cast<CREATE_FEAT_EXTR>(dlsym(handle_ner_feat, "create_feat_extr"));
	FeatExtractor* feat_ext_ner = create_feat_extr(dict_conf, pre);
    if (feat_ext_ner == NULL){
        cout << ">>>>>> ERROR: feat_ext error." << endl;
        exit(-1);
    }
	
	int ret = 0;
	string result = "";
	vector<_slot> slots;
    string log_conf = "../conf/log_path.conf";
	OpenDomain* open_domain = new OpenDomain(log_conf);
	
    SECTION("测试 create_slot_filling 和 delete_slot_filling"){
		SlotFilling* slot_fill = create_slot_filling("null", log_conf);
        REQUIRE_NOTHROW(create_slot_filling("null", log_conf));
		REQUIRE_NOTHROW(delete_slot_filling(slot_fill));
    }
	
	SECTION("测试构造函数和析构函数"){
        OpenDomain* open_domain_test = new OpenDomain(log_conf);
        REQUIRE_NOTHROW(delete open_domain_test);
    }
	SECTION("open_domain, query为空"){
        ret = open_domain->parse("", slots, feat_ext_ner);
		REQUIRE(ret == 0);
    }
	SECTION("open_domain, 周杰伦的青花瓷"){
        ret = open_domain->parse("周杰伦的青花瓷", slots, feat_ext_ner);
		for (size_t i = 0; i < slots.size(); i++) {
			result += slots[i].name + "/";
			result += slots[i].value + "/";
		}
		REQUIRE(ret == 0);
		REQUIRE(result == "person/周杰伦/");
    }
}
