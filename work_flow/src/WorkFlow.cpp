/*************************************************************************
	> File Name: WorkFlow.cpp
	> Author: TengyueMa
	> Mail: lumujin@icloud.com
	> Created Time: 一  2/26 11:21:29 2018
 ************************************************************************/

#include <iostream>
#include <vector>
#include <string>
#include "WorkFlow.h"
#include "ul_log.h"
#include "Configure.h"
#include "spreg.h"
#include "SluErrorCode.h"
#include "SluLog.h"

using namespace std;
using namespace hobotlog;

//< log定义
LogModule * loginstance = NULL;

WorkFlow::WorkFlow(){
    //load_conf("NULL");
}

WorkFlow::~WorkFlow(){

}

int WorkFlow::load_conf(const string conf_file, 
                        const string conf_path){


    if (conf_file == ""){
        fprintf(stderr,"workflow conf is null\n");
        return ERR_WORK_FLOW_CONF_NULL;
    }

    int ret = 0;
    comcfg::Configure config;
    ret = config.load(conf_path.c_str(), conf_file.c_str());
    if (ret < 0){
        fprintf(stderr,"failed to load workflow config file\n");
        return ERR_WORK_FLOW_LOAD_CONF;
    }

    //init work flow log
    try {
        string log_file = config["WORK_FLOW_LOG_CONF"].to_cstr();
		if(NULL == loginstance){
			loginstance = new hobotlog::LogModule(log_file);
			loginstance->info("[SLU]: load workflow log conf, and init loginstance success!");
		}
    }catch(...){
        cout << "do not set workflow log file in: " << conf_file << endl;
        return ERR_WORKFLOW_NOT_SET_LOG_FILE;
    }

    //< load
    try {
        int node_size = static_cast<int>(config["NODE"].size());
        for (int i = 0; i < node_size; i++){
            _workstep workstep;
            workstep.func_type = config["NODE"][i]["NODE_FUNC"].to_cstr();
            int param_size = static_cast<int>(config["NODE"][i]["PARAM"].size());
            for (int j = 0; j < param_size; j++){
                workstep.params.push_back(config["NODE"][i]["PARAM"][j].to_cstr());
            }
            workstep.output = config["NODE"][i]["OUTPUT"].to_cstr();

            workflow.push_back(workstep);
        }
    } catch(...){
        loginstance->error("[SLU]: failed to load config file: " + conf_file);
        return ERR_WORK_FLOW_CONF_CONTENT;
    }

    /*
    for (size_t i = 0; i < workflow.size(); i++){
        cout << "NODE:" << endl;
        cout << workflow[i].func_type << endl;
        for (size_t j = 0; j < workflow[i].params.size(); j++){
            cout << workflow[i].params[j] << endl;
        }
        cout << workflow[i].output << endl;
    }
    */

    loginstance->info("[SLU]: load workflow config file success！");
    return 0;
}
/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
