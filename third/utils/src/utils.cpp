/*************************************************************************
	> File Name: utils.cpp
	> Author: TengyueMa
	> Mail: lumujin@icloud.com
	> Created Time: 六  1/20 15:58:13 2018
 ************************************************************************/

#include <iostream>
#include <vector>
#include <string>
#include <stdlib.h>
#include <sstream>
#include <boost/locale/encoding_utf.hpp>
#include "utils.h"

using namespace std;
using boost::locale::conv::utf_to_utf;

//判断是否为数学的 + - × ÷
#define ismathoperator(ch) (ch == 43 || ch == 45 || ch == 215 || ch == 247)

namespace utils{
//< 字符串单字分割
//< @param str : 待分割字符串
//< @param split_strs : 分割后的结果
//< @param type : 编码类型 utf8 / gbk 默认为utf8
bool strSplit2simple(const string str, 
                     vector<string>& split_strs, 
                     const string& type){
    //< 非utf8 or gbk return false
    bool flag = false;
    vector<string> char_vec;
    if (type == "UTF-8" || type == "UTF8" || type == "utf8" || type == "utf-8"){
        /*
         * 基本定义, 勿动
         * 为了保证线程安全，禁止使用全局变量
         */ 
        const int count = 5;
        const int prefix[5] = {0x80, 0xF8, 0xF0, 0xE0, 0xC0};
        const int result[5] = {0, 0xF8, 0xF0, 0xE0, 0xC0};
        const int index[5] = {1, 5, 4, 3, 2};
        /*
         * -----分割线-----
         */

        for (size_t i = 0; i < str.size(); ){
            char chr = str[i];
            //< 单字判断
            for (int j = 0; j < count; j++){
                if ((chr & prefix[j]) == result[j]){
                    string strChar = str.substr(i, index[j]);
                    char_vec.push_back(strChar);
                    i += index[j];
                    break;
                }
            }
        }

        flag = true;
    }
    else if (type == "GBK" || type == "gbk"){
        for (size_t i = 0; i < str.size(); ){
            char chr = str[i];
            if ((chr & 0x80) == 0){
                string strChar = str.substr(i, 1);
                char_vec.push_back(strChar);
                i++;
            }
            else {
                // if (chr >= 161 && chr <= 247 && str[i+1] >= 161 && str[i+1] <= 254) 此行代码并不能解决问题
                string strChar = str.substr(i, 2);
                char_vec.push_back(strChar);
                i+=2;
            }
        }
        flag = true;
    }

    split_strs = char_vec;

    return flag;
}

//< 按照分隔符分割
//< @param str : 待分割字符串
//< @param split_strs : 分割结果
//< @param sep : 分隔符字符串
bool strSplitbysep(const string str, 
                   vector<string>& split_strs, 
                   const string& sep){
    //< 分割成功返回true 否则返回false
    bool flag = false;
    string str_c = str;

    int pos = str_c.find(sep);
    vector<string> str_vec;
    while(pos != -1){
        //< 获取从0-pos的字符串
        string str_res = str_c.substr(0, pos);
        if (str_res != ""){
            str_vec.push_back(str_res);
        }
        //< 删除0-pos & sep字符串
        str_c.erase(0, pos+sep.size());
        pos = str_c.find(sep);
    }
    if (str_c != ""){
        str_vec.push_back(str_c);
    }

    split_strs = str_vec;

    if (split_strs.size() > 1){
        flag = true;
    }

    return flag;
}

int rm_blank(string &s)
{
    int index = 0;
    if(!s.empty()) {
        while( (index = s.find(' ',index)) != string::npos) {
            s.erase(index,1);
        }
        while( (index = s.find('\t',index)) != string::npos) {
            s.erase(index,1);
        }
    }
    return 0;
}

//< string 转 int float
//< @param str : 待转换的string
int string2int(const string& str){
    return atoi(str.c_str());
}

float string2float(const string& str){
    istringstream iss(str);
    float num;
    iss >> num;

    return num;
}

//< ner decode
int ner_decode(vector<pair<string, string> >& slot_list, 
               vector<std::pair<int, int> >& slot_range, 
               const vector<string>& tag_vec, 
               const vector<string>& seq_vec){
    size_t count = tag_vec.size();
    if (count != seq_vec.size()){
        //< error.
        return -1;
    }

    vector<string> split_strs;
    string name = "";
    int idx = 0;
    int len = 0;
    for (size_t i = 0; i < count; i++){
        //< 如果tag为O
        if (tag_vec[i] == "O"){
            continue;
        }
        //< 对tag分割 获取尾部字符
        strSplitbysep(tag_vec[i], split_strs, "+");
        if (split_strs.size() <= 0){
            return -1;
        }
        //< 如果后缀为+b
        if (split_strs[split_strs.size()-1] == "b"){
            name = "";
            //< name赋值为前缀
            size_t j = 0;
            while(j < split_strs.size()-1){
                name += split_strs[j];
                if (j+1 < split_strs.size()-1){
                    name += "+";
                }
                j++;
            }
            idx = i;
            len++;
        }
        //< 如果后缀为+i
        else if ((split_strs[split_strs.size()-1] == "i" || split_strs[split_strs.size()-1] == "m") && len > 0){
            len++;
        }
        //< 如果后缀为+e
        else if (split_strs[split_strs.size()-1] == "e" && len > 0){
            len++;
            string value;
            for (int k = 0; k < len && k+idx < seq_vec.size(); k++){
                value += seq_vec[k+idx];
            }
            slot_list.push_back(pair<string, string>(name, value));
            slot_range.push_back(pair<int, int>(idx, idx+len-1));
            name = "";
            idx = 0;
            len = 0;
        }
        //< 如果后缀为+s
        else if (split_strs[split_strs.size()-1] == "s" && len == 0){
            string str;
            //< 单字词name
            size_t j = 0;
            while(j < split_strs.size()-1){
                str += split_strs[j];
                if (j+1 < split_strs.size()-1){
                    str += "+";
                }
                j++;
            }

            slot_list.push_back(pair<string, string>(str, seq_vec[i]));
            slot_range.push_back(pair<int, int>(i, i));
        }
    }

    if (name != ""){
        string value;
        for (size_t i = idx; i < seq_vec.size(); i++){
            value += seq_vec[i];
        }
        slot_list.push_back(pair<string, string>(name, value));
        slot_range.push_back(pair<int, int>(idx, seq_vec.size()-1));
    }

    return 0;
}

std::wstring utf8_to_wstring(const string &str) {
    return utf_to_utf<wchar_t>(str.c_str(), str.c_str() + str.size());
}

string wstring_to_utf8(const std::wstring &str) {
    return utf_to_utf<char>(str.c_str(), str.c_str() + str.size());
}

string rm_punctuation(string query_string) {
    // 转换成宽字节类型
    std::wstring ws = utf8_to_wstring(query_string);
    std::wstring nws;
    // 过滤每一行中的标点和空格
    for (wchar_t ch : ws) {
        // 检查是否是标点 中文 + - × ÷ 除外
        std::locale loc("en_US.UTF-8");
        if ((!ispunct(ch, loc) || ismathoperator(ch)) && !isspace(ch, loc)) {
            nws.push_back(ch);
        }
    }
    return wstring_to_utf8(nws);
}

} //< namespace utils
/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
