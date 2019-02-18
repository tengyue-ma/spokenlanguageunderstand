/*************************************************************************
	> File Name: CRF_UnitTest.cpp
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
#include "SlotTagger.h"
using namespace std;
using namespace feat_extractor;
using namespace slot_filling;

extern "C" SlotFilling* create_slot_filling(const string model_path,const string log_path);
extern "C" void delete_slot_filling(SlotFilling* ner);

TEST_CASE ("SlotFilling", "CRF") {
    
    //< 初始化ltp
	PreProcessor* pre = new PreProcessor();
    pre->create_wsg_engine("../../data/model/ltp_model/cws.model");
    pre->create_pos_engine("../../data/model/ltp_model/pos.model");
    pre->create_ner_engine("../../data/model/ltp_model/ner.model");
    pre->create_pas_engine("../../data/model/ltp_model/parser.model");

    //< 指定特征提取库, 
    string ner_feat_so = "../../feat_extractor/crf_feat/lib/libcrffeat.so";
	string dict_conf = "music_dict.conf";
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
	string query = "";
	string result = "";
	vector<_slot> slots;
    string model_path = "music.model";
    string log_conf = "../conf/log_path.conf";
	SlotTagger* slot_tagger1 = new SlotTagger(model_path, log_conf);
	SlotTagger* slot_tagger2 = new SlotTagger(model_path, log_conf);
	
    SECTION("测试 create_slot_filling 和 delete_slot_filling"){
		SlotFilling* slot_fill = create_slot_filling(model_path, log_conf);
        REQUIRE_NOTHROW(create_slot_filling(model_path, log_conf));
		REQUIRE_NOTHROW(delete_slot_filling(slot_fill));
    }
	
	SECTION("测试SlotTagger构造函数和析构函数"){
        SlotTagger* slot_tagger_test = new SlotTagger(model_path, log_conf);
        REQUIRE_NOTHROW(delete slot_tagger_test);
    }
	SECTION("测试CRFWrapper构造函数和析构函数"){
        CRFWrapper* crf_wrapper = new CRFWrapper();
        REQUIRE_NOTHROW(delete crf_wrapper);
    }
	SECTION("测试CRFWrapper析构函数"){
        CRFWrapper* crf_wrapper = new CRFWrapper();
		crf_wrapper->load_model(model_path);
        REQUIRE_NOTHROW(delete crf_wrapper);
    }
	SECTION("slot_tagger1, query为空"){
        ret = slot_tagger1->parse("", slots, feat_ext_ner);
		REQUIRE(ret == 0);
    }
	SECTION("slot_tagger1, 周杰伦的青花瓷"){
        ret = slot_tagger1->parse("周杰伦的青花瓷", slots, feat_ext_ner);
		for (size_t i = 0; i < slots.size(); i++) {
			result += slots[i].name + "/";
			result += slots[i].value + "/";
		}
		REQUIRE(ret == 0);
		REQUIRE(result == "artist/周杰伦/title/青花瓷/");
    }
}
