/*************************************************************************
	> File Name: test.cpp
	> Author: TengyueMa
	> Mail: lumujin@icloud.com
	> Created Time: 五  2/ 2 13:57:37 2018
 ************************************************************************/

#include <iostream>
#include <stdio.h>
#include <dlfcn.h>
#include <unistd.h>
#include "Feature.h"
#include "FeatExtractor.h"
#include "PreProcessor.h"
using namespace std;
using namespace feat_extractor;

int main(int argc, char** argv){
    int ret = 0;
    string feat_so = "lib/libonehotfeat.so";
    string conf = "feat.conf";
    PreProcessor* pre = new PreProcessor();

    cout << ">>>>>> 正在加载配置……" << endl;
    //< 初始化ltp
    pre->create_wsg_engine("../../data/model/ltp_model/cws.model");
    pre->create_pos_engine("../../data/model/ltp_model/pos.model");
    pre->create_ner_engine("../../data/model/ltp_model/ner.model");
    pre->create_pas_engine("../../data/model/ltp_model/parser.model");

    typedef FeatExtractor* (*CREATE_FEAT_EXTR)(const string, PreProcessor*);

    void *handle = dlopen(feat_so.c_str(), RTLD_NOW);
    CREATE_FEAT_EXTR create_feat_extr = reinterpret_cast<CREATE_FEAT_EXTR>(dlsym(handle, "create_feat_extr"));

    FeatExtractor* feat_ext = create_feat_extr(conf, pre);

    cout << ">>>>>> 加载完毕" << endl;
    while(true) {
        string query;
        cout << ">>>>>> Please input query:";
        cin >> query;
        if (query == "") {
            break;
        }
        feature::Feature feat;
        ret = feat_ext->get_feat(feat, query);
        if (ret < 0) {
            fprintf(stderr, "get me feature failed\n");
            fprintf(stderr, "error code:%d\n", ret);
        }
        cout << ">>>>>> me 特征如下:" << endl;
        cout << feat << endl;
    }
    
    typedef FeatExtractor* (*DELETE_FEAT_EXTR)(FeatExtractor *);
    DELETE_FEAT_EXTR delete_feat_extr = reinterpret_cast<DELETE_FEAT_EXTR>(dlsym(handle, "delete_feat_extr"));
    delete_feat_extr(feat_ext);

    dlclose(handle);
}

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
