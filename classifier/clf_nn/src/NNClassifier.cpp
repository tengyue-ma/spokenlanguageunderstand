/*************************************************************************
	> File Name: NNClassifier.cpp
	> Author: TengyueMa
	> Mail: lumujin@icloud.com
	> Created Time: Fri 09 Nov 2018 02:45:12 PM CST
 ************************************************************************/

#include <iostream>
#include <fstream>
#include "NNClassifier.h"
#include "NNErrorCode.h"
#include "utils.h"
#include "Classifier.h"
#include "ul_log.h"
#include "Configure.h"
#include "spreg.h"
#include "SluLog.h"
#include "Feature.h"

using namespace std;
using namespace utils;
using namespace hobotlog;
using namespace tensorflow;
using namespace tensorflow::ops;

//< log定义
LogModule * loginstance = NULL;

namespace classify{
    
extern "C" Classifier* create_classifier(const string model_conf_path, const string log_path){
    NNClassifier* nn_clf = new NNClassifier(model_conf_path, log_path);
    return dynamic_cast<Classifier *>(nn_clf);
}

extern "C" void delete_classifier(Classifier* clf){
    NNClassifier* nn_clf = dynamic_cast<NNClassifier *>(clf);
    //< delete nn_clf;
    nn_clf = NULL;
}

//< 模型配置读取
int NNClassifier::load_model_conf(string model_conf_path, string log_path){
    comcfg::Configure config_log;
    int ret = config_log.load("./", log_path.c_str());
    if (ret < 0){
        cout << "failed to load log conf in " << log_path << endl;
        exit(-1);
    }

    //< init log
    try {
        string log_file = config_log["NN_CLF_LOG_CONF"].to_cstr();
        if (loginstance == NULL){
            loginstance = new LogModule(log_file);
        }
    }catch(...){
        cout << "failed to load log config." << endl;
        exit(-1);
    }

    comcfg::Configure config;
    ret = config.load("./", model_conf_path.c_str());
    if (ret < 0){
        loginstance->error("[SLU]: load nn model conf error.");
        return ERR_NN_MODEL_CONF;
    }
    //< init model configure
    try {
        int init_size = static_cast<int>(config["NN_MODEL_CONF"].size());
        if (init_size != 1){
            loginstance->error("[SLU]: failed to load nn model conf.");
            return ERR_NN_MODEL_CONF; 
        }

        int classes_size;
        try {
            classes_size = static_cast<int>(config["NN_MODEL_CONF"][0]["CLASS_NAME"].size());
        } catch(...){
            classes_size = 0;
        }

        //< 读domain
        for (int i = 0; i < classes_size; i++){
            string name = config["NN_MODEL_CONF"][0]["CLASS_NAME"][i].to_cstr();
            this->class_name_map[i] = name;
        }

        this->default_class = config["NN_MODEL_CONF"][0]["DEFAULT_CLASS"].to_cstr();
        this->input_name = config["NN_MODEL_CONF"][0]["INPUT_NAME"].to_cstr();
        this->keepprob = config["NN_MODEL_CONF"][0]["KEEP_PROB"].to_cstr();
        this->keep_prob_value = string2float(config["NN_MODEL_CONF"][0]["KEEP_PROB_VALUE"].to_cstr());
        this->output_name = config["NN_MODEL_CONF"][0]["OUTPUT_NAME"].to_cstr();
        this->td_type = config["NN_MODEL_CONF"][0]["TD_TYPE"].to_cstr();

        //< 读模型
        string graph_path = config["NN_MODEL_CONF"][0]["GRAPH_PATH"].to_cstr();
        string checkpoint_path = config["NN_MODEL_CONF"][0]["CHECKPOINT_PATH"].to_cstr();
            
        //< create session
        this->session = NewSession(SessionOptions());
        if (this->session == nullptr){
            loginstance->error("[SLU]: could not create TensorFlow session");

            return ERR_NOT_CREATE_SESSION;
        }

        //< load graph
        MetaGraphDef graph_def;
        auto status = ReadBinaryProto(Env::Default(), graph_path, &graph_def);
        if (!status.ok()){
            loginstance->error("[SLU]: error reading graph definition from " + graph_path + " : " + status.ToString());

            return ERR_LOAD_GRAPH;
        }
        
        //< add graph to session;
        status = this->session->Create(graph_def.graph_def());
        if (!status.ok()){
            loginstance->error("[SLU]: error creating graph: " + status.ToString());

            return ERR_GRAPH_CREAT;
        }

        //< load model weights from saved checkpoint
        Tensor checkpoint_path_tensor(DT_STRING, TensorShape());
        checkpoint_path_tensor.scalar<std::string>()() = checkpoint_path;
        status = this->session->Run({
            {graph_def.saver_def().filename_tensor_name(), checkpoint_path_tensor}, }, {}, {graph_def.saver_def().restore_op_name()}, nullptr);
        if (!status.ok()){
            loginstance->error("[SLU]: error loading checkpoint from " + checkpoint_path + " : " + status.ToString());

            return ERR_LOAD_CHECKPOINT;
        }

    } catch(...) {
        loginstance->error("[SLU]: failed to load nn model conf.");
        return ERR_NN_MODEL_CONF; 
    }

    loginstance->info("[SLU]: load model conf success.");

    return 0;
}

//< 构造函数
NNClassifier::NNClassifier(const string model_conf_path, const string log_path){
    int ret = load_model_conf(model_conf_path, log_path);
    if (ret < 0){
        cout << "load_model_conf error." << endl;

        exit(-1);
    }
}

//< 析构函数
NNClassifier::~NNClassifier(){
    //< delete
}

//< 对外接口
int NNClassifier::classify(const string query, vector<std::pair<string, float> >& class_name, feat_extractor::FeatExtractor* feat_ext){
    //< action1
    feature::Feature feat;
    feat_ext->get_feat(feat, query);
    //< 解码
    vector<vector<float> > tensor;
    int ret = decode_feat(feat, tensor);
    if (ret < 0){
        loginstance->error("[SLU]: feature to tensor error.");

        return ERR_NN_DECODE;
    }
   

    //< action2
    vector<std::pair<int, float> > class_idx;
    ret = classify(tensor, class_idx);
    if (ret < 0){
        loginstance->error("[SLU]: NN model classify error.");

        return ERR_NN_CLASSIFY;
    }

    //< action3
    vector<std::pair<string, float> > classify_res;
    for (size_t i = 0; i < class_idx.size(); i++){
        string name;
        get_class_name(class_idx[i].first, name);
        classify_res.push_back(std::pair<string, float>(name, class_idx[i].second));
    }
    
    class_name = classify_res;

    return 0;
}

//< 调用模型返回类别结果
int NNClassifier::classify(vector<vector<float> > tensor, vector<std::pair<int, float> >& class_idx){
    //< 必须在这个位置
    if (tensor.size() <= 0){
        loginstance->error("[SLU]: tensor error. size == 0");

        return ERR_TENSOR;
    }

    //< 构造输入
    vector<std::pair<string, Tensor> > input;
    
    tensorflow::Tensor keep_prob(tensorflow::DT_FLOAT, tensorflow::TensorShape());
    keep_prob.scalar<float>()() = keep_prob_value;
    
    int tensor_x = tensor.size();
    int tensor_y = tensor[0].size();

    //< 转置
    bool flag = true; //< true竖列 false横列
    if (tensor_y == 1){
        tensor_y = tensor_x;
        tensor_x = 1;
        flag = false; 
    }
    //< 类型判断
    if (this->td_type == "INT32"){
        tensorflow::Tensor input_tensor(tensorflow::DT_INT32, tensorflow::TensorShape({tensor_x, tensor_y}));
        auto input_tensor_mapped = input_tensor.tensor<int, 2>();
        for (size_t i = 0; i < tensor_x; i++){
            for (size_t j = 0; j < tensor_y; j++){
                if (flag){
                    input_tensor_mapped(i, j) = int(tensor[i][j]);
                }
                else {
                    input_tensor_mapped(i, j) = int(tensor[j][i]);
                }
            }
        }
            
        input = {
            {this->input_name, input_tensor},
            {this->keepprob, keep_prob},
        };
    }
    else if (this->td_type == "FLOAT"){
        tensorflow::Tensor input_tensor(tensorflow::DT_FLOAT, tensorflow::TensorShape({tensor_x, tensor_y}));
        auto input_tensor_mapped = input_tensor.tensor<float, 2>();
        for (size_t i = 0; i < tensor_x; i++){
            for (size_t j = 0; j < tensor_y; j++){
                if (flag){
                    input_tensor_mapped(i, j) = tensor[i][j];
                }
                else {
                    input_tensor_mapped(i, j) = tensor[j][i];
                }
            }
        }
            
        input = {
            {this->input_name, input_tensor},
            {this->keepprob, keep_prob},
        };
    }
    
    //< 构造输出
    vector<tensorflow::Tensor> output;

    //< Run
    auto status = this->session->Run(input, {output_name}, {}, &output);
    if (!status.ok()){
        loginstance->error("[SLU]: session run error.");

        return ERR_SESSION_RUN;
    }

    int best_res;
    if (output.size() > 0){
        auto output_y = output[0].flat<int64>();
        best_res = static_cast<int>(output_y(0));
    }
    else {
        loginstance->error("[SLU]: session run result error.");
        return ERR_SESSION_RES;
    }

    vector<std::pair<int, float> > res_vec;
    res_vec.push_back(std::pair<int, float>(best_res, 1.0));

    class_idx = res_vec;

    return 0;
}

//< 获取类别名称结果
int NNClassifier::get_class_name(int class_idx, string& class_name){
    class_name = this->class_name_map[class_idx];
    if (class_name == ""){
        class_name = default_class;
    }

    return 0;
}

//< feats解码 tensor
int NNClassifier::decode_feat(feature::Feature feat, vector<vector<float> >& tensor){
    vector<vector<float> > tensor_vec;
    for (size_t i = 0; i < feat.feats.size(); i++){
        vector<float> feat_vec;
        vector<string> feat_str_vec;
        strSplitbysep(feat.feats[i], feat_str_vec, " ");
        for (int j = 0; j < feat_str_vec.size(); j++){
            float num = string2float(feat_str_vec[j]);
            feat_vec.push_back(num);
        }
        tensor_vec.push_back(feat_vec);
    }
    tensor = tensor_vec;

    return 0;
}

} //< namespace classify

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
