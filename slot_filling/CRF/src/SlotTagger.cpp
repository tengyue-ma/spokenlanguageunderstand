/*************************************************************************
	> File Name: SlotTagger.cpp
	> Author: TengyueMa
	> Mail: lumujin@icloud.com
	> Created Time: 一  1/29 13:15:45 2018
 ************************************************************************/

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include "SlotTagger.h"
#include "CrfErrorCode.h"
#include "logger.h"
#include "Configure.h"
#include "spreg.h"
#include "SluLog.h"

using namespace std;
using namespace hobotlog;

namespace slot_filling{

/*
 * @param model_path : 模型地址
 * @param log_path   : log地址  
 */
extern "C" SlotFilling* create_slot_filling(const string model_path, 
                                            const string log_path){
    SlotTagger* crf_ner = new SlotTagger(model_path, log_path);
    return dynamic_cast<SlotFilling *>(crf_ner);
}

extern "C" void delete_slot_filling(SlotFilling* ner){
    SlotTagger* crf_ner = dynamic_cast<SlotTagger *>(ner);
    //delete crf_ner;
    crf_ner = NULL;
}

//< 构造函数
SlotTagger::SlotTagger(const string model_path, 
                       const string log_path){
    comcfg::Configure config;
    int ret = config.load("./", log_path.c_str());
    if (ret < 0){
        cout << "failed to load log conf in " << log_path << endl;
        exit(-1);
    }

    //< init log
    try {
        string log_file = config["CRF_LOG_CONF"].to_cstr();
        if (loginstance == NULL){
            loginstance = new LogModule(log_file);
        }
    }catch(...){
        cout << "failed to load log config." << endl;
        exit(-1);
    }

    this->_tagger = NULL;
    //< load model
    CRFWrapper* crf_w = new CRFWrapper();
    crf_w->load_model(model_path);
    load_model(*crf_w);
    pthread_mutex_init(&lock, NULL);
    loginstance->info("[SLU]: load model " + model_path + " success.");
}

//< 对外接口
int SlotTagger::parse(const string query, 
                      vector<_slot>& slots, 
                      feat_extractor::FeatExtractor* feat_ext){
    //< 获取feat
    feature::Feature feat;
    feat_ext->get_feat(feat, query);

    //< 解码成CRF目标格式
    //< nothing;
    //< 调用ner接口返回结果
    pthread_mutex_lock(&lock);
    vector<string> tags;
    int ret = parse(feat.feats, tags);
    slots = tags2slots(tags, query);
    pthread_mutex_unlock(&lock);
    return ret;
}

//< 析构函数
SlotTagger::~SlotTagger(){
    if (!_tagger){
        return ;
    }
    delete _tagger;
    pthread_mutex_destroy(&lock);
}

//< load model.
int SlotTagger::load_model(CRFWrapper& crf_wrapper){
    if (!_tagger){
        delete _tagger;
    }

    if (crf_wrapper.get_crf_model() == NULL){
        _tagger = NULL;
    }

    _tagger = crf_wrapper.get_crf_model()->createTagger();
    if (!_tagger){
		loginstance->error("[SLU]: failed to create crf tagger!");
        return ERR_CRF_CREAT_TAGGER;
    }
	
	loginstance->info("[SLU]: create crf tagger success!");

    return 0;
}

int SlotTagger::parse(const vector<string>& sequence, 
                      vector<string>& tags){
    tags.clear();

    if (!_tagger){
        for (size_t i = 0; i < sequence.size(); i++){
            tags.push_back("O");
        }

        return 0;
    }
    _tagger->clear();

    //< push
    vector<string>::const_iterator iter = sequence.begin();
    for ( ; iter != sequence.end(); iter++){
        _tagger->add((*iter).c_str());
    }

    //< 预测
    if (!_tagger->parse()){
		loginstance->error("[SLU]: failed to crf parse!");
        return ERR_CRF_TAGGER_PARSE;
    }

    //< 获取预测结果
    for (size_t i = 0; i < _tagger->size(); i++){
        tags.push_back(_tagger->y2(i));
    }
	
	loginstance->info("[SLU]: crf parse success!");

    return 0;
}

//< tags to slots
vector<_slot> SlotTagger::tags2slots(const vector<string>& tags, 
                                     const string query){
    vector<_slot> slots;
    
    //< query 单字切分
    vector<string> seq_vec;
    if (!utils::strSplit2simple(query, seq_vec)){
        return slots;
    }

    //< 解码
    vector<pair<string, string> > slot_list;
    vector<pair<int, int> > slot_range;

    if (utils::ner_decode(slot_list, slot_range, tags, seq_vec) == -1){
		loginstance->error("[SLU]: ner decode error!");
    }
    
    if (slot_list.size() != slot_range.size()){
		loginstance->error("[SLU]: slot count error!");
    }

    for (size_t i = 0; i < slot_list.size(); i++){
        _slot slot;
        slot.name  = slot_list[i].first;
        slot.value = slot_list[i].second;
        slot.start = slot_range[i].first;
        slot.end   = slot_range[i].second;

        slots.push_back(slot);
    }
	
	loginstance->info("[SLU]: tags2slots success! query: " + query);

    return slots;
}

} //< namespace slot_filling

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
