/*************************************************************************
	> File Name: SemanticInfo.cpp
	> Author: TengyueMa
	> Mail: lumujin@icloud.com
	> Created Time: Tue 06 Mar 2018 04:50:37 PM CST
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include "utils.h"
#include "SemanticInfo.h"
#include "SluErrorCode.h"
#include "SluLog.h"

using namespace std;

extern
//< 存入query
//< 对query切分为单字符并存储到seq_vec
int SemanticInfo::set_query(const string& query){
    //< set query
    this->query = query;

    //< set seq_vec
    if (!utils::strSplit2simple(query, this->seq_vec)){
        loginstance->error("[SLU]: failed to split query!");
        return ERR_SEMANTIC_INFO_SPLIT_QUERY;
    }

    loginstance->info("[SLU]: split query success!");

    return 0;
}

//< 存入无领域序列标注结果
int SemanticInfo::set_null_slots(const vector<string>& tags){
    this->global_slots = tags2slots(tags);

    return 0;
}

int SemanticInfo::set_null_slots(const vector<_slot>& slots){
    this->global_slots = slots;

    return 0;
}

//< 存入domain信息 one-best & n-best
int SemanticInfo::set_domain(const vector<pair<string, float> >& domain_vec){
    //< 先清空vec
    this->form_vec.clear();
    //< 循环填充
    for (size_t i = 0; i < domain_vec.size(); i++){
        _form form;
        form.domain.classes = domain_vec[i].first;
        form.domain.score = domain_vec[i].second;
        //< push
        this->form_vec.push_back(form);
    }
    
    if (this->form_vec.size() <= 0){
        loginstance->error("[SLU]: failed to set domain!");
        return ERR_SEMANTIC_INFO_SET_DOMAIN;
    }

    loginstance->info("[SLU]: set domain success!");

    return 0;
}

int SemanticInfo::set_domain(const vector<clf_info>& domain_vec){
    //< 清空vec
    this->form_vec.clear();
    //< 循环填充
    for (size_t i = 0; i < domain_vec.size(); i++){
        _form form;
        form.domain = domain_vec[i];

        //< push
        this->form_vec.push_back(form);
    }
    
    if (this->form_vec.size() <= 0){
        loginstance->error("[SLU]: failed to set domain!!");
        return ERR_SEMANTIC_INFO_SET_DOMAIN;
    }

    loginstance->info("[SLU]: set domain success!!");

    return 0;
}

//< 存入限定领域的intent信息 one-best
int SemanticInfo::set_intent(const string& domain, 
                             const vector<pair<string, float> >& intent_vec){
    bool flag = false;

    //< 找到对应的限定领域
    for (size_t i = 0; i < this->form_vec.size(); i++){
        if (domain == this->form_vec[i].domain.classes){
            //< one-best
            for (size_t j = 0; j < 1 && j < intent_vec.size(); j++){
                this->form_vec[i].intent.classes = intent_vec[j].first;
                this->form_vec[i].intent.score = intent_vec[j].second;
                flag = true;
            }
            break;
        }
    }

    if (!flag){
        loginstance->error("[SLU]: failed to set intent!");
        return ERR_SEMANTIC_INFO_SET_INTENT;
    }

    loginstance->info("[SLU]: set intent success!");

    return 0;
}

int SemanticInfo::set_intent(const string& domain, 
                             const clf_info& intent){
    bool flag = false;

    //< 找到对应的限定领域
    for (size_t i = 0; i < this->form_vec.size(); i++){
        if (domain == this->form_vec[i].domain.classes){
            //< one-best
            this->form_vec[i].intent = intent;

            flag = true;
            break;
        }
    }

    if (!flag){
        loginstance->error("[SLU]: failed to set intent!!");
        return ERR_SEMANTIC_INFO_SET_INTENT;
    }

    loginstance->info("[SLU]: set intent success!!");

    return 0;
}

//< 存入限定领域的序列标注结果
int SemanticInfo::set_slots(const string& domain, 
                            const vector<string>& tags){
    bool flag = false;

    //< 找到对应的限定领域
    for (size_t i = 0; i < this->form_vec.size(); i++){
        if (domain == this->form_vec[i].domain.classes){
            this->form_vec[i].slots = tags2slots(tags);

            flag = true;
            break;
        }
    }

    if (!flag){
        loginstance->error("[SLU]: failed to set slots!");
        return ERR_SEMANTIC_INFO_SET_SLOTS;
    }

    loginstance->info("[SLU]: set slots success!");

    return 0;
}

int SemanticInfo::set_slots(const string& domain, 
                            const vector<_slot>& slots){
    bool flag = false;

    //< 找到对应的限定领域
    for (size_t i = 0; i < this->form_vec.size(); i++){
        if(domain == this->form_vec[i].domain.classes){
            this->form_vec[i].slots = slots;

            flag = true;
            break;
        }
    }

    if (!flag){
        loginstance->error("[SLU]: failed to set slots!!");
        return ERR_SEMANTIC_INFO_SET_SLOTS;
    }

    loginstance->info("[SLU]: set slots success!!");

    return 0;
}

int SemanticInfo::set_form(const _form& form){
    this->form_vec.push_back(form);

    return 0;
}

int SemanticInfo::set_forms(vector<_form>& forms){
    this->form_vec = forms;

    return 0;
}

int SemanticInfo::set_match_mode(string match_mode){
    this->match_mode = match_mode;
    return 0;
}

//< 获取domain信息
vector<string> SemanticInfo::get_domain(){
    vector<string> domains;

    for (size_t i = 0; i < this->form_vec.size(); i++){
        domains.push_back(this->form_vec[i].domain.classes);
    }

    return domains;
}

//< 获取限定领域的intent信息
string SemanticInfo::get_intent(const string& domain){
    string intent;

    for (size_t i = 0; i < this->form_vec.size(); i++){
        if (domain == this->form_vec[i].domain.classes){
            intent = this->form_vec[i].intent.classes;
            break;
        }    
    }

    return intent;
}

//< 获取限定领域的slots信息
vector<_slot> SemanticInfo::get_slots(const string& domain){
    vector<_slot> slots;

    if (domain == "null"){
        return this->global_slots;
    }

    for (size_t i = 0; i < this->form_vec.size(); i++){
        if (domain == this->form_vec[i].domain.classes){
            slots = this->form_vec[i].slots;
            break;
        }
    }

    return slots;
}

//< 获取完整的form信息
vector<_form> SemanticInfo::get_forms(){
    return this->form_vec;
}

//< 获取match_mode信息
string SemanticInfo::get_match_mode(){
    return this->match_mode;
}

//< 获取query信息
string SemanticInfo::get_query(){
    return this->query;
}

//< 获取ner_query
string SemanticInfo::get_ner_query(string domain){
    string res;
    if (domain == "null"){
        res = slots2ner_query(this->global_slots);
    }
    else {
        for (size_t i = 0; i < this->form_vec.size(); i++){
            if (domain == this->form_vec[i].domain.classes){
                res = slots2ner_query(this->form_vec[i].slots);
                break;
            }
        }
    }

    return res;
}

//< tags to slots
vector<_slot> SemanticInfo::tags2slots(const vector<string>& tags){
    vector<_slot> slots;
    vector<pair<string, string> > slot_list;
    vector<pair<int, int> > slot_range;

    if (utils::ner_decode(slot_list, slot_range, tags, this->seq_vec) == -1){
        loginstance->error("[SLU]: failed to ner decode!");
    }
    
    if (slot_list.size() != slot_range.size()){
        loginstance->error("[SLU]: lot count error!");
    }

    for (size_t i = 0; i < slot_list.size(); i++){
        _slot slot;
        slot.name  = slot_list[i].first;
        slot.value = slot_list[i].second;
        slot.start = slot_range[i].first;
        slot.end   = slot_range[i].second;

        slots.push_back(slot);
    }

    loginstance->info("[SLU]: tags to slots success!");

    return slots;
}

//< sort slot start
bool compare(const _slot& slot1, const _slot& slot2){
    return slot1.start < slot2.start;
}

//< slots to ner query
string SemanticInfo::slots2ner_query(const vector<_slot>& slots){
    //< 判断value是否存在于原query
    //< 如果不存在直接返回query
    for (size_t i = 0; i < slots.size(); i++){
        string value;
        for (size_t j = slots[i].start; j <= slots[i].end && j < this->seq_vec.size(); j++){
            value += this->seq_vec[j];
        }
        if (value != slots[i].value){
            return this->query;
        }
    }
    //< 如果全部存在;
    string ner_query;
    int idx = 0;
    bool flag = false;

    vector<_slot> slots_c = slots;
    sort(slots_c.begin(), slots_c.end(), compare);

    for (size_t i = 0; i < slots_c.size(); i++){
        flag = true;
        int start = slots_c[i].start;
        int end   = slots_c[i].end;

        for ( ; idx < start && idx < this->seq_vec.size(); idx++){
            ner_query += this->seq_vec[idx];
        }
        ner_query += "[" + slots_c[i].value + ":" + slots_c[i].name + "]";
        idx = end+1;
    } 

    for ( ; idx < this->seq_vec.size(); idx++){
        ner_query += this->seq_vec[idx];
    }

    if (!flag){
        ner_query = this->query;   
    }

    return ner_query;
}
