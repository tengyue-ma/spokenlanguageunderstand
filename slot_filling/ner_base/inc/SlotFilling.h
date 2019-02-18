/*************************************************************************
	> File Name: SlotFilling.h
	> Author: TengyueMa
	> Mail: lumujin@icloud.com
	> Created Time: 一  1/29 15:25:01 2018
 ************************************************************************/

#ifndef _SLOTFILLING_H
#define _SLOTFILLING_H

#include <string>
#include <vector>

#include "defStruct.h"
#include "FeatExtractor.h"

using namespace std;

namespace slot_filling{

class SlotFilling{
public:
    /*
     * @brief 析构
     */
    virtual ~SlotFilling(){};

    /*
     * @brief 获取slot解析结果
     * @param query : 待预测句子
     * #@param tags : 预测结果
     * @param slots : 预测结果slot形式 
     * @param feat_ext : 特征提取方式
     */
    //< 本接口修改于2018.3.21 将返回值tag list改为slot list 用以支持更多的解析方式
    //virtual int parse(const string query, vector<string>& tags, feat_extractor::FeatExtractor* feat_ext) = 0;
    virtual int parse(const string query, 
                      vector<_slot>& slots, 
                      feat_extractor::FeatExtractor* feat_ext) = 0;

public:
    //< 实体识别器类型
    string ner_type;
};

} //< namespace slot_filling;
#endif
/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
