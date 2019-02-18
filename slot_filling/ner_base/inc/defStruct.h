/*************************************************************************
	> File Name: defStruct.h
	> Author: TengyueMa
	> Mail: lumujin@icloud.com
	> Created Time: Wed 21 Mar 2018 01:31:48 PM CST
 ************************************************************************/

#ifndef _DEFSTRUCT_H
#define _DEFSTRUCT_H

#include <string>
#include <vector>

using namespace std;

//< 分类信息
struct clf_info{
    //< 类别
    string classes;
    //< 分值
    float score;
};

//< 槽位信息
struct _slot{
    //< 标签名
    string name;
    //< 内容
    string value;
    //< 起止位置
    int start;
    int end;
};

//< 每个解析结果
struct _form{
    //< 领域
    clf_info domain;
    //< 意图
    clf_info intent;
    //< 槽位
    vector<_slot> slots;
};

#endif
/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
