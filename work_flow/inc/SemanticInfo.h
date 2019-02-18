/*************************************************************************
	> File Name: SemanticInfo.h
	> Author: TengyueMa
	> Mail: lumujin@icloud.com
	> Created Time: 一  2/ 5 19:21:29 2018
 ************************************************************************/

#ifndef _SEMANTICINFO_H
#define _SEMANTICINFO_H

#include <string>
#include <vector>
#include "defStruct.h"

using namespace std;

class SemanticInfo{
public:
    /*
     * @brief 构造函数
     */
    SemanticInfo(){};

    /*
     * @brief 析构函数
     */
    ~SemanticInfo(){};

public:
    /*
     * @brief 存入query
     * @brief 对query切分为单字符并存储到seq_vec
     */
    int set_query(const string& query);

    /*
     * @brief 存入无领域序列标注结果到global_slots(第一层NER结果)
     */
    int set_null_slots(const vector<string>& tags);
    int set_null_slots(const vector<_slot>& slots);

    /*
     * @brief 存入domain信息
     * @brief one-best & n-best
     */
    int set_domain(const vector<pair<string, float> >& domain_vec);
    int set_domain(const vector<clf_info>& domain_vec);

    /*
     * @brief 存入限定领域的intent信息
     * @brief one-best
     */
    int set_intent(const string& domain, 
                   const vector<pair<string, float> >& intent_vec);//< 只使用第一个
    int set_intent(const string& domain, 
                   const clf_info& intent);

    /*
     * @brief 存入限定领域序列标注结果到对应的限定领域
     * @param domain : 限定领域
     * @param tags   : 对应的解析结果
     * @param slots  : 对应的slot结果
     */
    int set_slots(const string& domain, 
                  const vector<string>& tags);
    int set_slots(const string& domain, 
                  const vector<_slot>& slots);

    /*
     * @brief set form
     */
    int set_form(const _form& form);
    int set_forms(vector<_form>& forms);

    /*
     * @brief set match_mode
     */
    int set_match_mode(string match_mode);

    /*
     * @brief 获取domain信息
     */
    vector<string> get_domain();

    /*
     * @brief 获取限定领域的intent信息
     */
    string get_intent(const string& domain);

    /*
     * @brief 获取限定领域的slots信息
     */
    vector<_slot> get_slots(const string& domain = "null");

    /*
     * @brief 获取完整的form信息
     */
    vector<_form> get_forms();

    /*
     * @brief 获取match_mode信息
     */
    string get_match_mode();

    /*
     * @brief 获取query信息
     * @brief query & ner_query
     */
    string get_query();
    string get_ner_query(string domain = "null");

private:
    /*
     * @brief seq_vec + tags到slots的转换
     * @brief 刘 b+artist  
     * @brief 德 i+artist
     * @brief 华 e+artist
     * @brief 的 O
     * @brief 歌 O
     * @brief => artist = 刘德华
     */
    vector<_slot> tags2slots(const vector<string>& tags);

    /*
     * @brief seq_vec + slots到ner_query的转换
     * @brief artist = 刘德华
     * @brief => [刘德华:artist]的歌
     */
    string slots2ner_query(const vector<_slot>& slots);

private:
    //< query信息
    string query;
    //< 单字符切分的结果 方便NER编解码
    vector<string> seq_vec;
    //< 无领域序列标注结果
    vector<_slot> global_slots;
    //< n-best SLU结果
    vector<_form> form_vec;
    //< 匹配模式(regex/model)
    string match_mode;
};

#endif
/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
