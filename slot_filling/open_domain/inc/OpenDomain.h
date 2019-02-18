/*************************************************************************
	> File Name: OpenDomain.h
	> Author: TengyueMa
	> Mail: lumujin@icloud.com
	> Created Time: Mon 19 Mar 2018 07:13:43 PM CST
 ************************************************************************/

#ifndef _OPENDOMAIN_H
#define _OPENDOMAIN_H

#include "Feature.h"
#include "FeatExtractor.h"
#include "SlotFilling.h"

namespace slot_filling{
/*
 * @brief 对于特定的开放领域query解析
 * @brief 几乎是空调用
 */
class OpenDomain:public SlotFilling{
public:
    /*
     * @brief 构造函数
     */
    OpenDomain(const string log_path);

    /*
     * @brief 析构函数
     */
    ~OpenDomain();

public:
    /*
     * @brief 对外接口
     */
    int parse(const string query, 
              vector<_slot>& tags, 
              feat_extractor::FeatExtractor* feat_ext);
};

} //< namespace slot_filling;

#endif
