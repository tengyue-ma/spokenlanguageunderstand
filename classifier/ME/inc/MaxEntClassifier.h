/*************************************************************************
	> File Name: MaxEntClassifier.h
	> Author: TengyueMa
	> Mail: lumujin@icloud.com
	> Created Time: 一  1/22 14:34:53 2018
 ************************************************************************/

#ifndef _MAXENTCLASSIFIER_H
#define _MAXENTCLASSIFIER_H

#include <map>
#include <string>
#include <vector>
#include <unordered_map>
#include "Feature.h"
#include "Classifier.h"

using std::string;
using std::vector;
using std::map;
using std::pair;

namespace classify{

//< me feature type
struct _feat{
    string content;
    float weight;
};

class MaxEntClassifier:public Classifier{
public:
    /*
     * @brief 构造函数
     * @param model_path 模型路径
     * @param log_path log路径
     */
    MaxEntClassifier(const string model_path, 
                     const string log_path);

    /*
     * @brief 析构函数
     */
    ~MaxEntClassifier();

public:
    /*
     * @brief 模型读取
     * @param model_path 模型路径
     */
    int load_model(string model_path);

    /*
     * @brief 对外接口
     * @param query 待分类query
     * @param class_name n-best结果 type:vector<pair<string, float> >
     * @param feat_ext 特征提取器
     */
    int classify(const string query, 
                 vector<std::pair<string, float> >& class_name, 
                 feat_extractor::FeatExtractor* feat_ext);

    /*
     * @brief 获取分类
     * @param feat:特征
     * @param class_name:n-best结果
     */
    int classify(vector<_feat> feats, 
                 vector<pair<string, float> >& class_name);

private:
    /*
     * @brief id => class num
     * @brief 根据id找到对应的class name
     */
    string get_id_class(const int& id);

    /*
     * @brief feat => id
     * @brief 根据特征值找到对应的特征id
     */
    int get_feat_id(const string& feat_name);

private:
    //< 类别个数
    int class_num;
    //< 特征个数
    int feat_num;
    //< 默认分类
    string default_clf_name;
    //< 参数矩阵
    vector<vector<float> > weights;
    //< id => class name
    map<int, string> id_class_map;
    //< feat => id
    unordered_map<string, int> feat_id_map;
};

} //< classify
#endif
/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
