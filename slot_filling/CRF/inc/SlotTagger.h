/*************************************************************************
	> File Name: SlotTagger.h
	> Author: TengyueMa
	> Mail: lumujin@icloud.com
	> Created Time: 一  1/29 12:45:44 2018
 ************************************************************************/

#ifndef _SLOT_TAGGER_H
#define _SLOT_TAGGER_H

#include "crfpp.h"

#include "CRFWrapper.h"
#include "Feature.h"
#include "utils.h"
#include "FeatExtractor.h"
#include "SlotFilling.h"

namespace slot_filling{
/*
 * @brief CRF++ tagger封装
 */
class SlotTagger:public SlotFilling{
public:
    /*
     * @brief 构造函数
     */
    SlotTagger(const string model_path, 
               const string log_path);

    /*
     * @brief 析构函数
     */
    ~SlotTagger();

public:
    /*
     * @brief 对外接口
     */
    //< 修改于2018.3.21 返回结果由tag list改为slot list
    /*
     * slot => name  = artist
     *         value = 刘德华
     *         start = 0
     *         end   = 2
     *
     *         name  = title
     *         value = 冰雨
     *         start = 4
     *         end   = 5
     */
    int parse(const std::string query, 
              std::vector<_slot>& slots, 
              feat_extractor::FeatExtractor* feat_ext);
    
    /*
     * @brief load model.
     */
    int load_model(CRFWrapper& crf_wrapper);

    /*
     * @brief slot parse
     * @param sequence:待预测的句子&特征
     * 刘 artist+b  O
     * 德 artist+i  O
     * 华 artist+e  O
     * 的 O         O
     * 冰 O         title+b
     * 雨 O         title+e
     * 放 O         O
     * 一 O         O
     * 下 O         O
     * @param tags:预测结果
     * 刘 artist+b  O       artist+b
     * 德 artist+i  O       artist+i
     * 华 artist+e  O       artist+e
     * 的 O         O       O
     * 冰 O         title+b title+b
     * 雨 O         title+e title+e
     * 放 O         O       O
     * 一 O         O       O
     * 下 O         O       O
     */
    int parse(const std::vector<std::string>& sequence, 
              std::vector<std::string>& tags);

    /*
     * @brief tags to slots
     */
    vector<_slot> tags2slots(const vector<string>& tags, 
                             const string query);

private:
    CRFPP::Tagger* _tagger;
    string ner_type;
    pthread_mutex_t lock;
};

} //< namespace slot_filling

#endif
/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
