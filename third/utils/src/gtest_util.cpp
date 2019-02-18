/*************************************************************************
	> File Name: gtest_util.cpp
	> Author: TengyueMa
	> Mail: lumujin@icloud.com
	> Created Time: 六  1/20 15:45:46 2018
 ************************************************************************/

#include <limits>
#include <iostream>
#include "utils.h"
#include "gtest/gtest.h"

using namespace std;
using namespace utils;

//< test utf8strSplit
TEST(util, strSplit2simple){
    string strs[] = {"你", "叫", "什", "么", "名", "字", "?"};
    vector<string> str_vec(&strs[0], &strs[7]);
    vector<string> str_res;
    bool flag = strSplit2simple("你叫什么名字?", str_res);
    EXPECT_EQ(str_vec, str_res);
    EXPECT_EQ(true, flag);
}

//< test strSplitbysep
TEST(util, strSplitbysep){
    string strs[] = {"你叫", "什么", "名字"};
    vector<string> str_vec(&strs[0], &strs[3]);

    vector<string> str_res;
    strSplitbysep("你叫.什么.名字.", str_res, ".");
    EXPECT_EQ(str_vec, str_res);
    
    strSplitbysep(".你叫.什么.名字.", str_res, ".");
    EXPECT_EQ(str_vec, str_res);

    strSplitbysep("..你叫.什么.名字..", str_res, ".");
    EXPECT_EQ(str_vec, str_res);
    
    strSplitbysep("..你叫.什么...名字.", str_res, ".");
    EXPECT_EQ(str_vec, str_res);
    
    bool flag = strSplitbysep(";;你叫;;什么;;名字;;", str_res, ";;");
    EXPECT_EQ(str_vec, str_res);
    EXPECT_EQ(true, flag);
    
    vector<string> str_vec1;
    str_vec1.push_back("x");
    str_vec1.push_back("4");
    strSplitbysep("x:4", str_res, ":");
    EXPECT_EQ(str_vec1, str_res);
}

//< test string2int
TEST(util, string2int){
    string str = "123213";
    EXPECT_EQ(123213, string2int(str));
}

//< test string2float
TEST(util, string2float){
    string str = "0.123";
    float num = 0.123;
    EXPECT_EQ(num, string2float(str));
}

//< test ner_decode
TEST(util, ner_decode){
    vector<pair<string, string> > slot_list_res;
    slot_list_res.push_back(pair<string, string>("artist", "刘德华"));
    slot_list_res.push_back(pair<string, string>("title", "冰雨"));
    vector<pair<string, string> > slot_list;
    vector<string> tag_vec;
    tag_vec.push_back("artist+b");
    tag_vec.push_back("artist+i");
    tag_vec.push_back("artist+e");
    tag_vec.push_back("O");
    tag_vec.push_back("title+b");
    tag_vec.push_back("title+e");
    vector<string> seq_vec;
    seq_vec.push_back("刘");
    seq_vec.push_back("德");
    seq_vec.push_back("华");
    seq_vec.push_back("的");
    seq_vec.push_back("冰");
    seq_vec.push_back("雨");

    ner_decode(slot_list, tag_vec, seq_vec);

    EXPECT_EQ(slot_list_res, slot_list);
}

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
