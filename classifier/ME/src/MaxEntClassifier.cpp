/*************************************************************************
	> File Name: MaxEntClassifier.cpp
	> Author: TengyueMa
	> Mail: lumujin@icloud.com
	> Created Time: 二  1/23 12:26:48 2018
 ************************************************************************/

#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <cmath>
#include "Feature.h"
#include "MaxEntClassifier.h"
#include "utils.h"
#include "Classifier.h"
#include "MeErrorCode.h"
#include "ul_log.h"
#include "Configure.h"
#include "spreg.h"
#include "SluLog.h"

using namespace std;
using namespace utils;
using namespace hobotlog;

//< log定义
LogModule * loginstance = NULL;

namespace classify{

extern "C" Classifier* create_classifier(const string model_path, 
                                         const string log_path){
    MaxEntClassifier* me_clf = new MaxEntClassifier(model_path, log_path);
    return dynamic_cast<Classifier *>(me_clf);
}

extern "C" void delete_classifier(Classifier* clf){
    MaxEntClassifier* me_clf = dynamic_cast<MaxEntClassifier *>(clf);
    //delete me_clf;
    me_clf = NULL;
}

//< 对外接口
int MaxEntClassifier::classify(const string query, 
                               vector<std::pair<string, float> >& class_name, 
                               feat_extractor::FeatExtractor* feat_ext){
    //< 获取feat
    feature::Feature feat;
    feat_ext->get_feat(feat, query);
   
    //< 解码成ME目标格式
    vector<_feat> feats;
    for (size_t i = 0; i < feat.feats.size(); i++){
        _feat feature;
        feature.content = feat.feats[i];
        //< ME的特征解码与特征提取耦合保持一致性
        feature.weight = 1.0;

        feats.push_back(feature);
    }

    //< 调用分类函数返回结果
    return classify(feats, class_name);
}

//< 构造函数
MaxEntClassifier::MaxEntClassifier(const string model_path, 
                                   const string log_path){
    comcfg::Configure config;
    int ret = config.load("./", log_path.c_str());
    if (ret < 0){
        cout << "failed to load log conf in " << log_path << endl;
        exit(-1);
    }

    //< init log
    try {
        string log_file = config["ME_LOG_CONF"].to_cstr();
        if (loginstance == NULL){
            loginstance = new LogModule(log_file);
        }
    }catch(...){
        cout << "failed to load log config." << endl;
        exit(-1);
    }
    
    this->class_num = 0;
    this->feat_num = 0;
    this->default_clf_name = "";
    
    ret = load_model(model_path);
    if (ret < 0){
        loginstance->error("[SLU]: load me model error in " + model_path);
    }
    loginstance->info("[SLU]: load me model success.");
}

//< 析构函数
MaxEntClassifier::~MaxEntClassifier(){
    //< delete
}

//< 模型读取
//< @param model_path : 模型路径
int MaxEntClassifier::load_model(string model_path){
    ifstream mdl;
    //< 打开文件
    mdl.open(model_path.c_str());

    if (!mdl){
        //log
		loginstance->error("[SLU]: failed to open me model: " + model_path);
        return ERR_ME_OPEN_MAXENT_MODEL;
    }

    //< load class_map
    mdl >> class_num;

    for (int i = 0; i < class_num; i++){
        int num;
        string class_name;
        mdl >> num >> class_name;
        id_class_map[num] = class_name;
    }

    //< load feat_map
    mdl >> feat_num;

    for (int i = 0; i < feat_num; i++){
        int num;
        string feat_name;
        mdl >> num >> feat_name;
        feat_id_map[feat_name] = num;
    }

    //< load weights_map
    string x, y;
    int col, row;
    vector<string> split_strs;
    //< deal
    mdl >> x; mdl >> y;
    strSplitbysep(x, split_strs, ":");
    row = string2int(split_strs[1]);
    strSplitbysep(y, split_strs, ":");
    col = string2int(split_strs[1]);

    //< 读取对应的id
    int* id = new int(col);
    for (int i = 0; i < col; i++){
        mdl >> id[i];
    }

    //< 读取参数矩阵
    string line;
    for (int i = 0; i < row && getline(mdl, line); i++){
        if (line == ""){
            i--;
            continue;
        }
        //< 申请空间
        vector<float> w_list;
        //< 字符串分割
        vector<string> res;
        if (!strSplitbysep(line, res, " ")){
            cout << "error." << endl;
            exit(-1);
        }
        for (size_t j = 1; j < res.size(); j++){
            w_list.push_back(string2float(res[j]));
        }
        weights.push_back(w_list);
    }

    //< default_clf_name
    default_clf_name = id_class_map[0];

    //< 关闭文件
    mdl.close();

    return 0;
}

//< 分类接口
//< @param feat : 特征信息
//< @param class_name : n-best分类结果
int MaxEntClassifier::classify(vector<_feat> feats, 
                               vector<pair<string, float> >& class_name){
    vector<pair<string, float> > class_res;
    
    //< 计算权重
    //< 初始化分数
    pair<float, int> pa(0.0, -1);
    vector<pair<float, int> > class_score(class_num, pa);

    //< 加偏置
    _feat feature;
    feature.content = "DEFAULT_BIAS";
    feature.weight = 1.0;
    feats.push_back(feature);

    //< 计算
    for (size_t i = 0; i < feats.size(); i++){
        _feat feature = feats[i];
        for (int j = 0; j < class_num; j++){
            if (i == 0){
                class_score[j].second = j;
            }
            class_score[j].first += (weights[get_feat_id(feature.content)][j] * feature.weight);
        }
    }
    
    //< 分数归一化 softmax
    //float sum = 0;
    //vector<float> score_sf;
    //for (size_t i = 0; i < class_score.size(); i++){
    //    score_sf.push_back(exp(class_score[i].first));
    //    sum += score_sf[i];
    //}
    //for (size_t i = 0; i < class_score.size(); i++){
    //    class_score[i].first = score_sf[i]/sum;
    //}

    //< 给出n-best结果
    //< 升序
    sort(class_score.begin(), class_score.end());
    //< 找出最大的N个
    //< 如果最大值为0返回默认结果
    if (class_score[class_num-1].first == 0.0){
        class_res.push_back(pair<string, float>(default_clf_name, 0.0));
    }
    else {
        //< push_back 最大值 对应class name
        pair<string, float> pa;
        pa.first = get_id_class(class_score[class_num-1].second);
        pa.second = class_score[class_num-1].first;
        class_res.push_back(pa);
        for (int i = class_score.size()-2; i >= 0; i--){
            if (class_score[i].first == class_score[class_num-1].first){
                class_res.push_back(pair<string, float>(get_id_class(class_score[i].second), class_score[i].first));
            }
            else{
                break;
            }
        }
    }

    class_name = class_res;
	
	loginstance->info("[SLU]: classify success! ");

    return 0;
}

//< id => class name
//< @param id : class id
//< 如果找不到返回空 否则返回对应的class name
string MaxEntClassifier::get_id_class(const int& id){
    if (id_class_map.count(id) == 0){
        return "";
    }
    
    return id_class_map[id];
}

//< feat => id
//< @brief feat : 特征值
//< 如果找不到返回0 否则返回对应的特征序号
int MaxEntClassifier::get_feat_id(const string& feat_name){
    if (feat_id_map.count(feat_name) == 0){
        return 0;
    }

    return feat_id_map[feat_name];
}

} //< classify
/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
