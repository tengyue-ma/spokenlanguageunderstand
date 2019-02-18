/*************************************************************************
	> File Name: SlotFilling.cpp
	> Author: TengyueMa
	> Mail: lumujin@icloud.com
	> Created Time: 六  2/ 3 13:57:57 2018
 ************************************************************************/

#include <iostream>
#include "SlotFilling.h"

using namespace std;

namespace slot_filling{

int SlotFilling::parse(const string query, 
                       vector<_slot>& slots, 
                       feat_extractor::FeatExtractor* feat_ext){
    if (feat_ext == NULL){
        return -1;
    }

    return 0;
}

} //< namespace slot_filling

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
