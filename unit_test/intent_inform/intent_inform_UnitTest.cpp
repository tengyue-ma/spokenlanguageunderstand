/*************************************************************************
	> File Name: intent_inform_UnitTest.cpp
	> Author: haoqiang.liang
	> Mail: haoqiang.liang@horizon.ai
	> Created Time: Sat Dec 15 10:48:33 2018
 ************************************************************************/

#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include <iostream>
#include "intent_inform.h"
#include "Feature.h"
#include "FeatExtractor.h"

using namespace std;
using namespace classify;
using namespace feat_extractor;

extern "C" Classifier* create_classifier(const string model_path, const string log_path);
extern "C" void delete_classifier(Classifier* clf);

TEST_CASE("Classifier", "intent_inform"){
    
    string model_path = "inform";
    string log_path = "NULL";
    FeatExtractor* feat_extr = NULL;

    SECTION("测试create_classifier和delete_classifier"){
        Classifier* clf = create_classifier(model_path, log_path);
        REQUIRE_NOTHROW(delete_classifier(clf));
    }

    SECTION("测试classify"){
        Classifier* clf = create_classifier("music"," ");
        vector<pair<string, float>> class_name;
        clf->classify("周杰伦的歌",class_name, feat_extr);
        REQUIRE(class_name[0].first == "music");
        REQUIRE(class_name[0].second == 100);
    }

}

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
