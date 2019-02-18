/*************************************************************************
	> File Name: OneHotFeatExtractor_UnitTest.cpp
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
#include "OneHotFeatExtractor.h"
using namespace std;
using namespace feat_extractor;

extern "C" FeatExtractor* create_feat_extr(const string confpath,PreProcessor* pre);
extern "C" void delete_feat_extr(FeatExtractor* feat_extr);

TEST_CASE ("FeatExtractor", "onehot_feat") {
    
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
	OneHotFeatExtractor* onehot_feat_extr1 = new OneHotFeatExtractor(conf1, pre);
	OneHotFeatExtractor* onehot_feat_extr2 = new OneHotFeatExtractor(conf2, pre);
	OneHotFeatExtractor* onehot_feat_extr3 = new OneHotFeatExtractor(conf3, pre);
	OneHotFeatExtractor* onehot_feat_extr4 = new OneHotFeatExtractor(conf4, pre);
	
    SECTION("测试create_feat_extr 和 delete_feat_extr"){
		FeatExtractor* feat_ext = create_feat_extr(conf1, pre);
        REQUIRE_NOTHROW(create_feat_extr(conf1, pre));
		REQUIRE_NOTHROW(delete_feat_extr(feat_ext));
    }
	
	SECTION("测试构造函数和析构函数"){
        OneHotFeatExtractor* onehot_feat_extr_test = new OneHotFeatExtractor(conf1, pre);
        REQUIRE_NOTHROW(delete onehot_feat_extr_test);
    }
	
	SECTION("feat1, query为空"){
        ret = onehot_feat_extr1->get_feat(feat, "");
		REQUIRE(ret == 0);
    }
	SECTION("feat1, query+分词, 张国荣/演唱/的/我/这/首/歌"){
        ret = onehot_feat_extr1->get_feat(feat, "张国荣演唱的我这首歌");
		for (int i = 0; i < feat.feats.size(); ++i){
			result += feat.feats[i] + "/";
		}
		REQUIRE(ret == 0);
		REQUIRE(result == "0/0/0/0/0/0/0/0/0/0/0/0/0/0/0/0/0/0/0/0/0/0/0/0/0/0/0/0/0/0/0/0/0/0/0/0/0/0/0/0/0/0/0/19/20/6/7/8/9/10/");
    }
	
    SECTION("feat2, query+不分词, [周杰伦:artist]/演/唱/的/[青花瓷:title]"){
        ret = onehot_feat_extr2->get_feat(feat, "[周杰伦:artist]演唱的[青花瓷:title]");
		for (int i = 0; i < feat.feats.size(); ++i){
			result += feat.feats[i] + "/";
		}
		REQUIRE(ret == 0);
		REQUIRE(result == "0/0/0/0/0/0/0/0/0/0/0/0/0/0/0/0/0/0/0/0/0/0/0/0/0/0/0/0/0/0/0/0/0/0/0/0/0/0/0/0/0/11/12/13/4/5/6/14/15/16/");
    }
    SECTION("feat3, pattern+分词, [周杰伦:artist]/演唱/的/[青花瓷:title]"){
        ret = onehot_feat_extr3->get_feat(feat, "[周杰伦:artist]演唱的[青花瓷:title]");
		for (int i = 0; i < feat.feats.size(); ++i){
			result += feat.feats[i] + "/";
		}
		REQUIRE(ret == 0);
		REQUIRE(result == "0/0/0/0/0/0/0/0/0/0/0/0/0/0/0/0/0/0/0/0/0/0/0/0/0/0/0/0/0/0/0/0/0/0/0/0/0/0/0/0/0/0/0/0/0/0/17/20/6/18/");
    }
    SECTION("feat4, pattern+不分词+VECTOR+不存在的字, [青花瓷:title]呀"){
        ret = onehot_feat_extr4->get_feat(feat, "[青花瓷:title]呀");
		for (int i = 0; i < feat.feats.size(); ++i){
			result += feat.feats[i] + "/";
		}
		REQUIRE(ret == 0);
		REQUIRE(result == "0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1 0 0 0 0 /1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 /");
    }
	SECTION("feat4, pattern+不分词+VECTOR+超过定义长度, [青花瓷:title]呀好不好"){
        ret = onehot_feat_extr4->get_feat(feat, "[青花瓷:title]呀好不好");
		for (int i = 0; i < feat.feats.size(); ++i){
			result += feat.feats[i] + "/";
		}
		REQUIRE(ret == 0);
		REQUIRE(result == "0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1 0 0 0 0 /1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 /");
    }
}
