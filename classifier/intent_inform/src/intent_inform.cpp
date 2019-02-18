/*************************************************************************
	> File Name: intent_inform.cpp
	> Author: TengyueMa
	> Mail: lumujin@icloud.com
	> Created Time: Mon 02 Apr 2018 04:51:46 PM CST
 ************************************************************************/

#include <iostream>
#include "intent_inform.h"
using namespace std;

namespace classify{
    
extern "C" Classifier* create_classifier(const string model_path, 
                                         const string log_path){
    Intent_inform* intent = new Intent_inform(model_path, log_path);
    return dynamic_cast<Classifier *>(intent);
}

extern "C" void delete_classifier(Classifier* clf){
    Intent_inform* intent = dynamic_cast<Intent_inform *>(clf);
    //delete me_clf;
    intent = NULL;
}

int Intent_inform::classify(const string query, 
                            vector<std::pair<string, float> >& class_name, 
                            feat_extractor::FeatExtractor* feat_ext){
    class_name.push_back(std::pair<string, float>(this->intent, 100));

    return 0;
}
} //< namespace classify
