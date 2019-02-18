/*************************************************************************
	> File Name: PreProcessor.h
	> Author: TengyueMa
	> Mail: lumujin@icloud.com
	> Created Time: 一  1/22 14:11:53 2018
 ************************************************************************/

#ifndef _PREPROCESSOR_H
#define _PREPROCESSOR_H

#include <string>
#include <vector>

using std::string;
using std::vector;

class PreProcessor{
public:
    /*
     * @brief 构造函数
     */
    PreProcessor();

    /*
     * @brief 析构函数
     */
    ~PreProcessor();

public:
    /*
     * @brief 模型初始化
     */
    //< 分词
    int create_wsg_engine(string wsg_mdl_path);
    int create_wsg_engine(string wsg_mdl_path, 
                          string word_dict_path);
    //< 词性标注
    int create_pos_engine(string pos_mdl_path);
    int create_pos_engine(string pos_mdl_path, 
                          string pos_dict_path);
    //< 命名实体识别
    int create_ner_engine(string ner_mdl_path);
    //< 依存句法分析
    int create_pas_engine(string pas_mdl_path);
    
    /*
     * @brief 模型调用
     * @brief 失败返回-1 成功返回0
     */
    //< 分词
    int wordseg(string query, 
                vector<string>& ws_vec);
    //< 词性标注
    int postag(vector<string>& ws_vec, 
               vector<string>& pos_vec);
    //< 命名实体识别
    int ner(vector<string>& ws_vec, 
            vector<string>& pos_vec, 
            vector<string>& ner_vec);
    //< 依存句法分析
    int parse(vector<string>& ws_vec, 
              vector<string>& pos_vec, 
              vector<int>& heads_vec, 
              vector<string>& pas_vec);

private:
    //< 模型
    void *_wsg_engine;
    void *_pos_engine;
    void *_ner_engine;
    void *_pas_engine;
};

#endif
/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
