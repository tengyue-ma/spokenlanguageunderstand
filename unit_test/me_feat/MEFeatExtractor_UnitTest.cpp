/*************************************************************************
	> File Name: MEFeatExtractor_UnitTest.cpp
	> Author: leilei.zhou
	> Mail: leilei.zhou@horizon.ai
	> Created Time: 2018.12.06
 ************************************************************************/


#define CATCH_CONFIG_MAIN  
#include "catch.hpp"
#include <iostream>
#include <stdio.h>
#include <dlfcn.h>
#include <unistd.h>
#include "Feature.h"
#include "FeatExtractor.h"
#include "PreProcessor.h"
#include "MEFeatExtractor.h"
using namespace std;
using namespace feat_extractor;

extern "C" FeatExtractor* create_feat_extr(const string confpath,PreProcessor* pre);
extern "C" void delete_feat_extr(FeatExtractor* feat_extr);

TEST_CASE ("FeatExtractor", "me_feat") {
    
    //< 初始化ltp
	PreProcessor* pre = new PreProcessor();
    pre->create_wsg_engine("../../data/model/ltp_model/cws.model");
    pre->create_pos_engine("../../data/model/ltp_model/pos.model");
    pre->create_ner_engine("../../data/model/ltp_model/ner.model");
    pre->create_pas_engine("../../data/model/ltp_model/parser.model");
	
	int ret = 0;
	string query = "";
	string result = "";
	feature::Feature feat;
    string conf1 = "feat1.conf";
	string conf2 = "feat2.conf";
	string conf3 = "feat3.conf";
	string conf4 = "feat4.conf";
	MEFeatExtractor* me_feat_extr1 = new MEFeatExtractor(conf1, pre);
	MEFeatExtractor* me_feat_extr2 = new MEFeatExtractor(conf2, pre);
	MEFeatExtractor* me_feat_extr3 = new MEFeatExtractor(conf3, pre);
	MEFeatExtractor* me_feat_extr4 = new MEFeatExtractor(conf4, pre);
	
    SECTION("测试create_feat_extr 和 delete_feat_extr"){
		FeatExtractor* feat_ext = create_feat_extr(conf1, pre);
        REQUIRE_NOTHROW(create_feat_extr(conf1, pre));
		REQUIRE_NOTHROW(delete_feat_extr(feat_ext));
    }
	
	SECTION("测试构造函数和析构函数"){
        MEFeatExtractor* me_feat_extr_test = new MEFeatExtractor(conf1, pre);
        REQUIRE_NOTHROW(delete me_feat_extr_test);
    }
	
	SECTION("feat1, query+分词, query为空"){
        query = "";
        ret = me_feat_extr1->get_feat(feat, query);
		REQUIRE(ret < 0);
    }
    SECTION("feat1, query+分词, 周杰伦的歌"){
        ret = me_feat_extr1->get_feat(feat, "周杰伦的歌");
		for (int i = 0; i < feat.feats.size(); ++i){
			result += feat.feats[i] + "/";
		}
		REQUIRE(ret == 0);
		REQUIRE(result == "周杰伦的/的歌/周杰伦的歌/");
    }
	SECTION("feat1, query+分词, [周杰伦:artist]唱的[青花瓷:title]"){
        ret = me_feat_extr1->get_feat(feat, "[周杰伦:artist]唱的[青花瓷:title]");
		for (int i = 0; i < feat.feats.size(); ++i){
			result += feat.feats[i] + "/";
		}
		REQUIRE(ret == 0);
		REQUIRE(result == "周杰伦唱/唱的/的青花瓷/周杰伦唱的/唱的青花瓷/周杰伦唱的青花瓷/");
    }
	
    SECTION("feat2, query+不分词, 周杰伦的歌"){
        ret = me_feat_extr2->get_feat(feat, "周杰伦的歌");
		for (int i = 0; i < feat.feats.size(); ++i){
			result += feat.feats[i] + "/";
		}
		REQUIRE(ret == 0);
		REQUIRE(result == "周杰/杰伦/伦的/的歌/周杰伦/杰伦的/伦的歌/周杰伦的/杰伦的歌/");
    }
	SECTION("feat2, query+不分词, [周杰伦:artist]唱的[青花瓷:title]"){
        ret = me_feat_extr2->get_feat(feat, "[周杰伦:artist]唱的[青花瓷:title]");
		for (int i = 0; i < feat.feats.size(); ++i){
			result += feat.feats[i] + "/";
		}
		REQUIRE(ret == 0);
		REQUIRE(result == "周杰/杰伦/伦唱/唱的/的青/青花/花瓷/周杰伦/杰伦唱/伦唱的/唱的青/的青花/青花瓷/周杰伦唱/杰伦唱的/伦唱的青/唱的青花/的青花瓷/");
    }
	SECTION("feat3, pattern+分词, query为空"){
        query = "";
        ret = me_feat_extr3->get_feat(feat, query);
		REQUIRE(ret < 0);
    }
    SECTION("feat3, pattern+分词, 周杰伦的歌"){
        ret = me_feat_extr3->get_feat(feat, "周杰伦的歌");
		for (int i = 0; i < feat.feats.size(); ++i){
			result += feat.feats[i] + "/";
		}
		REQUIRE(ret == 0);
		REQUIRE(result == "周杰伦的/的歌/周杰伦的歌/");
    }
	SECTION("feat3, pattern+分词, 周杰伦唱的[青花瓷:title]"){
        ret = me_feat_extr3->get_feat(feat, "周杰伦唱的[青花瓷:title]");
		for (int i = 0; i < feat.feats.size(); ++i){
			result += feat.feats[i] + "/";
		}
		REQUIRE(ret == 0);
		REQUIRE(result == "周杰伦唱/唱的/的[title]/周杰伦唱的/唱的[title]/周杰伦唱的[title]/");
    }
	
    SECTION("feat4, pattern+不分词, 周杰伦的歌"){
        ret = me_feat_extr4->get_feat(feat, "周杰伦的歌");
		for (int i = 0; i < feat.feats.size(); ++i){
			result += feat.feats[i] + "/";
		}
		REQUIRE(ret == 0);
		REQUIRE(result == "周杰/杰伦/伦的/的歌/周杰伦/杰伦的/伦的歌/周杰伦的/杰伦的歌/");
    }
	SECTION("feat4, pattern+不分词, [周杰伦:artist]唱的[青花瓷:title]"){
        ret = me_feat_extr4->get_feat(feat, "[周杰伦:artist]唱的[青花瓷:title]");
		for (int i = 0; i < feat.feats.size(); ++i){
			result += feat.feats[i] + "/";
		}
		REQUIRE(ret == 0);
		REQUIRE(result == "[artist]唱/唱的/的[title]/[artist]唱的/唱的[title]/[artist]唱的[title]/");
    }
}
