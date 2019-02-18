/*************************************************************************
	> File Name: DomainManage_UnitTest.cpp
	> Author: haoqiang.liang
	> Mail: haoqiang.liang@horizon.ai
	> Created Time: Mon Dec 17 15:13:28 2018
 ************************************************************************/

#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include <iostream>
#include "DomainManage.h"
#include "SluErrorCode.h"

using namespace std;

TEST_CASE("WorkFlow","domainManage"){

    string conf_file = "domain_manage.conf";
    DomainManage domainManage = DomainManage(conf_file);

    SECTION("测试构造函数和析构函数"){
        DomainManage* dm = new  DomainManage(conf_file);
        REQUIRE_NOTHROW(delete dm);
    }

    SECTION("测试读取conf_file"){
        DomainManage dm = DomainManage();
        int ret = dm.load_conf(conf_file);
        REQUIRE(ret == 0);
        REQUIRE(dm.default_domain == "other");
        REQUIRE(dm.default_intent == "other");
        REQUIRE(dm.domain_vec.size() == 2);
    }

    SECTION("测试conf_file为空"){
        DomainManage dm = DomainManage();
        int ret = dm.load_conf("");
        REQUIRE(ret == ERR_DOMAIN_MANAGE_CONF_NULL);
    }
    
    SECTION("测试find_domain"){
        REQUIRE(domainManage.find_domain("music") > 0);
        REQUIRE(domainManage.find_domain("weather") == 0);
    }

    SECTION("测试get_default_domain"){
        REQUIRE("other" == domainManage.get_default_domain());
        domainManage.default_domain = "";
        REQUIRE("other" == domainManage.get_default_domain());
    }

    SECTION("测试get_default_intent"){
        REQUIRE("other" == domainManage.get_default_intent());
        domainManage.default_intent = "";
        REQUIRE("other" == domainManage.get_default_intent());
    }
}

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
