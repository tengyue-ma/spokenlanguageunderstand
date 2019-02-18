/*************************************************************************
	> File Name: test1.cpp
	> Author: speechteam
	> Mail: speech@hobot.cc
	> Created Time: Mon 09 Apr 2018 06:59:30 PM CST
 ************************************************************************/

#include<iostream>
#include "SLU.h"
#include "WorkFlow.h"
#include "SemanticInfo.h"

using namespace std;
using namespace slu;

int main(int argc, char ** argv){
    WorkFlow workflow;
    workflow.load_conf("../data/conf/workflow.conf");

    SLU* slu = new SLU();
    slu->init_slu("../data/conf/resource.conf");
    //slu->set_workflow(workflow);

    int n = 0;
    while(true){
        SemanticInfo sem_info;
        string query;
        cout << ">>>> Please input query:" << endl;
        cin >> query;
        if (query == ""){
            break;
        }
        slu->semantic_interface(query, sem_info, "aaa&bbb&ccc", workflow);
       
        
        vector<_form> forms = sem_info.get_forms();
        /*
        for (size_t i = 0; i < forms.size(); i++){
            cout << forms[i].domain.classes << " " << forms[i].intent.classes << " ";
            cout << forms[i].domain.score;
            for (size_t j = 0; j < forms[i].slots.size(); j++){
                cout << " " << forms[i].slots[j].name << " " << forms[i].slots[j].value;
            }
            cout << endl;
        }
        */
        cout << sem_info.get_domain()[0] << " " << forms[0].intent.classes << " " << sem_info.get_ner_query(sem_info.get_domain()[0]) << " " << sem_info.get_ner_query("null")<< endl;
        n++;
    }

    return 0;
}
