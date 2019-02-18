/*************************************************************************
	> File Name: slotType.cpp
	> Author: TengyueMa
	> Mail: lumujin@icloud.com
	> Created Time: 四  1/ 4 15:21:23 2018
 ************************************************************************/

#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <fstream>
#include "slotType.h"
#include "SluLog.h"

using namespace std;

//< 构造函数
slotType::slotType(){
    //loadDict2Map("city", "./dict/City.txt");
    //loadDict2Map("area", "./dict/Area.txt");
    //loadDict2Map("country", "./dict/Country.txt");
    //loadDict2Map("province", "./dict/Province.txt");
}

slotType::slotType(const vector<pair<string, string> > vec_dict){
    for (size_t i = 0; i < vec_dict.size(); i++){
        loadDict2Map(vec_dict[i].first, vec_dict[i].second);
    }
}

//< 析构函数
slotType::~slotType(){
    if(!dictmap.empty()){  
        //< 释放
        for(map<string, string>::iterator iter = dictmap.begin(); iter != dictmap.end(); iter++){  
            dictmap.erase(iter);  
        }  
        dictmap.clear();  
    }  
}

//< 查询key的slotname
string slotType::getSlotname(const string value, 
                             string name){
    if (dictmap.find(value) != dictmap.end()){
        return dictmap[value];
    }
	
	loginstance->info("[SLU]: get slot name success! name: " + name);
    
    return name;
}

//< 加载词典信息到map
bool slotType::loadDict2Map(const string slotname, 
                            const string dictpath){
    ifstream dictfile;
    //< 打开文件
    dictfile.open(dictpath.c_str());

    if (!dictfile){
		loginstance->error("[SLU]: failed to open dict! dict path: " + dictpath);
        return false;
    }
    
    //< load
    string slot_value;
    while (dictfile >> slot_value){
        dictmap[slot_value] = slotname;
    }
	
	loginstance->info("[SLU]: load dict to map success! dict path: " + dictpath);
    
    //< 关闭文件
    dictfile.close();

    return true;
}

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
