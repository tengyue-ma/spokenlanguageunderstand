/*************************************************************************
	> File Name: test.cpp
	> Author: TengyueMa
	> Mail: lumujin@icloud.com
	> Created Time: 一  1/22 21:02:38 2018
 ************************************************************************/

#include <iostream>
#include <string>
#include <vector>
#include "PreProcessor.h"
using namespace std;

void printInt(string name, vector<int> vec){
    cout << name << ":" << endl;

    for (size_t i = 0; i < vec.size(); i++){
        cout << vec[i] << "\t";
    }
}

void print(string name, vector<string> vec){
    cout << name << ":" << endl;

    for (size_t i = 0; i < vec.size(); i++){
        cout << vec[i] << "\t";
    }

    cout << endl;
}

int main(int argc, char ** argv){
    PreProcessor* pre = new PreProcessor();

    //< 初始化
    pre->create_wsg_engine("../../data/model/ltp_model/cws.model");
    pre->create_pos_engine("../../data/model/ltp_model/pos.model");
    pre->create_ner_engine("../../data/model/ltp_model/ner.model");
    pre->create_pas_engine("../../data/model/ltp_model/parser.model");
    
    //< input
    string query;
    if (argc >= 2){
        query = string(argv[1]);
    }
    else {
        return -1;
    }

    //< output
    vector<string> ws_vec;
    vector<string> pos_vec;
    vector<string> ner_vec;
    vector<int> heads_vec;
    vector<string> pas_vec;

    //< workflow
    pre->wordseg(query, ws_vec);
    pre->postag(ws_vec, pos_vec);
    pre->ner(ws_vec, pos_vec, ner_vec);
    pre->parse(ws_vec, pos_vec, heads_vec, pas_vec);

    //< print
    print("分词结果", ws_vec);
    print("词性标注结果", pos_vec);
    print("命名实体识别结果", ner_vec);
    printInt("句法分析结果", heads_vec);
    print("", pas_vec);

    return 0;
}

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
