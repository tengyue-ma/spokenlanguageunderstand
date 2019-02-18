/*************************************************************************
	> File Name: NNClassifier_UnitTest.cpp
	> Author: haoqiang.liang
	> Mail: haoqiang.liang@horizon.ai
	> Created Time: Fri Dec 14 17:19:00 2018
 ************************************************************************/

#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include <iostream>
#include "NNClassifier.h"
#include "PreProcessor.h"
#include "OneHotFeatExtractor.h"
using namespace std;
using namespace classify;
using namespace feat_extractor;

extern "C" Classifier* create_classifier(const string model_conf_path, const string log_path);
extern "C" void delete_classifier(Classifier* clf);
extern "C" FeatExtractor* create_feat_extr(const string confpath, PreProcessor* pre);
extern "C" void delete_feat_extr(FeatExtractor* feat_extr);

TEST_CASE("Classifier", "NNClassifier"){
    
    //<初始化ltp
    PreProcessor* pre = new PreProcessor();
    pre->create_wsg_engine("../../data/model/ltp_model/cws.model");
    pre->create_pos_engine("../../data/model/ltp_model/pos.model");
    pre->create_ner_engine("../../data/model/ltp_model/ner.model");
    pre->create_pas_engine("../../data/model/ltp_model/parser.model");

    FeatExtractor* feat_extr = create_feat_extr("feat.conf", pre);
    FeatExtractor* feat_extr2 = create_feat_extr("feat2.conf", pre);

    string model_conf_path1 = "nn_model.conf" ;
    string model_conf_path2 = "nn_model2.conf";
    string log_path = "log_path.conf";
    string query = "周杰伦的歌";
    
    SECTION("测试构造函数和析构函数"){
        NNClassifier* clf = new NNClassifier(model_conf_path1, log_path);
        REQUIRE_NOTHROW(delete clf);
    }

    SECTION("测试create_classifier和delete_classifier"){
        Classifier *clf = create_classifier(model_conf_path1, log_path);
        REQUIRE_NOTHROW(delete_classifier(clf));
    }
    
    SECTION("使用mode_conf1 测试classify"){
        Classifier *clf = create_classifier(model_conf_path1, log_path);
        vector<pair<string, float>> class_name;
        clf->classify(query, class_name, feat_extr);
        REQUIRE(class_name[0].first == "other");
        REQUIRE(class_name[0].second - 1.0 < 1e-6 );

        delete_classifier(clf);
    }
    /*
    SECTION("使用model_conf2 测试classify"){
        Classifier *clf = create_classifier(model_conf_path1, log_path);
        vector<pair<string,float>> class_name;
        clf->classify(query, class_name, feat_extr2);
        REQUIRE(class_name[0].first == "other");
        REQUIRE(class_name[0].second >= 1.0 );

        delete_classifier(clf);
    }
    */
    
}

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
