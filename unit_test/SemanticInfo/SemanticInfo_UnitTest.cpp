/*************************************************************************
	> File Name: SemanticInfo_UnitTest.cpp
	> Author: haoqiang.liang
	> Mail: haoqiang.liang@horizon.ai
	> Created Time: Mon Dec 17 16:57:21 2018
 ************************************************************************/

#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include <iostream>
#include "SemanticInfo.h"
#include "utils.h"
#include "WorkFlow.h"
#include "defStruct.h"
#include "SluErrorCode.h"

using namespace std;

 TEST_CASE("WorkFlow","semanticInfo"){
    
    string query = "周杰伦的七里香";

    //< 实例化一个workflow，以便实例化loginstance
    WorkFlow workflow;
    int ret =  workflow.load_conf("../data/conf/workflow.conf");
    REQUIRE(ret == 0);

    //< 构造tags
    vector<string> tags;
    tags.push_back("artist+b");
    tags.push_back("artist+i");
    tags.push_back("artist+e");
    tags.push_back("0");
    tags.push_back("title+b");
    tags.push_back("title+i");
    tags.push_back("title+e");

     //<  构造slots 结果
    vector<_slot> slots_res;
     _slot t1,t2;
    t1.name = "artist";
    t1.value = "周杰伦";
    t1.start = 0;
    t1.end = 2;
    slots_res.push_back(t1);

    t2.name = "title";
    t2.value = "七里香";
    t2.start = 4;
    t2.end  = 6;
     slots_res.push_back(t2);

    //< 构造domain_vec
    vector<pair<string, float>> domain_vec = {
        pair<string, float>("music", 1.0)
    };

    clf_info domain = {"music", 1.0};
    vector<clf_info> domain_vec2 = { domain};

    //< 构造intent
    vector<pair<string, float>> intent_vec = {
        pair<string, float>("inform", 0.8),
        pair<string, float>("confirm", 0.2)
    };
    clf_info intent = {"inform", 0.8};
    clf_info intent2 = {"confirm", 0.2};

    //< 构造form & forms
    _form form = {
        domain,
        intent,
        vector<_slot>(slots_res)
    };
    _form form2 = {
        domain,
        intent2,
        vector<_slot>(slots_res)
    };
    vector<_form> forms ={form, form2};


    SECTION("测试构造函数和析构函数"){
        SemanticInfo * si = new SemanticInfo();
        REQUIRE_NOTHROW(delete si);
    }

    SECTION("测试set_query"){
        SemanticInfo*  sem_info = new SemanticInfo();
        int ret = sem_info->set_query("周杰伦的七里香");

        REQUIRE(ret == 0);
        REQUIRE("周杰伦的七里香" == sem_info->get_query());
        delete sem_info;
    }

    SECTION("测试set_null_slots"){
        SemanticInfo sem_info;
        sem_info.set_query(query);

        int ret =  sem_info.set_null_slots(tags);
        REQUIRE(ret == 0);

        ret = sem_info.set_null_slots(slots_res);
        REQUIRE(ret == 0);
    }

    SECTION("测试set get_domain"){
        SemanticInfo sem_info;
        int ret = sem_info.set_domain(domain_vec);
        REQUIRE(ret == 0);
        
        vector<string> domains = sem_info.get_domain();
        REQUIRE(domains[0] == "music");

        ret = sem_info.set_domain(domain_vec2);
        REQUIRE(ret == 0);
    }

    SECTION("测试get set_intent"){
        SemanticInfo sem_info;
        sem_info.set_query(query);
        sem_info.set_domain(domain_vec);
        int ret = sem_info.set_intent("music", intent_vec);
        REQUIRE(ret == 0);

        string intent_str  = sem_info.get_intent("music");
        REQUIRE(intent_str == "inform");

        ret = sem_info.set_intent("movie", intent_vec);
        REQUIRE(ret == ERR_SEMANTIC_INFO_SET_INTENT);

        ret = sem_info.set_intent("music", intent);
        REQUIRE(ret == 0);
        
        ret = sem_info.set_intent("movie", intent);
        REQUIRE(ret == ERR_SEMANTIC_INFO_SET_INTENT);
    }

    SECTION("测试set_slots"){
        SemanticInfo sem_info;
        sem_info.set_query(query);
        sem_info.set_null_slots(tags);
        sem_info.set_domain(domain_vec);
        sem_info.set_intent("music", intent_vec);
        
        int ret = sem_info.set_slots("music", slots_res);
        REQUIRE(ret == 0);

        ret = sem_info.set_slots("movie", slots_res);
        REQUIRE(ret == ERR_SEMANTIC_INFO_SET_SLOTS);

        ret = sem_info.set_slots("music", tags);
        REQUIRE(ret == 0);

        ret = sem_info.set_slots("movie", tags);
        REQUIRE(ret == ERR_SEMANTIC_INFO_SET_SLOTS);
    }

    SECTION("测试set_form"){
        SemanticInfo sem_info;
        int ret = sem_info.set_form(form);
        REQUIRE(ret == 0);

        ret = sem_info.set_forms(forms);
        REQUIRE(ret == 0);
    }

    SECTION("测试set_match_mode"){
        SemanticInfo sem_info;
        int ret = sem_info.set_match_mode("model");
        REQUIRE(ret == 0);
    }

    SECTION("测试get domain, intent, slots, forms match_mode ner_query"){
        SemanticInfo sem_info;
        sem_info.set_query(query);
        sem_info.set_null_slots(tags);
        sem_info.set_domain(domain_vec);
        sem_info.set_intent("music", intent_vec);
        sem_info.set_slots("music", slots_res);
        sem_info.set_match_mode("model");

        vector<_slot> slots =  sem_info.get_slots("null");
        REQUIRE(slots[0].name == "artist");
        REQUIRE(slots[1].name == "title");

        slots = sem_info.get_slots("music");
         REQUIRE(slots[0].name == "artist");
        REQUIRE(slots[1].name == "title");


        vector<_form> forms = sem_info.get_forms();
        REQUIRE(forms[0].domain.classes == "music");

        string model = sem_info.get_match_mode();
        REQUIRE(model == "model");
    }

    SECTION("测试get_ner_query"){
        SemanticInfo sem_info;
        sem_info.set_query(query);
        sem_info.set_null_slots(tags);
        string ner_query = sem_info.get_ner_query();
        REQUIRE(ner_query == "[周杰伦:artist]的[七里香:title]");
    }
 }
/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
