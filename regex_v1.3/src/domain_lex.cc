#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <stdlib.h>
#include <string>

#include <ctime>
#include <iostream>
#include <fstream>
#include <cstring>
#include <algorithm>
#include <set>
#include <queue>
#include <vector>

#include "utils.h"
#include "nfa.h"
#include "group.h"
#include "domain_lex.h"
#include "expression.h"
#include "variable.h"
#include "thread_pool.h"

using std::queue;
using std::set;
using std::ifstream;
using std::cout;
using std::endl;
using std::ostream;
using std::vector;
using namespace progschj;

namespace SemanticRegex{

extern NFA* build_NFA(const char*, map<string, Variable*>&, bool expression);

DomainLex::DomainLex(const string& name, const string& lex_dir){
	wrd_max_len = 0;
	lex_dir_ = lex_dir;
    field_name_ = name;
    list_file(lex_dir);
    for(size_t i=0; i != files_.size(); ++i){
        process_file(files_[i]);
    }
    sort_expr();
}

DomainLex::~DomainLex(){
    map<string, Variable*>::iterator iter;
    for(iter=map_var_.begin(); iter!=map_var_.end(); ++iter){
        delete iter->second;
    }
    
    for(size_t i=0; i!=vec_expr_.size(); ++i){
        delete vec_expr_[i];
    }
}

string get_slot_index(const string& name){
	string idx = "";
	for(size_t i=name.size()-1; i>0; i--){
		if(name[i]>=48 && name[i]<=57)
			idx += name[i];
		else if(name[i] == '_')
			break;
		else
			return "";
	}
	return idx;
}

string modify_name(string name, string idx){
	string old_idx = get_slot_index(name);
	if(old_idx != ""){
		name = name.substr(0, name.size()-old_idx.size());
	}
	if(name.back() == '_')
		name = name.substr(0, name.size()-1);
	return name + "_" + idx;
}

bool str_gt_str(const string& str1, const string& str2){
	if(str1.size() > str2.size())
		return true;
	else if(str1.size() < str2.size())
		return false;
	return str1>str2;
}

void merge_semantics(Semantic* sem1, const Semantic* sem2){
	//< 遍历sem1中已经有的slot的最大下标
	string max_idx = "0";
	for(size_t i=0; i<sem1->slots.size(); i++){
		string name = sem1->slots[i]->name;
		string idx = get_slot_index(name);
		if(str_gt_str(idx, max_idx))
			max_idx = idx;
	}
	max_idx = Utils::uint2str(Utils::str2uint(max_idx)+1);
	
	//< 去重判断
	map<string, vector<string> > idx_slots_map;
	for(size_t i=0; i<sem1->slots.size(); i++){
		string name = sem1->slots[i]->name;
		string idx = get_slot_index(name);
		string slotname = name.substr(0, name.size()-idx.size()-1);
		string slotvalue = sem1->slots[i]->value;
		string slot = slotname + " = " + slotvalue;
		if(idx_slots_map.find(idx) == idx_slots_map.end()){
			idx_slots_map[idx] = vector<string>();
		}
		idx_slots_map[idx].push_back(slot);
	}

	vector<string> sem2_slots;
	for(size_t i=0; i<sem2->slots.size(); i++){
		string name = sem2->slots[i]->name;
		string idx = get_slot_index(name);
		string slotname = name.substr(0, name.size()-idx.size()-1);
		string slotvalue = sem2->slots[i]->value;
		string slot = slotname + " = " + slotvalue;
		sem2_slots.push_back(slot);
	}
	std::sort(sem2_slots.begin(), sem2_slots.end());

	map<string, vector<string> >::iterator iter;
	for(iter=idx_slots_map.begin(); iter!=idx_slots_map.end(); iter++) {
		std::sort(iter->second.begin(), iter->second.end());
		if(sem2_slots == iter->second)
			return;
	}

	//< 将sem2中的所有slot拼接到sem1中
	for(size_t i=0; i<sem2->slots.size(); i++){
		string name  = sem2->slots[i]->name;
		string value = sem2->slots[i]->value;
		int start = sem2->slots[i]->start;
		int end   = sem2->slots[i]->end;
		string idx = get_slot_index(name);
		if(str_gt_str(idx, max_idx))
			sem1->add_slot(name, value, start, end);
		else
			sem1->add_slot(modify_name(name, max_idx), value, start, end);
	}
	
}

/*
ostream& operator<<(ostream& out, const Semantic& sem){
	out << "  domain: " << sem.domain << endl;
	out << "  intent: " << sem.intent << endl;
	out << "  slots:" << endl;
	for(size_t i=0; i<sem.slots.size(); i++){
		out << "    " << sem.slots[i]->name << " : " << sem.slots[i]->value << endl;
	}
	return out;
}
*/

vector<Semantic*> DomainLex::split_match(const string& raw_sentence,
										 const vector<string>& sentence,
										 map<string, int>* log,
										 string& matched_string,
										 RedisWrapper *redis_wrapper,
										 unordered_map< string, vector<size_t> >& substr_idx_map,
										 hobotlog::LogModule *loginstance){
	float char_weight = -1.0f;
	//< 最终的语义结果
	vector<Semantic*> sems;
	vector<float> max_weight(sentence.size(), -1.0f);
	vector<int> pre_chars(sentence.size(), -1);
	vector<Semantic*> max_sem(sentence.size(), NULL);
	size_t len = 0;
	for(size_t i=0; i<sentence.size()-1;){
		bool matched = false;
		string part_mc_str = "";
		for(size_t e=sentence.size(); e>i+1; e--){
			vector<string> part_sent(sentence.begin()+i, sentence.begin()+e);
			//< 记得释放part_sems里的Semantic对象
			vector<Semantic*> part_sems = match(Utils::join(part_sent), part_sent,
											log, part_mc_str, redis_wrapper,
											substr_idx_map, loginstance);
			if(part_sems.size()<=0)
				continue;
			matched = true;
			len = part_sent.size();
			// cout << "matched length: " <<  len << endl;
			float pre_max_weight = 0.0f;
			if(i>0)
				pre_max_weight = max_weight[i-1];
			size_t idx = i + part_sent.size() - 1;
			if(max_weight[idx]<pre_max_weight+part_sems[0]->weight){
				max_weight[idx] = pre_max_weight + part_sems[0]->weight;
				pre_chars[idx] = i;
				//< 先删除原结果,才能把新结果赋给该位置
				if(max_sem[idx] != NULL)
					delete max_sem[idx];
				max_sem[idx] = part_sems[0];
				for(size_t j=1; j<part_sems.size(); j++)
					delete part_sems[j];
				//< 修改slot的[start,end]为全局index
				for(size_t j=0; j<max_sem[idx]->slots.size(); j++){
					if(max_sem[idx]->slots[j]->start != -1)
						max_sem[idx]->slots[j]->start += i;
					if(max_sem[idx]->slots[j]->end != -1)
						max_sem[idx]->slots[j]->end += i;
				}
			}
			i += part_sent.size();
			// cout << "matched string: " << Utils::join(part_sent) << endl;
			break;
		}
		if(!matched && i>0){
			size_t t = i-1;
			i -= len;
			// cout << "t=" << t << "; i=" << i << endl;
			while(t>i+1){
				vector<string> part_sent(sentence.begin()+i, sentence.begin()+t);
				vector<Semantic*> part_sems = match(Utils::join(part_sent), part_sent,
													log, part_mc_str, redis_wrapper,
													substr_idx_map, loginstance);
				if(part_sems.size()<=0){
					t--;
					if(t==i+1)
						i += len;
					continue;
				}
				matched = true;
				len = part_sent.size();
				// cout << "submatched string: " << Utils::join(part_sent) << endl;
				float pre_max_weight = 0.0f;
				if(i>0)
					pre_max_weight = max_weight[i-1];
				size_t idx = i + part_sent.size() - 1;
				if(max_weight[idx]<pre_max_weight+part_sems[0]->weight){
					max_weight[idx] = pre_max_weight + part_sems[0]->weight;
					pre_chars[idx] = i;
					if(max_sem[idx] != NULL)
						delete max_sem[idx];
					max_sem[idx] = part_sems[0];
					for(size_t j=1; j<part_sems.size(); j++)
						delete part_sems[j];
					for(size_t j=0; j<max_sem[idx]->slots.size(); j++){
						if(max_sem[idx]->slots[j]->start != -1)
							max_sem[idx]->slots[j]->start += i;
						if(max_sem[idx]->slots[j]->end != -1)
							max_sem[idx]->slots[j]->end += i;
					}
				}
			i += part_sent.size();
			break;
			}
		}
		//< 一个都没匹配上,直接返回
		if(!matched){
			for(size_t i=0; i<max_sem.size(); i++){
				if(max_sem[i] != NULL){
					delete max_sem[i];
					max_sem[i] = NULL;
				}
			}
			return sems;
		}
	}

	//< 回溯
	vector<Semantic*> final_max_sem;
	int i = sentence.size()-1;
	while(i > -1){
		if(max_sem[i] == NULL)
			return sems;
		final_max_sem.push_back(max_sem[i]);
		max_sem[i] = NULL;
		i = pre_chars[i]-1;
	}
	std::reverse(final_max_sem.begin(), final_max_sem.end());
	
	//< 释放max_sem的空间
	for(size_t i=0; i<max_sem.size(); i++){
		if(max_sem[i] != NULL){
			delete max_sem[i];
			max_sem[i] = NULL;
		}
	}

	//< 多个语义结果合并
	Semantic *final_sem = new Semantic();
	final_sem->query = raw_sentence;
	final_sem->match_len = sentence.size();
	final_sem->weight = max_weight.back();
	final_sem->domain = this->field_name_;
	final_sem->intent = "inform";
	for(size_t i=0; i<final_max_sem.size(); i++){
		merge_semantics(final_sem, final_max_sem[i]);
		delete final_max_sem[i];
	}
	sems.push_back(final_sem);
	matched_string = raw_sentence;

	return sems;
}

/*
 * 在当前领域下匹配用户的输入句子
 * 遍历当前领域的所有正则表达式,每个表达式的NFA都匹配一遍
 * 最后收集各个NFA的结果,写到一个vector中返回
 */
vector<Semantic*> DomainLex::match(const string& raw_sentence, 
                                   const vector<string>& sentence, 
                                   map<string, int>* log, 
                                   string& matched_string, 
								   RedisWrapper *redis_wrapper,
								   unordered_map< string, vector<size_t> >& substr_idx_map,
								   hobotlog::LogModule *loginstance){
	vector<Semantic*> sems;

	//< vec_idx表示一个数组,
	//< 每个位置表示此表中相应词在sentence中出现的结尾位置
	vector< vector<size_t> > vec_idx(wrd_idx_map.size()+1, vector<size_t>());
	for(unordered_map< string, vector<size_t> >::iterator iter=substr_idx_map.begin();
		iter!=substr_idx_map.end(); iter++){
		if(wrd_idx_map.find(iter->first) != wrd_idx_map.end()){
			size_t index = wrd_idx_map[iter->first];
			vec_idx[index] = iter->second;
			// loginstance->debug("vec_idx[" + std::to_string(index) + "] = " + std::to_string(iter->second));
		}
	}

	//< 遍历所有的正则表达式,每个都匹配一遍
	for(size_t i=0; i<vec_expr_.size(); i++){
		Expression *expr = vec_expr_[i];
		// loginstance->debug("matching expression " + expr->expr_value_ + ".");
        vector<Group> match_group = expr->match(raw_sentence, sentence, log, matched_string, redis_wrapper, vec_idx);
		if(match_group.size() <= 0){
			continue;
        // }else{
		// 	cout << "matched regex: " << expr->expr_value_ << endl;
		}
		Semantic* sem = new Semantic();
        sem->match_len = matched_string.size();
        sem->query = raw_sentence;
		sem->weight = expr->weight_;
		for(size_t j=0; j<expr->requests_.size(); j++){
			size_t index = expr->requests_[j].second;
			string slot_name = expr->requests_[j].first;
			string slot_valu = match_group[index].str;
			if(slot_name == "domain")
				sem->domain = slot_valu;
			else if(slot_name == "intent")
				sem->intent = slot_valu;
			else if(slot_valu != "")
				sem->add_slot(slot_name, slot_valu,
							  match_group[index].start,
							  match_group[index].end);
		}
		for(size_t j=0; j<match_group.size(); j++){
			if(match_group[j].name != "" && match_group[j].str != ""){
				int idx = sem->has_slot(match_group[j].name);
				if(idx == -1 || sem->slots[idx]->value!=match_group[j].str){
					sem->add_slot(match_group[j].name,
								  match_group[j].str,
								  match_group[j].start,
								  match_group[j].end);
				}
			}
		}

		for(map<string, string>::iterator iter=expr->requests1_.begin();
			iter!=expr->requests1_.end(); iter++){
			if(iter->first == "domain")
				sem->domain = iter->second;
			else if(iter->first == "intent")
				sem->intent = iter->second;
			else
				sem->add_slot(iter->first, iter->second, -1, -1);
		}
		if(sem->domain == "")
			sem->domain = this->field_name_;
		if(sem->intent == "")
			sem->intent = "inform";

        //< 看该条规则的匹配权重是否跟之前的某规则权重一样大
        //< 若是,则保留匹配长度较大者
        bool diff_weight_flag = true;
        for(size_t i=0; i<sems.size(); i++){
            if(sems[i]->weight == sem->weight){
                diff_weight_flag = false;
                if(sem->match_len > sems[i]->match_len){
                    delete sems[i];
                    sems[i] = sem;
                }else if(*sem == *sems[i]){
                    delete sem;
                }else if(sem->match_len == sems[i]->match_len){
                    diff_weight_flag = true;
                }else{
                    delete sem;
                }
                break;
            }
        }
        if(diff_weight_flag){
            sems.push_back(sem);
        }
	}

	return sems;
}

bool DomainLex::compare_weight(const Expression* e1, const Expression* e2){
    return e1->weight_ > e2->weight_;
}

void DomainLex::sort_expr(){
    sort(vec_expr_.begin(), vec_expr_.end(), DomainLex::compare_weight);
}

void DomainLex::list_file(const string& field){
    const char* dir_name = field.c_str();
    if(NULL == dir_name){
        throw std::runtime_error("Invalid folder name!");
    }
    struct dirent* read_dir;
    DIR* open_dir;
    open_dir = opendir(dir_name);

    if(NULL == open_dir){
		throw std::runtime_error("Cannot open directory!");
    }
    
    while((read_dir = readdir(open_dir)) != NULL){
        //< 忽略表示当前目录的"."以及上级目录的".."
		if(read_dir->d_name[0] == '.')
            continue;
        string file_name = dir_name;
        file_name.append("/");
        file_name.append(read_dir->d_name);
        this->files_.push_back(file_name);
    }
}

/*
 * 处理单个文件
 */
void DomainLex::process_file(const string& file_name, bool root){
    ifstream in(file_name.c_str());

	//< 暂存expr line，最后处理
    vector<string> expr_lines;
	//< 暂存var line，最后处理
    vector<string> var_lines;
    if(root)
        map_var_.clear();
    
    if(in){
        string line;
        size_t line_num = 1;
        while(getline(in, line)){
			Utils::replace(line, '\t', ' ');
			//< 检查该行的类型
            LineType line_type = check_line(file_name, line, line_num);
			//< 文件包含
            if(line_type == INCLUDE){
                hash_line(line);
			//< 注释行
            }else if(line_type == ANNOTATION){
                anno_line(line);
			//< 正则表达式行
            }else if(line_type == EXPRESSION){
                expr_lines.push_back(line);
			//< 空行
            }else if(line_type == VOID){
                void_line(line);
			//< 变量定义行
            }else{
                var_lines.push_back(line);
            }
            ++line_num;
        }
        
        //< 处理变量定义
        for(size_t i=0; i<var_lines.size(); ++i)
            var_line(var_lines[i]);




        //< 为所有变量建立nfa
        map<string, Variable*>::iterator iter;
        for(iter=map_var_.begin(); iter!=map_var_.end(); ++iter){
            iter->second->pnfa_ = build_NFA(iter->second->var_value_.c_str(), map_var_, false);
		}
       

        // 线程池
        ThreadPool pool(8);
        // domain内表达式构造nfa过程-多线程
        vector<std::future<Expression*>> expr_results;

        // 把处理每个expression行当作任务分发到线程池
        for(size_t i=0; i<expr_lines.size(); ++i){
            string exp = expr_lines[i];
            expr_results.emplace_back(
                pool.enqueue([exp, this]{
                    Expression* pexp = this->expr_line(exp);
                    return pexp;
                })
            );
		}
        // 等待所有任务执行完毕
        pool.wait_until_empty();
        // 对每条Expression对应的nfa处理，扩展词表、更新最大词长度、str_idx，并对Expression去重
        for(size_t i = 0; i < expr_results.size(); i++){
            Expression* pexp = expr_results[i].get();
            extend_vocab(pexp->pnfa_);
            int flag = 0;
            for(size_t j = 0; j < vec_expr_.size(); j++){
                if(*pexp == *(vec_expr_[j])){
                    flag = 1;
                    break;
                }
            }
            if(flag == 0)
                this->vec_expr_.push_back(pexp);
        }

		in.close();
    }else{
        std::cerr << "Failed to open " << file_name << "!" << endl;
    }
    return;
}

/*
 * 检查读取的一行属于何种类型
 */
LineType DomainLex::check_line(const string& file_name,
							   const string& line,
							   const int& line_num){
	string line1 = Utils::strip(line);
	if(line1.size() == 0){
		return VOID;
	}else if(line1.substr(0, 8) == "#include"){
		return INCLUDE;
	}else if(line1[0] == '#'){
		return ANNOTATION;
	}else if(line1.substr(0, 6) == "export" && line1.find("request") != string::npos){
		return EXPRESSION;
	}else if(line1.find("=") != 0 && line1.find("=") != string::npos){
		return VARIABLE;
	}else{
        string err_msg = file_name;
        err_msg.append(": ");
        err_msg.append(std::to_string(line_num));
        err_msg.append(": Invalid line!");
        throw std::runtime_error(err_msg);
	}
}

/*
 * 处理文件包含,采用递归的形式,若包含了某文件,
 * 则读取该文件的内容,一行一行处理
 */
void DomainLex::hash_line(const string& line){
	string file_name = this->lex_dir_;
	if(file_name.back() != '/')
		file_name.append("/");
	int count = 0;
	for(size_t i=0; i<line.size(); i++){
		if(line[i] == '"')
			count++;
		else if(count>0)
			file_name += line[i];
	}
	if(count!=2 || line.back()!='"')
        throw std::runtime_error("Invalid include!");
    process_file(file_name, 0);
    return;
}

/*
 * 处理表达式中的request返回值
 */
PassRequests DomainLex::get_requests(const string& str_requests){
    PassRequests pass_requests;
    size_t i=0;
    vector<string> split_req;
    Utils::utf8strs(str_requests.c_str(), split_req);
    
    while(split_req[i] != "(") ++i;
    ++i;
    while(i < split_req.size()-1 && split_req[i] != ")"){
        string attr;
        while(split_req[i] != "="){
            if(split_req[i] == " "){
                ++i;
                continue;
            }else{
                attr += split_req[i];
                ++i;
            }
        }++i;
        while(split_req[i] != "\"") ++i;
        ++i;
        while(split_req[i] == " ") ++i;
        if(split_req[i] == "$"){
			++i;
            string str_index;
            while(split_req[i] != "\""){
                if(split_req[i] == " "){
                    ++i;
                    continue;
                }else{
                    str_index += split_req[i];
                    ++i;
                }
            }++i;
            while(split_req[i]!="," && split_req[i]!=")") ++i;
            ++i;
            pass_requests.requests1.push_back(make_pair(attr, Utils::str2uint(str_index)));
        }else{
            string str_index;
            while(split_req[i] != "\""){
                if(split_req[i] == " "){
                    ++i;
                    continue;
                }else{
                    str_index += split_req[i];
                    ++i;
                }
            }++i;
            while(split_req[i] != "," && split_req[i] != ")") ++i;
            ++i;
            pass_requests.requests2.insert(pair<string, string>(attr, str_index));
        }
    }
    return pass_requests;
}

/*
 * 判断某字符串是否为"< 数字 >"的格式,例如:
 *	< 0.93 >
 */
bool is_weight(const string& line){
	const int NONE=0, INT=1, DOT=2, FLOAT=3, END=4;
	string new_line = Utils::strip(line);
	if(new_line[0] != '<' || new_line[new_line.size()-1] != '>')
		return false;
	new_line = new_line.substr(1, new_line.size()-2);
	int state = NONE;
	for(int i=0; i<new_line.size(); i++){
		if(new_line[i]>=48 && new_line[i]<=57){
			if(state == NONE)
				state = INT;
			else if(state == DOT)
				state = FLOAT;
			else if(state == END)
				return false;
		}else if(new_line[i] == '.'){
			if(state == INT)
				state = DOT;
			else
				return false;
		}else if(new_line[i] == ' ' || new_line[i] == '\t'){
			if(state == DOT)
				return false;
			else if(state != NONE)
				state = END;
		}else
			return false;
	}

	if(state == DOT)
		return false;

	return true;
}

/*
 * 判断某字符串是否为正则表达式,例如:
 *	export exp=我(要|想)听(吻别|青花瓷) => request(title="$2") <0.9>;
 */
bool is_expr(const string& line){
	size_t idx;
	string new_line = Utils::strip(line);
	if(new_line[new_line.size()-1] != ';')
		return false;
	new_line = new_line.substr(0, new_line.size()-1);
	Utils::replace(new_line, '\t', ' ');

	new_line = Utils::strip(new_line);
	if(new_line.substr(0, 6) != "export")
		return false;

	new_line = Utils::strip(new_line.substr(6));
	idx = new_line.find("=");
	if(idx == string::npos)
		return false;

	new_line = Utils::strip(new_line.substr(idx+1));
	idx = new_line.find("=>");
	if(idx == string::npos)
		return false;

	new_line = Utils::strip(new_line.substr(idx+2));
	if(new_line.substr(0, 7) != "request")
		return false;
	
	new_line = Utils::strip(new_line.substr(7));
	if(new_line[0] != '(')
		return false;
	
	new_line = Utils::strip(new_line.substr(1));
	idx = new_line.find(")");
	if(idx == string::npos)
		return false;

	new_line = Utils::strip(new_line.substr(idx+1, new_line.size()));
	if(is_weight(new_line))
		return true;
	return false;
}


void DomainLex::extend_vocab(NFA *pnfa){
	set<State*> visited;
	queue<State*> sque;
	sque.push(pnfa->start);
	while(sque.size()>0){
		State *ps = sque.front();
		sque.pop();
		if(visited.find(ps) != visited.end())
			continue;
		visited.insert(ps);
		for(size_t i=0; i<ps->olinks.size(); i++){
			Link *pl = ps->olinks[i];
			if(pl->ltype==STRING && pl->cs->type==ORDINARY && ps->kflag){
				if(wrd_idx_map.find(pl->cs->str) == wrd_idx_map.end())
					wrd_idx_map[pl->cs->str] = wrd_idx_map.size();
				pl->cs->str_idx = wrd_idx_map[pl->cs->str];
				if(wrd_max_len < pl->cs->vec_str.size())
					wrd_max_len = pl->cs->vec_str.size();
			}
			sque.push(pl->end);
		}
	}
}


/*
 * 处理一行的正则表达式,为其建立对应的NFA
 */
Expression* DomainLex::expr_line(const string& line){
    if(!is_expr(line)){
        string err_msg = "Invalid expression line! \n\t";
        err_msg.append(line);
        throw std::runtime_error(err_msg);
    }

    size_t i = 0;
    vector<string> split_line;
    Utils::utf8strs(line.c_str(), split_line);
    
    string expr_name;
    while(split_line[i] != " ") ++i;
    while(split_line[i] != "="){
        if(split_line[i] == " "){
            ++i;
            continue;
        }else{
            expr_name += split_line[i];
            ++i;
        }
    }++i;

    string expr_value;
    while(split_line[i] != "="){
        if(split_line[i] == " "){
            ++i;
            continue;
        }else{
            expr_value += split_line[i];
            ++i;
        }
    }i+=2;
    
    string str_requests;
    while(split_line[i] != "<"){
        str_requests += split_line[i];
        ++i;
    }++i;

    string str_weight;
    while(split_line[i] != ">"){
        if(split_line[i] == " "){
            ++i;
            continue;
        }else{
            str_weight += split_line[i];
            ++i;
        }
    }

    //< 构建Expression
    PassRequests pass_requests = get_requests(str_requests);
    double weight = atof(str_weight.c_str());
	for(map<string, Variable*>::iterator iter = map_var_.begin();
		iter!=map_var_.end(); iter++){
	}
    NFA* pnfa = build_NFA(expr_value.c_str(), map_var_, true);


    Expression* expr = new Expression(expr_name, expr_value, pass_requests.requests1, pass_requests.requests2, weight, pnfa);
    
    return expr;
}



/*
 * 处理变量定义行,为该变量建立NFA
 */
void DomainLex::var_line(const string& line){
    int i=0;
    vector<string> split_line;
    Utils::utf8strs(line.c_str(), split_line);
	while(split_line.back() != ";")
		split_line.pop_back();
	split_line.pop_back();

    string var_name;
    while(split_line[i] != "="){
        if(split_line[i] == " "){
            ++i;
            continue;
        }else{
            var_name += split_line[i];
            ++i;
        }
    }++i;
    if(i == split_line.size())
      throw std::runtime_error("Invalid variable!");

    string var_value;
    while(i != split_line.size()){
        if(split_line[i] == " "){
            ++i;
            continue;
        }else{
            var_value += split_line[i];
            ++i;
        }
    }
    
    //< 直接更新变量,不检查重复直接覆盖
    map_var_.insert(make_pair(var_name, new Variable(var_name, var_value)));

    return;
}

/*
 * 处理注释行,不需要执行任何操作
 */
void DomainLex::anno_line(const string& line){
    return;
}

/*
 * 处理空行,不需要执行任何操作
 */
void DomainLex::void_line(const string& line){
    return;
}

}
