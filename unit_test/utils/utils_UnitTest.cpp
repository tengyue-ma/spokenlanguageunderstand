/*************************************************************************
	> File Name: utils_UnitTest.cpp
	> Author: haoqiang.liang
	> Mail: haoqiang.liang@horizon.ai
	> Created Time: Wed Dec 19 14:52:40 2018
 ************************************************************************/

#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include<iostream>
#include "utils.h"
using namespace std;
using namespace utils;

TEST_CASE("utils_strSplit2simple", "strSplit2simple"){

    string str = "播放周杰伦的歌曲。";

    vector<string> res;
    vector<string> expect = {"播","放","周","杰","伦","的","歌","曲","。"};
    strSplit2simple(str,res);
    REQUIRE(res == expect); 

    int ret = strSplit2simple(str,res,"gbk");
    REQUIRE(ret == 1);
}


TEST_CASE("utils_strSplitbysep", "strSplitbysep"){

    string str = "播放-周杰伦-的-歌曲。";
    vector<string> res;
    vector<string> expect = {"播放","周杰伦","的","歌曲。"};
    strSplitbysep(str,res,"-");
    REQUIRE(res == expect);
}

TEST_CASE("utils_rm_blank","rm_blank"){

    string str = "播放 周杰 的歌 曲";
    string expect = "播放周杰的歌曲";
    rm_blank(str);
    REQUIRE(str == expect);
}

TEST_CASE("utils_string2int", "string2int"){

    string str = "12345";
    int value = 12345;
    REQUIRE(value == string2int(str));
}

TEST_CASE("utils_string2float", "string2float"){
    string str = "1234.567";
    float value = 1234.567;
    REQUIRE(value-string2float(str) < 1e-6);
}

TEST_CASE("utils_nerdecode)","ner_decode"){

    vector<string> seq_vec = {"放","周","杰","伦","的","七","里","香"};
    vector<string> tag_vec = {"intent+s","artist+b","artist+i","artist+e","O","title+b","title+i","title+e"};

    //< 构造slot_list
    vector<pair<string, string>> slot_list = {
        pair<string,string>("intent","放"),
        pair<string, string>("artist","周杰伦"),
        pair<string, string>("title","七里香")
    };

    //<构造slot_range
    vector<pair<int,int>> slot_range = {
        pair<int, int>(0,0),
        pair<int, int>(1, 3),
        pair<int,int>(5, 7)
    };

    vector<pair<string, string>> slot_list_res;
    vector<pair<int, int>> slot_range_res;
    int ret =  ner_decode(slot_list_res, slot_range_res, tag_vec,seq_vec);
    REQUIRE(ret == 0);
    REQUIRE(slot_list == slot_list_res);
    REQUIRE(slot_range == slot_range_res);
}

TEST_CASE("utils_rm_punctuation","rm_punctuation"){
    string str = "播：放，周杰？伦的歌.曲!!!!";
    string res = "播放周杰伦的歌曲";

    REQUIRE(res == rm_punctuation(str));

}
/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
