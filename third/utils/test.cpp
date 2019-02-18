//
// Created by zhouleilei on 18-5-23.
//
/*************************************************************************
	> File Name: test1.cpp
	> Author: speechteam
	> Mail: speech@hobot.cc
	> Created Time: Mon 09 Apr 2018 06:59:30 PM CST
 ************************************************************************/

#include <iostream>
#include <string.h>
#include <algorithm>
#include "utils.h"

using namespace std;
using namespace utils;

int main(int argc, char ** argv){

    while(true){
        //string query ="我   想 听 Beyond的光辉/<>?岁月，。、这首歌";
        string query;
        cin >> query;
        if (query == ""){
            break;
        }

        //< 去除空格和tab键
        rm_blank(query);

        //< 去除标点符号
        query = rm_punctuation(query);

        //< 大写字符转小写
        transform(query.begin(), query.end(), query.begin(), ::towlower);

        //< 小写字符转大写
        transform(query.begin(), query.end(), query.begin(), ::toupper);

        cout << query << endl;
    }

    return 0;
}


