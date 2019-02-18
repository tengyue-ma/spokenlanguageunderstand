/*************************************************************************
	> File Name: test.cpp
	> Author: leilei.zhou
	> Mail: leilei.zhou@hobot.cc
	> Created Time: 2018.7.6
 ************************************************************************/

#include <iostream>
#include <dlfcn.h>
#include <unistd.h>
#include <stdlib.h>
#include <fstream>
#include <cstring>
#include <map>
#include <vector>
#include <string>
#include "Feature.h"
#include "FeatExtractor.h"
#include "PreProcessor.h"
#include "Classifier.h"
using namespace std;
using namespace feat_extractor;
using namespace classify;

int print_result(string query, vector<pair<string, float> > class_name){

    cout << query << "\t" << class_name[0].first << "\t" << class_name[0].second << endl;

    return 0;
}

int main(int argc, const char** argv){
    int ret = 0;
    
    cout << ">>>>>> 正在加载配置……" << endl;
    //< 1. 初始化ltp
    PreProcessor* pre = new PreProcessor();
    pre->create_wsg_engine("../../data/model/ltp_model/cws.model");
    pre->create_pos_engine("../../data/model/ltp_model/pos.model");
    pre->create_ner_engine("../../data/model/ltp_model/ner.model");
    pre->create_pas_engine("../../data/model/ltp_model/parser.model");

    //< 2. 指定特征提取库, me的库, 配置, 这个没有写在argv参数里
    string feat_so = "../../feat_extractor/onehot_feat/lib/libonehotfeat.so";
    string me_so = "lib/libnn.so";
    string clf_conf = "nn_model.conf";
    string conf = "onehot_feat.conf";
    string log_conf = "log_path.conf";

    //< 3. 加载特征提取配置文件
    typedef FeatExtractor* (*CREATE_FEAT_EXTR)(const string, PreProcessor*);
    void *handle = dlopen(feat_so.c_str(), RTLD_NOW);
    if (!handle){
        cout << dlerror() << endl;
        exit(-1);
    }
    CREATE_FEAT_EXTR create_feat_extr = reinterpret_cast<CREATE_FEAT_EXTR>(dlsym(handle, "create_feat_extr"));
    FeatExtractor* feat_ext = create_feat_extr(conf, pre);
    if (feat_ext == NULL){
        cout << ">>>>>> ERROR: feat_ext error." << endl;
        exit(-1);
    }

    //< 4. 加载分类模型
    typedef Classifier* (*CREATE_CLASSIFIER)(const string, const string);
    void *handle_clf = dlopen(me_so.c_str(), RTLD_NOW);
    if (!handle_clf){
        cout << dlerror() << endl;
        exit(-1);
    }
    CREATE_CLASSIFIER create_classifier = reinterpret_cast<CREATE_CLASSIFIER>(dlsym(handle_clf, "create_classifier"));
    Classifier* clf = create_classifier(clf_conf, log_conf);
    if (clf == NULL){
        cout << ">>>>>> ERROR: load clf model error." << endl;
        exit(-1);
    }
    cout << ">>>>>> 配置加载完毕" << endl;

    //< 5. 进行分类
    while(1){
        string query;
        cin >> query;
        vector<pair<string, float> > class_name;
        ret = clf->classify(query, class_name, feat_ext);
        if (ret < 0) {
            cout<< ">>>>>> ERROR: clf failed" <<endl;
        }
        print_result(query, class_name);
    }

    //< 6. delete
    typedef FeatExtractor* (*DELETE_FEAT_EXTR)(FeatExtractor *);
    DELETE_FEAT_EXTR delete_feat_extr = reinterpret_cast<DELETE_FEAT_EXTR>(dlsym(handle, "delete_feat_extr"));
    delete_feat_extr(feat_ext);

    typedef Classifier* (*DELETE_CLASSIFIER)(Classifier *);
    DELETE_CLASSIFIER delete_classifier = reinterpret_cast<DELETE_CLASSIFIER>(dlsym(handle_clf, "delete_classifier"));
    delete_classifier(clf);

    dlclose(handle);
    dlclose(handle_clf);

    return 0;
}

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
