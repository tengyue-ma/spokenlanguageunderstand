/*************************************************************************
	> File Name: WorkFlow_UnitTest.cpp
	> Author: Liang Haoqiang
	> Mail: Haoqiang.liang@horizon.ai
	> Created Time: Sat Dec 15 16:17:41 2018
 ************************************************************************/

#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include <iostream>
#include "WorkFlow.h"
#include "SluErrorCode.h"

using namespace std;

TEST_CASE("WorkFlow","workflow"){

    string conf_file = "../data/conf/workflow.conf";

    SECTION("测试构造函数和析构函数"){
        WorkFlow * wf = new WorkFlow();
        REQUIRE_NOTHROW(delete wf);
    }
    
    SECTION("测试load_conf"){
        WorkFlow wf;
        int ret =  wf.load_conf(conf_file,"./");
        REQUIRE(ret == 0);
        REQUIRE(wf.workflow.size() == 5);
        REQUIRE(wf.workflow[0].func_type == "REGEX_GLOBAL");
        REQUIRE(wf.workflow[1].params.size() == 3);
        REQUIRE(wf.workflow[1].output == "domain");
    }

    SECTION("测试conf_file为空"){
        WorkFlow wf;
        int ret = wf.load_conf("");
        REQUIRE(ret == ERR_WORK_FLOW_CONF_NULL);
    }

}
/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
