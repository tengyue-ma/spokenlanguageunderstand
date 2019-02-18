/*************************************************************************
	> File Name: NNClassifier.h
	> Author: TengyueMa
	> Mail: lumujin@icloud.com
	> Created Time: Thu 08 Nov 2018 05:52:29 PM CST
 ************************************************************************/

#ifndef _NNCLASSIFIER_H
#define _NNCLASSIFIER_H

#include <tensorflow/cc/ops/io_ops.h>
#include <tensorflow/cc/ops/parsing_ops.h>
#include <tensorflow/cc/ops/array_ops.h>
#include <tensorflow/cc/ops/manip_ops.h>
#include <tensorflow/cc/ops/data_flow_ops.h>

#include <tensorflow/core/public/session.h>
#include <tensorflow/core/protobuf/meta_graph.pb.h>

#include <map>
#include <string>
#include <vector>
#include <fstream>
#include "Feature.h"
#include "Classifier.h"

using std::string;
using std::vector;
using std::map;
using namespace tensorflow;
using namespace tensorflow::ops;

namespace classify{

class NNClassifier:public Classifier{
public:
    /*
     * @brief 构造函数
     * @param model_conf_path 模型路径
     * @param log_path log路径
     */
    NNClassifier(const string model_conf_path, const string log_path);

    /*
     * @brief 析构函数
     */
    ~NNClassifier();

public:
    /*
     * @brief 模型配置读取
     */
    int load_model_conf(string model_conf_path, string log_path);

    /*
     * @brief 对外接口
     * @param query 待分类query
     * @param class_name n-best结果 type:vector<pair<string, float> >
     * @param feat_ext 特征提取器
     *
     * @action1:
     *      利用特征提取器提取特征并解码为模型可读特征
     * @action2:
     *      调用模型返回类别编号
     * @action3:
     *      映射并返回分类结果
     */
    int classify(const string query, vector<std::pair<string, float> >& class_name, feat_extractor::FeatExtractor* feat_ext);

private:
    /*
     * @brief 调用模型返回类别编号
     * @param tensor:输入张量
     * @param class_idx:输出张量
     *              first => 类别编号
     *              second=> 分数
     */
    int classify(vector<vector<float> > tensor, vector<std::pair<int, float> >& class_idx);

    /*
     * @brief 获取类别名称结果
     */
    int get_class_name(int class_idx, string& class_name);

    /*
     * @brief feats解码 tensor
     */
    int decode_feat(feature::Feature feat, vector<vector<float> >& tensor);

private:
    //< 类别
    map<int, string> class_name_map;
    //< 默认分类
    string default_class;
    //< input
    string input_name;
    //< keep_prob
    string keepprob;
    float keep_prob_value;
    //< output
    string output_name;
    //< input type
    string td_type;

    //< session
    tensorflow::Session* session;
};

} //< namespace classify;

#endif
/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
