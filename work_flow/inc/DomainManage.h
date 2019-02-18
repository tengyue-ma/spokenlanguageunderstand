/*************************************************************************
	> File Name: DomainManage.h
	> Author: TengyueMa
	> Mail: lumujin@icloud.com
	> Created Time: Mon 08 Oct 2018 11:04:52 AM CST
 ************************************************************************/

#ifndef _DOMAINMANAGE_H
#define _DOMAINMANAGE_H

#include <string>
#include <vector>
#include <iostream>

using std::vector;
using std::string;

class DomainManage{
public:
    /*
     * @brief 构造函数
     */
    DomainManage(){};
    DomainManage(const string conf_file);

    /*
     * @brief 析构函数
     */
    ~DomainManage();

public:
    /*
     * @brief 读取配置文件
     */
    int load_conf(const string conf_file, 
                  const string conf_path = "./");
   
    /*
     * @brief 查询领域是否存在
     * @brief 返回>0 为存在，否则不存在
     */
    int find_domain(string domain_name);

    /*
     * @brief 返回默认领域
     */
    string get_default_domain();

    /*
     * @brief 返回默认意图
     */
    string get_default_intent();

public:
    //< 默认领域
    string default_domain;
    //< 默认意图
    string default_intent;
    //< 定义可用的领域列表
    vector<string> domain_vec;
};

#endif
/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
