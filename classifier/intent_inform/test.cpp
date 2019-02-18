/*************************************************************************
	> File Name: test.cpp
	> Author: TengyueMa
	> Mail: lumujin@icloud.com
	> Created Time: 五  2/ 2 13:57:37 2018
 ************************************************************************/

#include <iostream>
#include <dlfcn.h>
#include <unistd.h>
#include <stdlib.h>
#include "Feature.h"
#include "FeatExtractor.h"
#include "PreProcessor.h"
#include "Classifier.h"
using namespace std;
using namespace feat_extractor;
using namespace classify;

int main(int argc, char** argv){
    if (argc == 0 && argv[0] == 0){
    }
    string me_so = "lib/libintent.so";
    PreProcessor* pre = new PreProcessor();
    FeatExtractor* feat_ext = NULL;

    typedef Classifier* (*CREATE_CLASSIFIER)(const string, const string);

    void *handle_clf = dlopen(me_so.c_str(), RTLD_NOW);
    if (!handle_clf){
        cout << dlerror() << endl;
        exit(-1);
    }

    //3.加载分类模型
    CREATE_CLASSIFIER create_classifier = reinterpret_cast<CREATE_CLASSIFIER>(dlsym(handle_clf, "create_classifier"));
    Classifier* clf = NULL;
    clf = create_classifier("inform", "null");
    if (clf == NULL){
        cout << "clf error." << endl;
        exit(-1);
    }

    //4. 分类
    vector<pair<string, float> > class_name;
    clf->classify("刘德华的歌曲", class_name, feat_ext);
    cout << class_name[0].first << " : " << class_name[0].second << endl;

    typedef Classifier* (*DELETE_CLASSIFIER)(Classifier *);
    DELETE_CLASSIFIER delete_classifier = reinterpret_cast<DELETE_CLASSIFIER>(dlsym(handle_clf, "delete_classifier"));
    delete_classifier(clf);

    dlclose(handle_clf);

    return 0;
}

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
