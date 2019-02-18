/*************************************************************************
	> File Name: WorkFlow.h
	> Author: TengyueMa
	> Mail: lumujin@icloud.com
	> Created Time: 一  2/26 11:12:21 2018
 ************************************************************************/

#ifndef _WORKFLOW_H
#define _WORKFLOW_H

using std::vector;
using std::string;

typedef struct _WORKSTEP{
    //< 函数类型
    string func_type;
    //< void类型指针 （用于某些特殊情况）
    void * _void;
    //< 参数列表
    vector<string> params;
    //< 返回结果
    string output;
   
    //< 构造函数
    _WORKSTEP(){
        _void = NULL;
    }
}_workstep;

class WorkFlow{
public:
    /*
     * @brief 构造函数
     */
    WorkFlow();

    /*
     * @brief 析构函数
     */
    ~WorkFlow();

public:
    /*
     * @brief 读取配置文件&解析存储
     */
    int load_conf(const string conf_file, 
                  const string conf_path = "./");

public:
    vector<_workstep> workflow;
};

#endif
/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
