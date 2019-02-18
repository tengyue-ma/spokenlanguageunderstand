/*************************************************************************
	> File Name: DomainManage.cpp
	> Author: TengyueMa
	> Mail: lumujin@icloud.com
	> Created Time: Mon 08 Oct 2018 11:43:15 AM CST
 ************************************************************************/

#include <iostream>
#include "DomainManage.h"
#include "ul_log.h"
#include "Configure.h"
#include "spreg.h"
#include "SluErrorCode.h"

//< 构造函数
DomainManage::DomainManage(const string conf_file){
    int ret = load_conf(conf_file);
    if (ret < 0){
        return;
    }
}

//< 析构函数
DomainManage::~DomainManage(){
    //< nothing;
}

//< 读取配置文件
int DomainManage::load_conf(const string conf_file, const string conf_path){
    if (conf_file == ""){
        fprintf(stderr, "DomainManage conf is null\n");
        return ERR_DOMAIN_MANAGE_CONF_NULL;
    }
    comcfg::Configure config;
    int ret = config.load(conf_path.c_str(), conf_file.c_str());
    if (ret < 0){
        fprintf(stderr, "failed to load DomainManage config file\n");
        return ERR_DOMAIN_MANAGE_LOAD_CONF;
    }

    //< load Configure
    try {
        default_domain = config["DEFAULT_DOMAIN"].to_cstr();
        default_intent = config["DEFAULT_INTENT"].to_cstr();
        int domain_size = static_cast<int>(config["DOMAIN"][0]["DOMAIN"].size());
        for (int i = 0; i < domain_size; i++){
            domain_vec.push_back(config["DOMAIN"][0]["DOMAIN"][i].to_cstr());
        }
    } catch(...){
        fprintf(stderr, "failed to load DomainManage config file\n");
        return ERR_DOMAIN_MANAGE_CONF_CONTENT;
    }

    return 0;
}

//< 查询领域是否存在
//< 返回>0 为存在
int DomainManage::find_domain(string domain_name){
    for (int i = 0; i < domain_vec.size(); i++){
        if (domain_vec[i] == domain_name){
            return i+1;
        }
    }

    return 0;
}

//< 返回默认领域
string DomainManage::get_default_domain(){
    if (default_domain == ""){
        return "other";
    }
    return default_domain;
}

//< 返回默认意图
string DomainManage::get_default_intent(){
    if (default_intent == ""){
        return "other";
    }
    return default_intent;
}

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
