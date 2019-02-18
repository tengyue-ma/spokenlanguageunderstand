/*************************************************************************
	> File Name: OpenDomainFeatExtractor_UnitTest.cpp
	> Author: leilei.zhou
	> Mail: leilei.zhou@horizon.ai
	> Created Time: 2018.12.11
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
#include "OpenDomainFeatExtractor.h"
using namespace std;
using namespace feat_extractor;

extern "C" FeatExtractor* create_feat_extr(const string confpath,PreProcessor* pre);
extern "C" void delete_feat_extr(FeatExtractor* feat_extr);

TEST_CASE ("FeatExtractor", "open_domain_feat") {
    
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
    string conf1 = "openDomain1.conf";
	string conf2 = "openDomain2.conf";
	OpenDomainFeatExtractor* open_domain_feat_extr1 = new OpenDomainFeatExtractor(conf1, pre);
	//OpenDomainFeatExtractor* open_domain_feat_extr2 = new OpenDomainFeatExtractor(conf2, pre);
	
    SECTION("测试create_feat_extr 和 delete_feat_extr"){
		FeatExtractor* feat_ext = create_feat_extr(conf1, pre);
        REQUIRE_NOTHROW(create_feat_extr(conf1, pre));
		REQUIRE_NOTHROW(delete_feat_extr(feat_ext));
    }
	
	SECTION("测试构造函数和析构函数"){
        OpenDomainFeatExtractor* open_domain_feat_extr_test = new OpenDomainFeatExtractor(conf1, pre);
        REQUIRE_NOTHROW(delete open_domain_feat_extr_test);
    }
	
	SECTION("feat1, query为空"){
        ret = open_domain_feat_extr1->get_feat(feat, "");
		REQUIRE(ret == 0);
    }
	SECTION("feat1, nh_person, 周杰伦是谁"){
        ret = open_domain_feat_extr1->get_feat(feat, "周杰伦是谁");
		for (int i = 0; i < feat.feats.size(); ++i){
			result += feat.feats[i] + "/";
		}
		REQUIRE(ret == 0);
		REQUIRE(result == "nh_person 周杰伦/O 是/O 谁/");
    }
	SECTION("feat1, rh_person, 放一首他的歌"){
        ret = open_domain_feat_extr1->get_feat(feat, "放一首他的歌");
		for (int i = 0; i < feat.feats.size(); ++i){
			result += feat.feats[i] + "/";
		}
		REQUIRE(ret == 0);
		REQUIRE(result == "O 放/O 一/O 首/rh_person 他/O 的/O 歌/");
    }
	SECTION("feat1, ns_location, 中国在哪里"){
        ret = open_domain_feat_extr1->get_feat(feat, "中国在哪里");
		for (int i = 0; i < feat.feats.size(); ++i){
			result += feat.feats[i] + "/";
		}
		REQUIRE(ret == 0);
		REQUIRE(result == "ns_location 中国/O 在/O 哪里/");
    }
	SECTION("feat1, ni_location, 北京大学在哪里"){
        ret = open_domain_feat_extr1->get_feat(feat, "北京大学在哪里");
		for (int i = 0; i < feat.feats.size(); ++i){
			result += feat.feats[i] + "/";
		}
		REQUIRE(ret == 0);
		REQUIRE(result == "ni_location 北京大学/O 在/O 哪里/");
    }
	SECTION("feat1, ni_location, 上海图书馆"){
        ret = open_domain_feat_extr1->get_feat(feat, "上海图书馆");
		for (int i = 0; i < feat.feats.size(); ++i){
			result += feat.feats[i] + "/";
		}
		REQUIRE(ret == 0);
		REQUIRE(result == "ni_location 上海图书馆/");
    }
	SECTION("feat1, rs_location, 那里天气怎么样"){
        ret = open_domain_feat_extr1->get_feat(feat, "那里天气怎么样");
		for (int i = 0; i < feat.feats.size(); ++i){
			result += feat.feats[i] + "/";
		}
		REQUIRE(ret == 0);
		REQUIRE(result == "rs_location 那里/O 天气/O 怎么样/");
    }
	SECTION("feat1, 词典city, 北京在哪里"){
        ret = open_domain_feat_extr1->get_feat(feat, "北京在哪里");
		for (int i = 0; i < feat.feats.size(); ++i){
			result += feat.feats[i] + "/";
		}
		REQUIRE(ret == 0);
		REQUIRE(result == "city 北京/O 在/O 哪里/");
    }
	SECTION("feat1, 词典city, 北京在哪里"){
        ret = open_domain_feat_extr1->get_feat(feat, "北京在哪里");
		for (int i = 0; i < feat.feats.size(); ++i){
			result += feat.feats[i] + "/";
		}
		REQUIRE(ret == 0);
		REQUIRE(result == "city 北京/O 在/O 哪里/");
    }
	//slotType.cpp
	SECTION("测试构造函数和析构函数, 词典空"){
		slotType* slot_type = new slotType();
        REQUIRE_NOTHROW(delete slot_type);
    }
	SECTION("测试构造函数和析构函数, 词典存在"){
		vector<pair<string, string> > vec_dict;
		vec_dict.push_back(pair<string, string>("city", "city.txt"));
		slotType* slot_type = new slotType(vec_dict);
        REQUIRE_NOTHROW(delete slot_type);
    }
	SECTION("测试构造函数和析构函数, 词典不存在"){
		vector<pair<string, string> > vec_dict;
		vec_dict.push_back(pair<string, string>("city", "notexist.txt"));
		slotType* slot_type = new slotType(vec_dict);
        REQUIRE_NOTHROW(delete slot_type);
    }
}
