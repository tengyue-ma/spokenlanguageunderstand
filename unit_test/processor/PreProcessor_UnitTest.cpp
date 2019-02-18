/*************************************************************************
	> File Name: PreProcessor_UnitTest.cpp
	> Author: haoqiang.liang
	> Mail: haoqiang.liang@horizon.ai
	> Created Time: Wed Dec 12 20:12:19 2018
 ************************************************************************/

#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include <iostream>
#include "PreProcessor.h"

using namespace std;

TEST_CASE("Processor","PreProcessor"){
    PreProcessor * pre = new PreProcessor();

    // 初始化
    pre->create_wsg_engine("../../data/model/ltp_model/cws.model");
    pre->create_pos_engine("../../data/model/ltp_model/pos.model");
    pre->create_ner_engine("../../data/model/ltp_model/ner.model");
    pre->create_pas_engine("../../data/model/ltp_model/parser.model");
    
    // input query
    string query = "播放周杰伦的七里香";

    // expect output;
    vector<string> ws_vec_res={"播放","周杰伦","的","七里香"};
    vector<string> pos_vec_res = {"v","nh","u","n" };

    string strs_ner[] = {"O", "S-Nh", "O", "O" };
    vector<string> ner_vec_res(&strs_ner[0], &strs_ner[4]);

    vector<int> heads_vec_res = {0,4,2,1 };
    vector<string> pas_vec_res = {"HED","ATT","RAD","VOB"};

    // output
    vector<string> ws_vec;
    vector<string> pos_vec;
    vector<string> ner_vec;
    vector<int> heads_vec;
    vector<string> pas_vec;

    pre->wordseg(query, ws_vec);
    pre->postag(ws_vec, pos_vec);
    pre->ner(ws_vec,pos_vec,ner_vec);
    pre->parse(ws_vec,pos_vec,heads_vec,pas_vec);

    SECTION("测试预处理结果"){
        REQUIRE(ws_vec_res == ws_vec);
        REQUIRE(pos_vec_res == pos_vec);
        REQUIRE(ner_vec_res == ner_vec);
        REQUIRE(heads_vec_res == heads_vec);
        REQUIRE(pas_vec_res == pas_vec);
    }
    // 测试析构函数
    REQUIRE_NOTHROW(delete pre);

    SECTION("test 外部词典"){
        pre = new PreProcessor();

       REQUIRE_NOTHROW( pre->create_wsg_engine ("../data/model/ltp_model/cws.model", "wsg_dict_test.txt"));
       REQUIRE_NOTHROW( pre->create_pos_engine("../data/model/ltp_model/pos.model","pos_dict_test.txt"));
        pre->create_ner_engine("../data/model/ltp_model/ner.model");
        pre->create_pas_engine("../data/mode/ltp_model/parse.model");
            
        REQUIRE_NOTHROW(delete pre);

    }

}

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
