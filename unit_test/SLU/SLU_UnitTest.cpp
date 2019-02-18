/*************************************************************************
	> File Name: SLU_UnitTest.cpp
	> Author: haoqiang.liang
	> Mail: haoqiang.liang@horizon.ai
	> Created Time: Tue Dec 18 14:04:10 2018
 ************************************************************************/

#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include <iostream>
#include "semantic.h"
#include "SemanticInfo.h"
#include "SlotFilling.h"
#include "spregex.h"
#include "SLU.h"
#include "SluLog.h"

using namespace std;
using namespace slu;
using SemanticRegex::Regex;
using SemanticRegex::Semantic;

TEST_CASE("SLU", "workflow.conf"){
    
    SLU* slu = new SLU();

    string workflow_config = "../data/conf/workflow.conf";

    //< init slu

    //< load WorkFlow
    WorkFlow workflow;
    int  ret = workflow.load_conf(workflow_config);
    REQUIRE(ret == 0);

    ret = slu->init_slu("../data/conf/resource.conf");
    REQUIRE(ret == 0);

    ret = slu->set_workflow(workflow);
    REQUIRE(ret == 0);
    
    
    SECTION("测试semantic_interface"){

        string query = "周杰伦的歌";
        string  id = "horizon&liang&123456";
        
        SemanticInfo sem_info;
        int ret = slu->semantic_interface(query, sem_info, id);
        REQUIRE(ret == 0);

        vector<string> seq_vec_res = {"周","杰","伦","的","歌" };
        REQUIRE(sem_info.get_query()=="周杰伦的歌");

        vector<string> domain = {"music"};
        REQUIRE(sem_info.get_domain() == domain);
    
    }

    SECTION("测试析构函数"){
        REQUIRE_NOTHROW(delete slu);
    }
}


TEST_CASE("CLF_FIXED","workflow2.conf"){
    
    SLU* slu = new SLU();
    string workflow_config = "workflow2.conf";

    WorkFlow workflow;
    workflow.load_conf(workflow_config);

    slu->init_slu("../data/conf/resource.conf");
    string query = "今天南京天气怎么样";
    string  id = "horizon&liang&123456";
    
    slu->set_workflow(workflow);

    SemanticInfo sem_info, sem_info2;
    int ret = slu->semantic_interface(query, sem_info, id);
    REQUIRE(ret == 0);

    REQUIRE(sem_info.get_query()=="今天南京天气怎么样");
    vector<string> domain = {"other"};
    REQUIRE(sem_info.get_domain() == domain);
    
    
    ret =  slu->semantic_interface("帮我导航到栖霞区",sem_info2,id);
    REQUIRE(ret == 0);
    vector<string> domain2 = {"other"};
    REQUIRE(sem_info.get_domain() == domain2);

}

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
