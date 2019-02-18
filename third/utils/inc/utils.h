/*************************************************************************
	> File Name: utils.h
	> Author: TengyueMa
	> Mail: lumujin@icloud.com
	> Created Time: 六  1/20 14:52:33 2018
 ************************************************************************/

#ifndef _UTILS_H
#define _UTILS_H

#include <vector>
#include <string>

using std::vector;
using std::string;

namespace utils{
    /*
     * @brief 字符串单字分割
     * @param str:输入的字符串
     * @param split_strs:单字分割的结果
     * @param type:默认为utf8
     */
    bool strSplit2simple(const string str, 
                         vector<string>& split_strs, 
                         const string& type = "utf8");

    /*
     * @brief 按照分隔符分割
     * @param str:输入的字符串
     * @param split_strs:分割结果
     * @param sep:分隔符(字符串)
     */
    bool strSplitbysep(const string str, 
                       vector<string>& split_strs,
                       const string& sep);

    /*
     * @brief 去空格和tab键
     */
    int rm_blank(string &s);

    /*
     * @brief string 转 int
     */
    int string2int(const string& str);
    float string2float(const string& str);
	
    /*
     * @brief 去除UTF-8编码的英文标点符号,除了数学的+ - × ÷
     */
	string rm_punctuation(string query_string);

    /*
     * @brief ner decode
     * @param slot_list : 返回的slot列表
     * @param slot_range: 返回start end
     * @param tag_vec : 序列标注结果
     * @param seq_vec : 原始的句子
     */
    int ner_decode(vector<std::pair<string, string> >& slot_list, 
                   vector<std::pair<int, int> >& slot_range, 
                   const vector<string>& tag_vec, 
                   const vector<string>& seq_vec);

}
#endif
/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
