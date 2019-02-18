/*************************************************************************
	> File Name: SLU.cpp
	> Author: TengyueMa
	> Mail: lumujin@icloud.com
	> Created Time: 四  2/22 14:55:23 2018
 ************************************************************************/

#include <iostream>
#include "SLU.h"
#include "SluLog.h"

using namespace std;

namespace slu{

//< 构造函数
SLU::SLU(){
    pre = NULL;
    parser = NULL;
}

//< 析构函数
SLU::~SLU(){
    delete pre;
    delete parser;
    
    //< 释放所有的特征提取模块
    typedef FeatExtractor* (*DELETE_FEAT_EXTR)(FeatExtractor *);
    for (map<string, FeatExtractor*>::iterator iter = feat_ext_map.begin(); iter != feat_ext_map.end(); iter++){
        DELETE_FEAT_EXTR delete_feat_extr = reinterpret_cast<DELETE_FEAT_EXTR>(dlsym(handle_feat_map[iter->first], "delete_feat_extr"));
        delete_feat_extr(iter->second);
        dlclose(handle_feat_map[iter->first]);
    }

    //< 释放所有的分类模块
    typedef Classifier* (*DELETE_CLASSIFIER)(Classifier *);
    for (map<string, Classifier*>::iterator iter = classifier_map.begin(); iter != classifier_map.end(); iter++){
        DELETE_CLASSIFIER delete_classifier = reinterpret_cast<DELETE_CLASSIFIER>(dlsym(handle_classifier_map[iter->first], "delete_classifier"));
        delete_classifier(iter->second);
        dlclose(handle_classifier_map[iter->first]);
    }
        
    //< 释放所有的NER模块
    typedef SlotFilling* (*DELETE_SLOT_FILLING)(SlotFilling *);
    for (map<string, SlotFilling*>::iterator iter = ner_map.begin(); iter != ner_map.end(); iter++){
        DELETE_SLOT_FILLING delete_slot_filling = reinterpret_cast<DELETE_SLOT_FILLING>(dlsym(handle_ner_map[iter->first], "delete_slot_filling"));
        delete_slot_filling(iter->second);
        dlclose(handle_ner_map[iter->first]);
    }
}

//< 创建各个模块并初始化
int SLU::init_slu(const string conf_file, 
                  const string conf_path){
    comcfg::Configure config;

    int ret = config.load(conf_path.c_str(), conf_file.c_str());
    if (ret < 0){
        loginstance->error("[SLU]: failed to load slu resource config file in " + conf_path +conf_file);
        return ERR_SLU_INIT_LOAD_CONF;
    }

    vector<string> pre_vec;
    vector<vector<string> > feat_vec;//包括crf特征和me特征
    vector<vector<string> > ner_vec;
    vector<vector<string> > classifier_vec;
    //< 规则引擎配置文件地址
    string regex_path;
    //< 领域管理控制配置文件地址
    string domain_manage_path;
    //< log path
    string log_path;

    //< load
    try {
        //< load ltp model path
        pre_vec.push_back(config["LTP_MODEL"][0]["CWS_PATH"].to_cstr());
        pre_vec.push_back(config["LTP_MODEL"][0]["POS_PATH"].to_cstr());
        pre_vec.push_back(config["LTP_MODEL"][0]["NER_PATH"].to_cstr());
        pre_vec.push_back(config["LTP_MODEL"][0]["PARSER_PATH"].to_cstr());

        //< load feat extractor
        int feat_ext_size = static_cast<int>(config["FEAT_EXT"].size());
        for (int i = 0; i < feat_ext_size; i++){
            vector<string> feat;
            feat.push_back(config["FEAT_EXT"][i]["FEAT_NAME"].to_cstr());
            feat.push_back(config["FEAT_EXT"][i]["LIB_SO"].to_cstr());
            feat.push_back(config["FEAT_EXT"][i]["FEAT_CONF"].to_cstr());
            feat_vec.push_back(feat);
        }

        //< load slot filling
        int slot_filling_size = static_cast<int>(config["SLOT_FILLING"].size());
        for (int i = 0; i < slot_filling_size; i++){
            vector<string> ner;
            ner.push_back(config["SLOT_FILLING"][i]["NER_NAME"].to_cstr());
            ner.push_back(config["SLOT_FILLING"][i]["LIB_SO"].to_cstr());
            ner.push_back(config["SLOT_FILLING"][i]["MODEL_PATH"].to_cstr());
            ner_vec.push_back(ner);
        }

        //< load classifier
        int classifier_size = static_cast<int>(config["CLASSIFIER"].size());
        for (int i = 0; i < classifier_size; i++){
            vector<string> clf;
            clf.push_back(config["CLASSIFIER"][i]["CLF_NAME"].to_cstr());
            clf.push_back(config["CLASSIFIER"][i]["LIB_SO"].to_cstr());
            clf.push_back(config["CLASSIFIER"][i]["MODEL_PATH"].to_cstr());
            classifier_vec.push_back(clf);
        }

        //< 规则引擎
        regex_path = config["REGEX_CONF_PATH"].to_cstr();
        //< domain_manage
        domain_manage_path = config["DOMAIN_MANAGE_PATH"].to_cstr();
        //< log
        log_path = config["LOG_PATH"].to_cstr();

    } catch(...){
        loginstance->error("[SLU]: failed to load slu config file: " + conf_file);
        return ERR_SLU_INIT_CONF_CONTENT;
    }
    loginstance->info("[SLU]: load slu config file success!");

    //< 初始化规则引擎
    if (regex_path != "null"){
        parser = new Regex(regex_path.c_str(), loginstance);
        loginstance->info("[SLU]: init regex success!");
    }
    else {
        loginstance->info("[SLU]: jump over init regex!");
    }

    //< 初始化领域管理
    domain_manage = new DomainManage(domain_manage_path.c_str());
    loginstance->info("[SLU]: init domain_manage success!");

    //< 初始化ltp
    //< 
    pre = new PreProcessor();
    if (pre_vec.size() == 4){
        pre->create_wsg_engine(pre_vec[0].c_str());
        pre->create_pos_engine(pre_vec[1].c_str());
        pre->create_ner_engine(pre_vec[2].c_str());
        pre->create_pas_engine(pre_vec[3].c_str());
        loginstance->info("[SLU]: init ltp success!");
    }
    else {
        loginstance->info("[SLU]: init ltp failed!");
    }

    //< 初始化FeatExtractor
    typedef FeatExtractor* (*CREATE_FEAT_EXTR)(const string, PreProcessor*);
     
    for (size_t i = 0; i < feat_vec.size(); i++){
        void *handle = dlopen(feat_vec[i][1].c_str(), RTLD_NOW);
        if (!handle){
            loginstance->error("[SLU]: failed to feat dlopen: " + feat_vec[i][1]);
            return ERR_SLU_INIT_FEAT_DLOPEN;
        }
        CREATE_FEAT_EXTR create_feat_extr = reinterpret_cast<CREATE_FEAT_EXTR>(dlsym(handle, "create_feat_extr"));

        FeatExtractor* feat_ext = create_feat_extr(feat_vec[i][2], pre);
        if (feat_ext == NULL){
            loginstance->error("[SLU]: failed to feat ext: " + feat_vec[i][2]);
            return ERR_SLU_INIT_CREATE_FEAT_EXTR;
        }

        feat_ext_map[feat_vec[i][0]] = feat_ext;
        handle_feat_map[feat_vec[i][0]] = handle;
    }
    loginstance->info("[SLU]: init feat extractor success!");

    //< 初始化分类器
    typedef Classifier* (*CREATE_CLASSIFIER)(const string, const string);

    for (size_t i = 0; i < classifier_vec.size(); i++){
        void *handle = dlopen(classifier_vec[i][1].c_str(), RTLD_NOW);
        if (!handle){
            loginstance->error("[SLU]: failed to classifier dlopen: " + classifier_vec[i][1]);
            return ERR_SLU_INIT_CLF_DLOPEN;
        }
        CREATE_CLASSIFIER create_classifier = reinterpret_cast<CREATE_CLASSIFIER>(dlsym(handle, "create_classifier"));

        Classifier* clf = create_classifier(classifier_vec[i][2], log_path);
        if (clf == NULL){
            loginstance->error("[SLU]: failed to create classifier: " + classifier_vec[i][2]);
            return ERR_SLU_INIT_CREATE_CLF;
        }

        classifier_map[classifier_vec[i][0]] = clf;
        handle_classifier_map[classifier_vec[i][0]] = handle;
    }
    loginstance->info("[SLU]: init classifier success!");

    //< 初始化NER
    typedef SlotFilling* (*CREATE_SLOT_FILLING)(const string, const string);

    for (size_t i = 0; i < ner_vec.size(); i++){
        void *handle = dlopen(ner_vec[i][1].c_str(), RTLD_NOW);
        if (!handle){
            loginstance->error("[SLU]: failed to ner dlopen: " + ner_vec[i][1]);
            return ERR_SLU_INIT_NER_DLOPEN;
        }
        CREATE_SLOT_FILLING create_slot_filling = reinterpret_cast<CREATE_SLOT_FILLING>(dlsym(handle, "create_slot_filling"));

        SlotFilling* ner = create_slot_filling(ner_vec[i][2], log_path);
        if (ner == NULL){
            loginstance->error("[SLU]: failed to create slot filling: " + ner_vec[i][2]);
            return ERR_SLU_INIT_CREATE_SLOT_FILLING;
        }

        ner_map[ner_vec[i][0]] = ner;
        handle_ner_map[ner_vec[i][0]] = handle;
    }
    loginstance->info("[SLU]: init ner success!");

    return 0;
}

int SLU::set_workflow(const WorkFlow& workflow){
    this->workflow = workflow;

    return 0;
}

//< SLU解析接口
int SLU::semantic_interface(string query, 
                            SemanticInfo& sem_info, 
                            string id){
    return semantic_interface(query, sem_info, id, this->workflow);
}
//< SLU解析接口
int SLU::semantic_interface(string query, 
                            SemanticInfo& sem_info,
							string id, WorkFlow workflow){
    int ret = 0;
	vector<string> ids;
	ret =  strSplitbysep(id, ids, "&");
    if(!ret || ids.size()!=3){
		loginstance->error("[SLU]: id format is wrong! right format is: appid&userid&sessionid");
        return ERR_ID_FORMAT;
    }

    string appid = ids[0];
	string userid = ids[1];
	string sessionid = ids[2];
	
	//去标点
	query = rm_punctuation(query);

	//英文小写转大写
	//query = lower2capital(query);
	
    //< 填充query;
    ret = sem_info.set_query(query);
    if(ret < 0){
        loginstance->error("[SLU]: failed to set query: " + query + " appid:"+appid+";userid:"+userid+";sessionid:"+sessionid);
        return ret;
    }

    //< 标志规则是否解析出结果
    bool flag = false;
    //< 节点函数名
    string fun_name = "DOMAIN_MANAGE";
    for (size_t i = 0; i < workflow.workflow.size(); i++){
        //< 如果规则已经解析出结果，忽略以下所有工作，DOMAIN_MANAGE除外
        if (flag){
            if (fun_name != workflow.workflow[i].func_type){
                continue;
            }
        }
        ret = stepfunc(workflow.workflow[i], sem_info, flag, ids);
        if(ret < 0){
            loginstance->error("[SLU]: stepfunc error appid:"+appid+";userid:"+userid+";sessionid:"+sessionid);
            //< return负值 并不代表解析结果一定不对，只能说明在某一步缺少模型或者操作出错
            return ret;
        }
    }
    if(flag){
        sem_info.set_match_mode("regex");
    }
    else{
        sem_info.set_match_mode("model");
    }

    loginstance->info("[SLU]: semantic interface success! appid:"+appid+";userid:"+userid+";sessionid:"+sessionid);
    return 0;
}

//< DOMAIN_MANAGE
int SLU::domain_manager(_workstep& workstep, 
                        SemanticInfo& sem_info, 
                        vector<string> ids){
    string appid = ids[0]; 
    string userid = ids[1]; 
    string sessionid = ids[2];
    
    DomainManage* domain_m = domain_manage;

    if (workstep._void != NULL){
        domain_m = (DomainManage*)workstep._void;
    }
    if (domain_m == NULL){
        return 0;
    }

    //< 获取所有解析结果
    vector<_form> forms = sem_info.get_forms();
    //< 逐一过滤
    string default_domain = domain_m->get_default_domain();
    string default_intent = domain_m->get_default_intent();

    for (size_t i = 0; i < forms.size(); i++){
        if (domain_m->find_domain(forms[i].domain.classes) <= 0){
            loginstance->info("[SLU]: domain: " + forms[i].domain.classes + " => " + default_domain + " &&& intent: " + forms[i].intent.classes + " => " + default_intent + "appid:" + appid + ";userid:" + userid + ";sessionid:" + sessionid);
            forms[i].domain.classes = default_domain;
            forms[i].intent.classes = default_intent;
        }
    }
    sem_info.set_forms(forms);

    return 0;
}

//< REGEX
int SLU::regex_global(_workstep& workstep, 
                 SemanticInfo& sem_info, 
                 bool& flag, 
                 vector<string> ids){
    vector<Semantic *> sems;
    string query = sem_info.get_query();

	//< subnav trick
    string domain = "";
    if (workstep.params.size() == 1){
        domain = workstep.params[0];
    }
	
    //< 如果规则有结果
    if (get_regex(query, sems, ids, domain) >= 0){
        flag = true;
        set_output(sems, sem_info);
    }

    for (size_t i = 0; i < sems.size(); i++){
        delete sems[i];
    }
    vector<Semantic *>().swap(sems);

    return 0;
}

//< NER_FIXED
int SLU::ner_fixed(_workstep& workstep,
              SemanticInfo& sem_info,
              vector<string> ids){
    string appid = ids[0]; 
    string userid = ids[1]; 
    string sessionid = ids[2];
    //< 定义输出
    vector<_slot> output;
    //< NER操作
    int ret = get_ner(ner_map[workstep.params[1]], feat_ext_map[workstep.params[2]], get_input(workstep.params[0], sem_info, "null"), output, ids);
    if(ret < 0){
        loginstance->error("[SLU]: failed to get ner:NER_FIXED");
		loginstance->error("[SLU]: appid:"+appid+";userid:"+userid+";sessionid:"+sessionid);
        return ret;
    }
    //< 返回输出
    ret = set_output_ner(workstep.output, output, sem_info, ids);
    if(ret < 0){
        loginstance->error("[SLU]: failed to set output ner:NER_FIXED");
		loginstance->error("[SLU]: appid:"+appid+";userid:"+userid+";sessionid:"+sessionid);
        return ret;
    }

    loginstance->info("[SLU]: NER_FIXED success!");
	loginstance->info("[SLU]: appid:"+appid+";userid:"+userid+";sessionid:"+sessionid);

    return 0;
}

//< NER_QUERY_DOMAIN
int SLU::ner_every_domain(_workstep& workstep, 
                     SemanticInfo& sem_info, 
                     vector<string> ids){
    string appid = ids[0]; 
    string userid = ids[1]; 
    string sessionid = ids[2];
    //< 获取domain
    vector<string> domain = sem_info.get_domain();
    //< NER操作
    //< 对于每个领域做一遍NER
    for (size_t i = 0; i < domain.size(); i++){
        //< 定义输出
        vector<_slot> output;
        int ret = get_ner(ner_map[domain[i]], feat_ext_map[domain[i]], get_input(workstep.params[0], sem_info, "null"), output, ids);
        if(ret < 0){
            loginstance->error("[SLU]: failed to get ner:NER_QUERY_DOMAIN, domain: " + domain[i]);
			loginstance->error("[SLU]: appid:"+appid+";userid:"+userid+";sessionid:"+sessionid);
            return ret;
        }
        //< 返回对应领域的NER
        ret = set_output_ner(domain[i], output, sem_info, ids);
        if(ret < 0){
            loginstance->error("[SLU]: failed to get ner:NER_QUERY_DOMAIN, domain: " + domain[i]);
			loginstance->error("[SLU]: appid:"+appid+";userid:"+userid+";sessionid:"+sessionid);
            return ret;
        }
    }
    loginstance->info("[SLU]: NER_QUERY_DOMAIN success!");
	loginstance->info("[SLU]: appid:"+appid+";userid:"+userid+";sessionid:"+sessionid);

    return 0;
}

int SLU::clf_fixed(_workstep& workstep,
                   SemanticInfo& sem_info,
                   vector<string> ids){
    string appid = ids[0]; 
    string userid = ids[1]; 
    string sessionid = ids[2];
    //< 定义输出
    vector<pair<string, float> > output;
    //< CLF操作
    int ret = get_classes(classifier_map[workstep.params[1]], feat_ext_map[workstep.params[2]], get_input(workstep.params[0], sem_info, "null"), output, ids);
    if(ret < 0){
        loginstance->error("[SLU]: failed to get classes:CLF_FIXED");
		loginstance->error("[SLU]: appid:"+appid+";userid:"+userid+";sessionid:"+sessionid);
        return ret;
    }
    //< 返回输出
    ret = set_output_clf(workstep.output, output, sem_info, ids);
    if(ret < 0){
        loginstance->error("[SLU]: failed to set output clf:CLF_FIXED");
		loginstance->error("[SLU]: appid:"+appid+";userid:"+userid+";sessionid:"+sessionid);
        return ret;
    }
    loginstance->info("[SLU]: CLF_FIXED success!");
	loginstance->info("[SLU]: appid:"+appid+";userid:"+userid+";sessionid:"+sessionid);

    return 0;
}

int SLU::clf_every_domain(_workstep& workstep,
                          SemanticInfo& sem_info,
                          vector<string> ids){
    string appid = ids[0]; 
    string userid = ids[1]; 
    string sessionid = ids[2];
    //< 获取domain
    vector<string> domain = sem_info.get_domain();
    //< CLF操作
    for (size_t i = 0; i < domain.size(); i++){
        //< 定义输出
        vector<pair<string, float> > output;
        int ret = get_classes(classifier_map[domain[i]], feat_ext_map[workstep.params[2]], get_input(workstep.params[0], sem_info, "null"), output, ids);
        if(ret < 0){
            loginstance->error("[SLU]: failed to get classes:clf_every_domain, domain: " + domain[i]);
			loginstance->error("[SLU]: appid:"+appid+";userid:"+userid+";sessionid:"+sessionid);
            return ret;
        }
        //< 返回输出
        ret = set_output_clf(workstep.output, output, sem_info, ids, domain[i]);
        if(ret < 0){
            loginstance->error("[SLU]: failed to set output clf:clf_every_domain, domain: " + domain[i]);
			loginstance->error("[SLU]: appid:"+appid+";userid:"+userid+";sessionid:"+sessionid);
            return ret;
        }
    }
    loginstance->info("[SLU]: clf_every_domain success!");
	loginstance->info("[SLU]: appid:"+appid+";userid:"+userid+";sessionid:"+sessionid);

    return 0;
}

int SLU::clf_if_domain(_workstep& workstep,
                       SemanticInfo& sem_info,
                       vector<string> ids){
    string appid = ids[0]; 
    string userid = ids[1]; 
    string sessionid = ids[2];
    //< 定义输出
    vector<pair<string, float> > output;
    //< 获取domain
    vector<string> domain = sem_info.get_domain();
    for (size_t i = 0; i < domain.size(); i++){
        if (domain[i] == workstep.params[3]){
            //< CLF操作
            int ret = get_classes(classifier_map[workstep.params[1]], feat_ext_map[workstep.params[2]], get_input(workstep.params[0], sem_info, "null"), output, ids);
            if(ret < 0){
                loginstance->error("[SLU]: failed to get classes:CLF_IF_DOMAIN, domain: " + domain[i]);
				loginstance->error("[SLU]: appid:"+appid+";userid:"+userid+";sessionid:"+sessionid);
                return ret;
            }
            //< 返回输出
            ret = set_output_clf(workstep.output, output, sem_info, ids, domain[i]);
            if(ret < 0){
                loginstance->error("[SLU]: failed to get classes:CLF_IF_DOMAIN, domain: " + domain[i]);
				loginstance->error("[SLU]: appid:"+appid+";userid:"+userid+";sessionid:"+sessionid);
                return ret;
            }
        }
    }
    loginstance->info("[SLU]: CLF_IF_DOMAIN success!");
	loginstance->info("[SLU]: appid:"+appid+";userid:"+userid+";sessionid:"+sessionid);

    return 0;
}

//< 单步操作
int SLU::stepfunc(_workstep& workstep, 
                  SemanticInfo& sem_info, 
                  bool& flag,
				  vector<string> ids){
    if (workstep.func_type == "REGEX_GLOBAL"){
        regex_global(workstep, sem_info, flag, ids);
    }
    else if (workstep.func_type == "NER_FIXED"){
        int ret = ner_fixed(workstep, sem_info, ids);
        if (ret < 0){
            return ret;
        }
    }
    else if (workstep.func_type == "CLF_FIXED"){
        int ret = clf_fixed(workstep, sem_info, ids);
        if (ret < 0){
            return ret;
        }
    }
    else if (workstep.func_type == "CLF_IF_DOMAIN"){
        int ret = clf_if_domain(workstep, sem_info, ids);
        if (ret < 0){
            return ret;
        }
    }
    else if (workstep.func_type == "CLF_EVERY_DOMAIN"){
        int ret = clf_every_domain(workstep, sem_info, ids);
        if (ret < 0){
            return ret;
        }
    }
    else if (workstep.func_type == "NER_EVERY_DOMAIN"){
        int ret = ner_every_domain(workstep, sem_info, ids);
        if (ret < 0){
            return ret;
        }
    }
    else if (workstep.func_type == "DOMAIN_MANAGE"){
        int ret = domain_manager(workstep, sem_info, ids);
        if (ret < 0){
            return ret;
        }
    }

    return 0;
}

//< 获取每层的输入
string SLU::get_input(const string param, 
                 SemanticInfo& sem_info, 
                 string domain){
    string input = "";
    if (param == "query"){
        input = sem_info.get_query();
    }
    else if (param == "ner_query"){
        input = sem_info.get_ner_query(domain);
    }

    return input;
}

//< set output regex
int SLU::set_output(vector<Semantic*>& sems, 
                    SemanticInfo& sem_info){
    for (size_t i = 0; i < sems.size(); i++){
        _form form;
        form.domain.classes = sems[i]->domain;
        form.domain.score = sems[i]->weight;
        form.intent.classes = sems[i]->intent;
        form.intent.score = sems[i]->weight;
        for (size_t j = 0; j < sems[i]->slots.size(); j++){
            _slot slot;
            slot.name = sems[i]->slots[j]->name;
            slot.value = sems[i]->slots[j]->value;
            slot.start = sems[i]->slots[j]->start;
            //< case: 播放刘德华的歌
            //< regex: artist 刘德华 start = 3 end = 6
            //< SLU 中统一改为下标位置
            slot.end = sems[i]->slots[j]->end-1;

            //< push slot
            form.slots.push_back(slot);
        }
        //< set form to sem_info
        sem_info.set_form(form);
    }

    return 0;
}

//< set result
//template <typename T>
//NER res
int SLU::set_output_ner(const string param, 
                        vector<_slot> output, 
                        SemanticInfo& sem_info,
			            vector<string> ids){
    int ret = 0;
    string appid = ids[0];
	string userid = ids[1];
	string sessionid = ids[2];
	
    if (param == "null_slots"){
        ret = sem_info.set_null_slots(output);
        if (ret < 0){
            loginstance->error("[SLU]: set slots error!");
			loginstance->error("[SLU]: appid:"+appid+";userid:"+userid+";sessionid:"+sessionid);
            return ret;
        }    
    }
    //< 对应领域
    else {
        ret = sem_info.set_slots(param, output);
        if (ret < 0){
            loginstance->error("[SLU]: set slots error!");
			loginstance->error("[SLU]: appid:"+appid+";userid:"+userid+";sessionid:"+sessionid);
            return ret;
        }
    }

    return 0;
}

//CLF res
int SLU::set_output_clf(const string param, 
                        vector<pair<string, float> > output, 
                        SemanticInfo& sem_info, 
			            vector<string> ids,
                        const string domain){
    int ret = 0;
    string appid = ids[0];
	string userid = ids[1];
	string sessionid = ids[2];
	
    if (param == "domain"){
        ret = sem_info.set_domain(output);
        if (ret < 0){
            loginstance->error("[SLU]: set domain error!");
			loginstance->error("[SLU]: appid:"+appid+";userid:"+userid+";sessionid:"+sessionid);
            return ret;
        }
    }
    else if (param == "intent"){
        ret = sem_info.set_intent(domain, output);
        if (ret < 0){
            loginstance->error("[SLU]: set intent error!");
			loginstance->error("[SLU]: appid:"+appid+";userid:"+userid+";sessionid:"+sessionid);
            return ret;
        }
    }

    return 0;
}

//< 特征提取
int SLU::get_feat(FeatExtractor* feat_ext, 
                  const string& query, 
                  Feature& feat,
				  vector<string> ids){
    int ret = 0;
    string appid = ids[0];
	string userid = ids[1];
	string sessionid = ids[2];
	
    if (feat_ext == NULL){
        loginstance->error("[SLU]: feat is null!");
		loginstance->error("[SLU]: appid:"+appid+";userid:"+userid+";sessionid:"+sessionid);
        return ERR_SLU_GET_FEAT_EXT_NULL;
    }

    ret = feat_ext->get_feat(feat, query);
    if (ret < 0){
        loginstance->error("[SLU]: fail to get feat!");
		loginstance->error("[SLU]: appid:"+appid+";userid:"+userid+";sessionid:"+sessionid);
        return ret;
    }

    return 0;
}

//< 分类
int SLU::get_classes(Classifier* clf, 
                     FeatExtractor* feat_ext, 
                     const string& query, 
                     vector<pair<string, float> >& class_res,
					 vector<string> ids){
    int ret = 0;
    string appid = ids[0];
	string userid = ids[1];
	string sessionid = ids[2];
	
    if (clf == NULL || feat_ext == NULL){
        loginstance->error("[SLU]: clf feat is null!");
		loginstance->error("[SLU]: appid:"+appid+";userid:"+userid+";sessionid:"+sessionid);
        return ERR_SLU_GET_CLF_FEAT_EXT_NULL;
    }
    
    ret = clf->classify(query, class_res, feat_ext);
    if (ret < 0){
        loginstance->error("[SLU]: fail to classify!");
		loginstance->error("[SLU]: appid:"+appid+";userid:"+userid+";sessionid:"+sessionid);
        return ret;
    }

    return 0;
}

//< NER
int SLU::get_ner(SlotFilling* ner, 
                 FeatExtractor* feat_ext, 
                 const string& query, 
                 vector<_slot>& slots,
				 vector<string> ids){
    int ret = 0;
    string appid = ids[0];
	string userid = ids[1];
	string sessionid = ids[2];
	
    if (ner == NULL || feat_ext == NULL){
        loginstance->error("[SLU]: ner or feat ext is null!");
		loginstance->error("[SLU]: appid:"+appid+";userid:"+userid+";sessionid:"+sessionid);
        return ERR_SLU_GET_NER_FEAT_EXT_NULL;
    }

    ret = ner->parse(query, slots, feat_ext);
    if (ret < 0){
        loginstance->error("[SLU]: fail to ner parse!");
		loginstance->error("[SLU]: appid:"+appid+";userid:"+userid+";sessionid:"+sessionid);
        return ret;
    }

    return 0;
}

//< regex
int SLU::get_regex(const string query, 
				   vector<Semantic*>& sems, 
				   vector<string> ids,
				   const string domain, 
                   const int n_best, 
                   const bool is_gbk){
    string appid = ids[0];
	string userid = ids[1];
	string sessionid = ids[2];
	
    if (this->parser == NULL){
        loginstance->error("[SLU]: regex not init. appid:"+appid+";userid:"+userid+";sessionid:"+sessionid);
        return ERR_PARSER_NULL;
    }

    sems = this->parser->match(query, domain, n_best, is_gbk);

    if (sems.size() <= 0){
        loginstance->error("[SLU]: fail to regex, result is null");
		loginstance->error("[SLU]: appid:"+appid+";userid:"+userid+";sessionid:"+sessionid);
        return ERR_REGEX_FAILED;
    }
        
    loginstance->info("[SLU]: regex match success!");
	loginstance->info("[SLU]: appid:"+appid+";userid:"+userid+";sessionid:"+sessionid);

    return 0;
}

} //< namespace slu;

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
