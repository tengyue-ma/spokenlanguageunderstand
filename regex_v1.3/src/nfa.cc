#include <iostream>
#include <map>
#include <set>
#include <ctime>
#include <queue>
#include <cstdlib>
#include <cstring>
#include <fstream>

#include "nfa.h"
#include "nfa_char_union.h"
#include "utils.h"
#include "variable.h"
#include "queue.hpp"

using namespace std;

namespace SemanticRegex{

#define PUSH(que1, que2, que3, que4, que5, val1, val2, val3, val4, val5) \
	que1.push(val1);	\
	que2.push(val2);	\
	que3.push(val3);	\
	que4.push(val4);	\
	que5.push(val5);

#define POP(que1, que2, que3, que4, que5) \
	que1.pop();	\
	que2.pop();	\
	que3.pop();	\
	que4.pop();	\
	que5.pop();


//< 支持的元字符
char _meta_chars[] = {'\\', '$', '^', '*', '(', ')', '+', '[', ']', '{', '}', '?', '<', '>', '&', ':'};
set<char> meta_chars(_meta_chars, _meta_chars+sizeof(_meta_chars));


/*
 * 把集合v1中的元素加入到集合v中
 */
void extend(set<State*>& v, const set<State*>& v1){
	for(set<State*>::iterator iter=v1.begin();
		iter!=v1.end(); iter++)
		v.insert(*iter);
}


NFA::NFA(){
	start = NULL;
	end	  = NULL;
	width = 0;
}

/*
 * 拷贝构造函数,复制pnfa指向的NFA的所有状态和边
 */
NFA::NFA(const NFA* pnfa){
	start = new State();
	queue<State*> sque;
	queue<State*> sque1;
	sque1.push(start);
	sque.push(pnfa->start);

	set<State*> visited;
	map<State*, State*> smap;
	smap.insert(std::pair<State*, State*>(pnfa->start, start));

	while(sque.size()>0){
		State* ps  = sque.front();
		State* ps1 = sque1.front();
		sque.pop();
		sque1.pop();
		if(visited.find(ps) != visited.end())
			continue;
		visited.insert(ps);
		for(size_t i=0; i<ps->olinks.size(); i++){
			Link* pl   = ps->olinks[i];
			//< 若ps1->olinks[i]->end已经创建过了，则不能重复创建
			State* ps2 = NULL;
			if(smap.find(pl->end) != smap.end()){
				ps2 = smap.find(pl->end)->second;
			}else{
				ps2 = new State();
				smap.insert(std::pair<State*, State*>(pl->end, ps2));
			}
			if(pl->ltype == STRING){
				CharSet* cs = new CharSet(pl->cs);
				Link* pl1  = new Link(ps1, ps2, cs);
			}else
				Link* pl1  = new Link(ps1, ps2, pl->ltype);
			if(pl->end->eflag)
				end = ps2;
			sque.push(pl->end);
			sque1.push(ps2);
		}
	}

	//< 复制宽度
	width = pnfa->width;

	start->sflag = true;
	end->eflag = true;
}

/*
 * 创建仅包含一个ltype类型边的NFA,例如:
 * 	start --(CAPS)--> end
 */
NFA::NFA(LinkType ltype){
	YES(ltype==STRING, "misused constructor!");
	start = new State();
	end   = new State();
	Link* pl     = new Link(start, end, ltype);
	start->sflag = true;
	end->eflag   = true;
	width = 1;
}

/*
 * 创建仅包含一条STRING类型边的NFA,例如:
 *  start --播放--> end
 */
NFA::NFA(const string& str, CharSetType ctype){
	start = new State();
	end   = new State();
	CharSet* cs = new CharSet(ctype, str);
	Link* pl = new Link(start, end, cs);
	start->sflag 	= true;
	end->eflag 		= true;
	width = 1;
}

/*
 * 仅释放该NFA所占空间，并不释放
 * NFA中的边和状态所占的空间
 */
NFA::~NFA(){
}

/*
 * 释放状态及边所占空间
 */
void NFA::destruct(){
	set<State*> state_set;
	queue<State*> state_que;
	state_que.push(start);
	while(state_que.size()>0){
		State* ps = state_que.front();
		state_que.pop();
		if(state_set.find(ps) != state_set.end())
			continue;
		state_set.insert(ps);
		for(size_t i=0; i<ps->olinks.size(); i++){
			state_que.push(ps->olinks[i]->end);
			//< 删除这条边，同时会删除该边上的CharSet
			delete ps->olinks[i];
		}
		ps->olinks.clear();
		ps->ilinks.clear();
		//< 删除这个状态节点
		delete ps;
	}
}

ostream& operator<<(ostream& out, Group& group){
	out << group.grp_idx << ": \"" << group.str << 
	"[" << group.start << ", " << group.end << "]\"";
	return out;
}

/*
 * 预匹配,跟正常匹配(即match函数)不同的是,若通过了某条边link到达新状态s,
 * 则下一个考虑的不是s的epsilon闭包状态,而是s的关键状态包
 * 且若当前状态不是关键状态,则直接pass,不进行任何匹配
 */
size_t NFA::pre_match(const string& raw_sentence,
					const vector<string>& sentence,
					const vector< vector<size_t> >& vec_idx){

	size_t sentence_size = sentence.size();
	for(size_t i=0; i<sentence_size; i++){
	    queue<size_t> idx_que;
	    queue<State*> state_que;
		for(set<State*>::iterator iter=start->key_closure.begin();
			iter!=start->key_closure.end(); iter++){
			idx_que.push(i);
			state_que.push(*iter);
		}

		while(state_que.size()>0){
            size_t idx = idx_que.front();
            State *pstate = state_que.front();
            idx_que.pop();
            state_que.pop();

			if(pstate->eflag){
				return i;
			}

			//< 若pstate不是关键状态节点,则直接pass
			if(!pstate->kflag){
				continue;
			}

			//< 否则,考虑pstate的所有出边
			for(size_t j=0; j<pstate->olinks.size(); j++){
				Link *pl = pstate->olinks[j];
				State *reach_state = pl->end;
				bool flag = false;
				size_t new_idx = idx;
				//< 若出边是STRING类型,且字符为普通字符类型,
				//< 则查看字符串是否出现在用户的query中
				if(pl->ltype == STRING && pl->cs->type==ORDINARY){
					size_t str_idx = pl->cs->str_idx;
					for(size_t i=0; i<vec_idx[str_idx].size(); i++){
						if(vec_idx[str_idx][i] > idx){
							new_idx = vec_idx[str_idx][i];
							flag = true;
							break;
						}
					}
				//< 若出边是HEAD类型且idx不等于0,则肯定匹配不上
				}else if(pl->ltype == HEAD && idx != 0){
					return sentence_size;
				//< 若出边是TAIL类型且idx不等于最后一个位置,则肯定匹配不上
				}else if(pl->ltype == TAIL && idx != sentence_size-1){
					return sentence_size;
				}
				if(flag){
					for(set<State*>::iterator iter=reach_state->key_closure.begin();
						iter!=reach_state->key_closure.end(); iter++){
						idx_que.push(new_idx);
						state_que.push(*iter);
					}
				}
			}
		}
	}

	//< 若一个都未匹配上,则说明预匹配失败
	return sentence_size;
}

/*
 * 匹配语句sentence.匹配的具体过程如下:
 * 从开始状态出发,把
 *  (状态,位置,当前匿名组的开始位置,
 *   已经匹配好的最后一个Group,上一条具有group_name的边link)
 * 入队,然后从队列里面取值,从队列取出每一个值后,判断当前状态的所有出边类型
 * 若下一个字符符合条件,则把出边的到达状态的所有epsilon闭包加入队列中
 * 否则pass.以上过程循环重复,直到队列为空结束.
 * 然后抽取出所有Group,填到一个vector中,返回
 */
vector<Group> NFA::match(const string& raw_sentence,
						 const vector<string>& sentence,
						 const vector< vector<size_t> >& vec_idx,
						 map<string, int>* log,
						 string& matched_string, 
						 RedisWrapper *redis_wrapper){
	vector<Group> MATCH_GROUP;
	MATCH_GROUP.clear();

    if(raw_sentence == ""){
		return MATCH_GROUP;
	}

    matched_string = "";
    size_t sentence_size = sentence.size();
	
	//< 先进行预匹配
	size_t start_idx = pre_match(raw_sentence, sentence, vec_idx);

	//< 从预匹配成功的位置开始进行精确匹配
	//< 若预匹配失败,则返回的是raw_sentence的长度,则相当于直接pass
    for(size_t i=start_idx; i<sentence_size; i++){
		set<Group*> group_set;
		vector<size_t> final_mc_idx;
		vector<Group*> final_mc_group;

		//< 已经匹配到的位置
		queue<size_t> cur_idx_que;
		//< 已经匹配到的状态节点
		queue<State*> state_que;
		//< 当前group的开始位置
		queue<size_t> beg_idx_que;
		//< 到当前位置,已经匹配到的group节点
		queue<Group*> group_que;
		//< 记录上一条end状态具有group_name的边link
		queue<Link*>  link_que;

		for(set<State*>::iterator iter=start->closure.begin();
			iter!=start->closure.end(); iter++){
			PUSH(cur_idx_que, state_que, beg_idx_que, group_que, link_que,
				 i, *iter, sentence_size, NULL, NULL);
			// cout << "pushing (" << i << ", " << *iter << ", "
			// << sentence_size << ", NULL, NULL)" << endl;
		}

		while(state_que.size()>0){
			size_t idx    	= cur_idx_que.front();
			State* pstate 	= state_que.front();
			size_t beg_idx	= beg_idx_que.front();
			Group* group	= group_que.front();
			Link* link 		= link_que.front();
			POP(cur_idx_que, state_que, beg_idx_que, group_que, link_que);
			/*if(group){
				cout << "Poping (" << idx << ", " << pstate << ", " <<
				beg_idx << ", " << *group << ", " << link << ")" << endl;
			}else{
				cout << "Poping (" << idx << ", " << pstate << ", " <<
				beg_idx << ", NULL, " << link << ")" << endl;
			}*/

            if(pstate->eflag){
				final_mc_idx.push_back(idx);
				final_mc_group.push_back(group);
            }

            size_t olinks_size = pstate->olinks.size();
            for(size_t j=0; j<olinks_size; j++){
				bool flag = false;
				Link* pl = pstate->olinks[j];

				size_t new_idx     = idx+1;
				size_t new_beg_idx = beg_idx;
				Group  *new_group  = group;
				Link   *new_link   = link;
				if(pl->end->group_name!="")
					new_link = pl;

				if(idx<sentence_size && pl->ltype == STRING){
                    string str = pl->cs->str;
					if(pl->cs->type == ORDINARY){
						string tmp = Utils::join(sentence, idx, idx+pl->cs->vec_str.size());
						if(tmp == pl->cs->str){
							flag = true;
							new_idx = idx + pl->cs->vec_str.size();
						}
					}else if(pl->cs->type == DATABASE){
						redisReply *reply;
						//< 查询redis数据库,从前往后匹配,有一个不匹配则break
						for(size_t k=idx+1; k<=sentence_size; k++){
							string tmp = Utils::join(sentence, idx, k);
                            string key = pl->cs->db_set + "_" + tmp;
							string cmd = "GET " + key;
                            //< 查看是否已查询过该key
							int db_val = -1;
                            map<string, int>::iterator db_query = log->find(key);
                            if(db_query == log->end()){
				                reply = redis_wrapper->query_redis(cmd);
                                if(reply->str == NULL){
									db_val   = -1;
									(*log)[key] = -1;
                                }else if(reply->str[0]=='1'){
									db_val   = 1;
									(*log)[key] = 1;
                                }else{
									db_val   = 0;
									(*log)[key] = 0;
                                }
                                freeReplyObject(reply);
                            }else{
								db_val = db_query->second;
							}

							if(db_val == -1){
								break;
							}else if(db_val == 1){
                            	for(set<State*>::iterator iter=pl->end->closure.begin();
                                   iter!=pl->end->closure.end(); iter++){
									PUSH(cur_idx_que, state_que, beg_idx_que, group_que, link_que,
								    	k, *iter, beg_idx, group, new_link);
									/*if(group){
										cout << "\tpushing (" << k << ", " << *iter << ", " << 
										beg_idx << ", " << *group << ", " << new_link << ")" << endl;
									}else{
										cout << "\tpushing (" << k << ", " << *iter << ", " << 
										beg_idx << ", NULL, " << new_link << ")" << endl;
									}*/
                         		}
							}
						}
					}else if(pl->cs->type == RANGE){
						if(pl->cs->min <= sentence[idx] &&
					   	   pl->cs->max >= sentence[idx])
							flag = true;
					}else if(pl->cs->type == NOT){
						if(pl->cs->except != sentence[idx])
							flag = true;
					}else if(pl->cs->type == ANY){
						flag = true;
					}else if(pl->cs->type == NOTRANGE){
						if(sentence[idx] < pl->cs->min ||
						   sentence[idx] > pl->cs->max)
						   flag = true;
					}
				}else if(CAPS == pl->ltype){
					flag = true;
					new_idx = idx;
					new_beg_idx = idx;
				}else if(CAPE == pl->ltype){
					//< CAPE类型,说明匹配完了一个Group,因此需要创建一个新的Group来存储它
					flag = true;
					new_idx = idx;
					new_beg_idx = sentence_size;
					new_link = NULL;
					//< 创建一个新的Group
					new_group = new Group();
					group_set.insert(new_group);
					new_group->start = beg_idx;
					new_group->end   = idx;
					new_group->pre_group = group;
					new_group->grp_idx = pl->index;
					new_group->str = Utils::join(sentence, beg_idx, idx);
					if(link!=NULL && link->end->group_name!="")
						new_group->name = link->end->group_name;
					new_group->total_group_len = new_group->end - new_group->start;
					if(new_group->pre_group != NULL)
						new_group->total_group_len += new_group->pre_group->total_group_len;
				}else if(HEAD == pl->ltype){
					if(idx == 0){
						flag = true;
						new_idx = idx;
					}
				}else if(TAIL == pl->ltype){
					if(idx == sentence_size){
						flag = true;
						new_idx = idx;
					}
				}

				set<State*>::iterator iter;
				set<State*>& closure = pl->end->closure;
				if(flag){
					//< 把reach_state的epsilon闭包这些状态全部加入到队列中
					for(iter=closure.begin(); iter!=closure.end(); iter++){
						PUSH(cur_idx_que, state_que, beg_idx_que, group_que, link_que, 
							 new_idx, *iter, new_beg_idx, new_group, new_link);
						/*if(new_group){
							cout << "\tpushing (" << new_idx << ", " << *iter << ", " << 
							new_beg_idx << ", " << *new_group << ", " << new_link << ")" << endl;
						}else{
							cout << "\tpushing (" << new_idx << ", " << *iter << ", " << 
							new_beg_idx << ", NULL, " << new_link << ")" << endl;
						}*/
					}
				}
			}
		}

		//< 若有多个匹配结果，返回最长的一个
		if(final_mc_idx.size()>0){
			size_t max_idx = 0;
			for(size_t j=1; j<final_mc_idx.size(); j++)
				if(final_mc_idx[j] > final_mc_idx[max_idx])
					max_idx = j;
				else if(final_mc_idx[j]==final_mc_idx[max_idx] &&
						final_mc_group[j]!=NULL && final_mc_group[max_idx]!=NULL &&
						final_mc_group[j]->total_group_len < final_mc_group[max_idx]->total_group_len)
					max_idx = j;
			matched_string = Utils::join(sentence, i, final_mc_idx[max_idx]);
			Group *group = final_mc_group[max_idx];
			MATCH_GROUP.resize(group_num+1);
			while(group!=NULL)
			{
				size_t index = group->grp_idx;
				MATCH_GROUP[index].start = group->start;
				MATCH_GROUP[index].end   = group->end;
				MATCH_GROUP[index].grp_idx = index;
				MATCH_GROUP[index].name  = group->name;
				MATCH_GROUP[index].str   = group->str;
				MATCH_GROUP[index].pre_group = NULL;
				group = group->pre_group;
			}
		}

		for(set<Group*>::iterator iter=group_set.begin();
			iter!=group_set.end(); iter++){
			delete *iter;
		}

		if(final_mc_idx.size()>0)
			break;
	}

	return MATCH_GROUP;
}

/*
 * 为了能捕获到该NFA中的匹配内容
 * 在两端加上CAPS以及CAPE边
 */
void NFA::add_cap_links(unsigned idx){
	State* ps1 = new State();
	State* ps2 = new State();
	Link* pl1  = new Link(ps1, start, CAPS, idx);
	Link* pl2  = new Link(end, ps2, CAPE, idx);
	ps1->sflag = true;
	ps2->eflag = true;
	start = ps1;
	end   = ps2;
}

/*
 * 串联两个NFA，实现串联功能
 * 把pnfa->start状态的所有出边链到当前NFA的结束状态上
 * 再把pnfa->start状态的所有入边也链到当前NFA的结束状态上
 * 最后更新当前NFA的宽度
 */
void NFA::concat(NFA* pnfa){
	for(size_t i=0; i<pnfa->start->olinks.size(); i++){
		Link* pl = pnfa->start->olinks[i];
		pl->set_start(this->end);
	}
	for(size_t i=0; i<pnfa->start->ilinks.size(); i++){
		Link* pl = pnfa->start->ilinks[i];
		pl->set_end(this->end);
	}
	end->eflag = false;
	pnfa->start->sflag = false;
	if(!pnfa->start->kflag)
		end->kflag = false;
	this->end = pnfa->end;
	delete pnfa->start;

	//< 宽度设置
	if(width < pnfa->width)
		width = pnfa->width;

	delete pnfa;
}

/*
 * 或两个NFA，实现'|'功能
 * 创建两个新的状态s1和s2
 * 创建两条从s1出发的epsilon边,一条到start,另一条到pnfa->start
 * 创建两条到达s2的epsilon边,一条从end出发,另一条从pnfa->end出发
 * 将s1和s2分别设为当前NFA新的开始状态start和结束状态end
 * 更新宽度
 */
void NFA::multiple(NFA* pnfa){
	if(NULL == start){
		throw std::runtime_error("Cannot or an NFA with an empty NFA!");
	}else{
		//< "(ab)|(cd)"这种表达式会产生捕获的歧义性，因此强制用户按规范书写
		if(start->olinks.size()==1 && start->olinks[0]->ltype==CAPS){
			if(pnfa->start->olinks.size()==1 && pnfa->start->olinks[0]->ltype==CAPS)
				YES(true, "to correctly capture matched string, you are suggested to write\n"
					"regular expressions like \"(ab|cd)\" instead of \"(ab)|(cd)\"\n");
		}
		State *s1 = new State();
		State *s2 = new State();

		Link *pl1 = new Link(s1, start, new CharSet());
		Link *pl2 = new Link(s1, pnfa->start, new CharSet());

		Link *pl3 = new Link(end, s2, new CharSet());
		Link *pl4 = new Link(pnfa->end, s2, new CharSet());

		s1->sflag = true;
		s2->eflag = true;

		start = s1;
		end = s2;

		//< 注意更新宽度
		width = width + pnfa->width;

		delete pnfa;
	}
}

/*
 * 使得NFA可选，实现'?'功能
 * 创建一条从start到end状态的epsilon边即可
 * 但是,要同时设置从start状态开始,到end状态(不包含end)
 * 的所有状态全部为非关键状态
 */
void NFA::optional(){
	CharSet* cs = new CharSet();
	State *s1 = start, *s2 = end;
	//< 考虑匿名捕获的问题: (a|b)?
	if(start->olinks.size() == 1 && start->olinks[0]->ltype == CAPS)
		s1 = start->olinks[0]->end;
	if(end->ilinks.size() == 1 && end->ilinks[0]->ltype == CAPE)
		s2 = end->ilinks[0]->start;
	Link* pl = new Link(s1, s2, cs);

	//< 设置中间状态全部为非关键节点
	set<State*> visited;
	queue<State*> s_que;
	s_que.push(s1);
	while(s_que.size()>0){
		State* pstate = s_que.front();
		s_que.pop();
		if(visited.find(pstate) != visited.end())
			continue;
		visited.insert(pstate);
		if(pstate == s2 || !pstate->kflag)
			continue;
		for(size_t i=0; i<pstate->olinks.size(); i++){
			s_que.push(pstate->olinks[i]->end);
		}
		pstate->kflag = false;
	}
}

/*
 * 使得NFA可重复多次，实现'+'功能
 */
void NFA::repeat(){
	CharSet* cs = new CharSet();
	State *s1 = start, *s2 = end;
	if(start->olinks.size() == 1 && start->olinks[0]->ltype == CAPS)
		s1 = start->olinks[0]->end;
	if(end->ilinks.size() == 1 && end->ilinks[0]->ltype == CAPE)
		s2 = end->ilinks[0]->start;
	Link* pl = new Link(s2, s1, cs);
	//< 创建pl时,s1的sflag会被赋为false,所以再设一遍
	//< 同样,s2的eflag也会被赋为false,所以再设一遍
	start->sflag = true;
	end->eflag   = true;
}

/*
 * 使得NFA可重复0次或多次，实现'*'功能
 */
void NFA::repeat0(){
	optional();
	repeat();
}

/*
 * 使得NFA可重复m~n次，实现'{m, n}'功能
 */
void NFA::repeatn(int min_num, int max_num){
	unsigned index = 0;
	if(start->olinks.size() == 1 && start->olinks[0]->ltype == CAPS){
		if(end->ilinks[0]->ltype != CAPE)
			YES(true, "expression illegal!");
		State *s1 = start, *s2 = end;
		index = start->olinks[0]->index;
		this->start = start->olinks[0]->end;
		this->end   = end->ilinks[0]->start;
		this->start->sflag = true;
		this->end->eflag   = true;
		//< 删除原start->olinks[0]->end的入边中的CAPS边
		for(vector<Link*>::iterator iter=start->ilinks.begin();
			iter!=start->ilinks.end(); iter++){
			if(*iter == s1->olinks[0]){
				start->ilinks.erase(iter);
				break;
			}
		}
		//< 删除原end->ilinks[0]->start的出边中的CAPE边
		for(vector<Link*>::iterator iter=end->olinks.begin();
			iter!=end->olinks.end(); iter++){
			if(*iter == s2->ilinks[0]){
				end->olinks.erase(iter);
				break;
			}
		}
		delete s1->olinks[0];
		delete s2->ilinks[0];
		delete s1;
		delete s2;
	}

	//< 先备份当前NFA，因为当前NFA后面会扩展
	NFA* orig_nfa = new NFA(this);
	State* last_nfa_start = this->start;

	//< 指定了最小重复次数
	if(min_num > 0){
		for(size_t i=1; i<min_num; i++){
			last_nfa_start = this->end;
			NFA* spnfa = new NFA(orig_nfa);
			this->concat(spnfa);
		}
	}

	//< 未指定最大重复次数，即相当于可重复无限次
	if(max_num == -1){
		CharSet* cs = new CharSet();
		Link* pl = new Link(end, last_nfa_start, cs);
	//< 指定了最大重复次数，相当于增加max_num-min_num个NFA进去
	}else{
		int diff = max_num - min_num;
		vector<State*> start_states;
		if(min_num<=0){
			start_states.push_back(this->start);
			diff = max_num - 1;
		}
		for(size_t j=0; j<diff; j++){
			start_states.push_back(this->end);
			NFA* spnfa = new NFA(orig_nfa);
			this->concat(spnfa);
		}
		for(size_t k=0; k<start_states.size(); k++){
			CharSet* cs = new CharSet();
			Link* pl = new Link(start_states[k], this->end, cs);
		}
	}

	if(index)
		this->add_cap_links(index);
	//< print();

	delete orig_nfa;
}

/*
 * 计算NFA的epsilon闭包以及key闭包，从结束状态
 * 往前走，采用闭包回传的机制，结果
 * 保存到每个节点的closure变量中
 */
void NFA::epsilon_closure(){
	//< 从结束状态出发，往前走
	map<State*, unsigned> old_size;
	map<State*, unsigned> key_old_size;
	queue<State*> sque;
	sque.push(end);
	end->closure.insert(end);
	end->key_closure.insert(end);

	while( sque.size()>0 ){
		State* ps = sque.front();
		sque.pop();
		old_size.clear();
		key_old_size.clear();
		for(size_t i=0; i<ps->ilinks.size(); i++){
			Link* pl = ps->ilinks[i];
			State* pstart = pl->start;
			if(old_size.find(pstart) == old_size.end())
				old_size[pstart] = pstart->closure.size();
			if(key_old_size.find(pstart) == key_old_size.end())
				key_old_size[pstart] = pstart->key_closure.size();
			if(pl->ltype == STRING && pl->cs->type == EPSILON){
				extend(pstart->closure, ps->closure);
			}else{
				pstart->closure.insert(pstart);
			}
			//< 若边为STRING类型,而且字符集为普通字符,且为关键状态
			if(pl->ltype == STRING && pl->cs->type == ORDINARY && pstart->kflag){
				pstart->key_closure.insert(pstart);
			}else{
				extend(pstart->key_closure, ps->key_closure);
			}
		}
		set<State*> tmp_set;
		for(map<State*, unsigned>::iterator iter=old_size.begin();
			iter!=old_size.end(); iter++){
			//< 若iter->first指向的节点的闭包发生了变化，则需要把这个变化回传
			if(iter->second != iter->first->closure.size()){
				sque.push(iter->first);
				tmp_set.insert(iter->first);
			}
		}
		for(map<State*, unsigned>::iterator iter=key_old_size.begin();
			iter!=key_old_size.end(); iter++){
			//< 若iter->first指向的节点的key闭包发生了变化，则也需要把这个变化回传
			if(iter->second != iter->first->key_closure.size()){
				if(tmp_set.find(iter->first) == tmp_set.end())
					sque.push(iter->first);
			}
		}
	}
}

void NFA::print(const char* dot_file){
	cout << endl << endl;
	set<State*> ss;
	if(NULL == dot_file){
		cout << "\t\tStart state: " << this->start << endl;
		cout << "\t\tEnd state: " << this->end << endl;
	}
	queue<State*> sque;
	sque.push(start);
	while(sque.size()>0){
		State* ps = sque.front();
		sque.pop();
		if(ss.find(ps) != ss.end())	// 若未访问过，则pass掉
			continue;
		if(ps->sflag)
			cout << "\t\t+++ State " << ps << " (start state, ";
		else if(ps->eflag)
			cout << "\t\t+++ State " << ps << " (end state, ";
		else
			cout << "\t\t+++ State " << ps << ", ";
		if(ps->kflag)
			cout << "key state): ";
		else
			cout << "not-key state): ";
		cout << "epsilon closure( ";
		for(set<State*>::iterator iter=ps->closure.begin();
			iter!=ps->closure.end(); iter++){
			cout << *iter << ", ";
		}
		cout << "), ";
		cout << "key closure( ";
		for(set<State*>::iterator iter=ps->key_closure.begin();
			iter!=ps->key_closure.end(); iter++){
			cout << *iter << ", ";
		}
		cout << ")" << endl;
		for(size_t i=0; i<ps->olinks.size(); i++){
			cout << "\t\t\t";
			if(ps->olinks[i]->ltype == STRING){
					if(ps->olinks[i]->cs->type == ORDINARY)
						cout << ps->olinks[i]->cs->str;
					else if(ps->olinks[i]->cs->type == ANY)
						cout << "ANY";
					else if(ps->olinks[i]->cs->type == EPSILON)
						cout << "EPSILON";
					else if(ps->olinks[i]->cs->type == RANGE)
						cout << "(" << ps->olinks[i]->cs->min << " - " << ps->olinks[i]->cs->max << ")";
					else if(ps->olinks[i]->cs->type == NOT)
						cout << "^" << ps->olinks[i]->cs->except;
					else if(ps->olinks[i]->cs->type == NOTRANGE)
						cout << "^(" << ps->olinks[i]->cs->min << " - " << ps->olinks[i]->cs->max << ")";
					else if(ps->olinks[i]->cs->type == DATABASE)
						cout << "DATABASE: " << ps->olinks[i]->cs->db_set;
					else
						cout << "OTHER";
			}else if(ps->olinks[i]->ltype == CAPS){
				cout << "CAPS";
			}else if(ps->olinks[i]->ltype == CAPE){
				cout << "CAPE";
			}else if(ps->olinks[i]->ltype == HEAD){
				cout << "HEAD";
			}else if(ps->olinks[i]->ltype == TAIL){
				cout << "TAIL";
			}
			cout << " ---> " << ps->olinks[i]->end << endl;
			sque.push(ps->olinks[i]->end);
		}
		ss.insert(ps);
	}

	cout << endl << endl;
}

void print_nfa_vec(const vector<Nfa_Char_Union*>& nfa_vec){
	cout << "====== Nfa_Char_Union in stack ======" << endl;
	for(size_t i=0; i<nfa_vec.size(); i++){
		cout << "\telement " << i+1 << ":" << endl;
		if(nfa_vec[i]->pnfa == NULL){
			cout << "\t\t" << nfa_vec[i]->c << endl;
		}else{
			nfa_vec[i]->pnfa->print();
		}
	}
	cout << "###### Nfa_Char_Union in stack ######" << endl << endl;
}

/*
 * merge函数,在读到一个"|"或")"时,需要先把前面的NFA合并
 * 由于两者的合并过程是一样的,所以写成一个函数,两种case都可以调用
 *	nfa_vec:	Nfa_Char_Union组成的一个栈,存于vector中
 *	c	   :	当前处理的字符,仅可能为"|"或")"
 *	brac_num:	已经有多少个"("左括号,左括号个数大于1时,不增加CAPS和CAPE边
 *	group_num:	指明第几个匿名组
 *	expression:	当前是在给变量还是表达式创建NFA,若是表达式,则增加CAPS和CAPE以支持匿名捕获
 */
void merge(vector<Nfa_Char_Union*>& nfa_vec, char c, 
			int& brac_num, int& group_num, bool expression=true){
	YES(nfa_vec.size()==0 || nfa_vec.back()->pnfa==NULL, "expression illegal! merge");
	Nfa_Char_Union *cur_ptr=NULL, *pre_ptr=NULL;
	cur_ptr = pre_ptr = nfa_vec.back();
	nfa_vec.pop_back();
	while(nfa_vec.size()>0){
		cur_ptr = nfa_vec.back();
		if(cur_ptr->pnfa == NULL){
			if(cur_ptr->c == '|'){
				delete cur_ptr;
				nfa_vec.pop_back();
				cur_ptr = nfa_vec.back();
				YES(NULL==cur_ptr->pnfa, "expression illegal! merge");
				cur_ptr->pnfa->multiple(pre_ptr->pnfa);
			}else if(cur_ptr->c == '('){
				if(c==')'){
					delete cur_ptr;
					nfa_vec.pop_back();
					brac_num--;
					if(expression && 0 == brac_num){
						group_num++;
						pre_ptr->pnfa->add_cap_links(group_num);
					}
				}
				//< 在最后合并时，肯定已经没有了括号
				break;
			}else{
				YES(true, "expression illegal!");
			}
		}else{
			cur_ptr->pnfa->concat(pre_ptr->pnfa);
		}
		delete pre_ptr;
		pre_ptr = cur_ptr;
		nfa_vec.pop_back();
	}
	nfa_vec.push_back(pre_ptr);
}

/*
 * 根据正则表达式建立一个NFA
 *	expr:		正则表达式内容
 *	var_table:	正则表达式可能会引用到变量,因此需要一张变量表,key为变量表达式,value为相应的NFA
 *	expression:	是规则中的export 表达式类型还是变量定义
 *	return:		返回最终的NFA
 */
NFA* build_NFA(const char* expr, map<string, Variable*>& var_table, bool expression=true){
	//< 一元操作符: ? * + ^
	//<	二元操作符:  | ( ) [ ] { } -
	vector<string> exprs;

	//< 先切割出所有的utf-8字符
	Utils::utf8strs(expr, exprs);

	//< 栈
	vector<Nfa_Char_Union*> nfa_vec;
	//< 括号数量
	int brac_num = 0;
	//< 匿名捕获的数量
	int group_num = 0;

	for(size_t i=0; i<exprs.size(); i++){
		if(exprs[i].size()==1){
			switch(exprs[i][0]){
				case '\\':
				{
					//< 转义，后面必须跟元字符
					YES(i+1>=exprs.size(), "expression illegal, \
						\\ must be followed by a meta char!");
					YES(exprs[i+1].size()!=1, "expression illegal, \
						\\ must be followed by a meta char!");
					YES(meta_chars.find(exprs[i+1][0])==meta_chars.end(),
						"expression illegal, \\ must be followed by a meta char!");
					i++;
					//< 当成普通的字符加入进去
					nfa_vec.push_back(new Nfa_Char_Union(new NFA(exprs[i])));
					break;
				}

				case '|':
				{
					merge(nfa_vec, '|', brac_num, group_num);
					nfa_vec.push_back(new Nfa_Char_Union('|'));
					break;
				}

				case '?':
				{
					YES(nfa_vec.empty() || nfa_vec.back()->pnfa==NULL, "expression illegal for ? !");
					nfa_vec.back()->pnfa->optional();
					break;
				}

				case '*':
				{
					YES(nfa_vec.empty() || nfa_vec.back()->pnfa==NULL, "expression illegal for * !");
					nfa_vec.back()->pnfa->repeat0();
					break;
				}

				case '+':
				{
					YES(nfa_vec.empty() || nfa_vec.back()->pnfa==NULL, "expression illegal for + !");
					nfa_vec.back()->pnfa->repeat();
					break;
				}

				case '^':
				{
					if(i == 0)
						nfa_vec.push_back(new Nfa_Char_Union(new NFA(HEAD)));
					else
						YES(true, "expression illegal! '^' must be the "
						"first character or follow '\\'!");
					break;
				}

				case '$':
				{
					//< 代表元字符，表示结尾
					if(i == exprs.size()-1){
						YES(nfa_vec.back()->pnfa==NULL, "expression illegal! "
						"'$' must be the last character or follow '\\'!");
						nfa_vec.push_back(new Nfa_Char_Union(new NFA(TAIL)));
					}else{
						YES(true, "expression illegal! '$' must be "
						"the last character or follow '\\'!");
					}
					break;
				}

				case '{':
				{
					//< 若后面是数字，则为重复次数,否则当成普通字符串
					i++;
					bool flag = true;
					string min="", max="";
					int min_num = -1, max_num = -1;
					while(i<exprs.size()){
						if(exprs[i].size() == 1){
							if(exprs[i][0] == ','){
								if(flag)
									flag = false;
								else
									YES(true, "exression illegal!");
							}else if((exprs[i][0]>=48 && exprs[i][0]<=57) || exprs[i][0]==' '){
								if(flag)
									min += exprs[i];
								else
									max += exprs[i];
							}else
								break;
						}else{
							break;
						}
						i++;
					}
					min = Utils::strip(min);
					max = Utils::strip(max);
					//< 不以'}'结尾，或上限及下限都不存在，则直接当成普通字符串考虑
					if(i==exprs.size() || exprs[i]!="}" || (min.size()==0 && max.size()==0))
						YES(true, "expression illegal!");

					if(min.size()>0)
						min_num = Utils::str2uint(min);
					if(max.size()>0)
						max_num = Utils::str2uint(max);
					//< 若未指定最大值，则最小值肯定已指
					//< ','未出现，说明最大值最小值相同
					if(-1 == max_num){
						if(flag)
							max_num = min_num;
					}else{
						YES(max_num<min_num, "the maximum repeat "
						"number must be larger than the minimum!");
					}
					YES(nfa_vec.back()->pnfa==NULL, "expression illegal! "
						"the '{' must follow a concrete regular expression!");
					nfa_vec.back()->pnfa->repeatn(min_num, max_num);
					break;
				}

				case '}':
				{
					nfa_vec.push_back(new Nfa_Char_Union(new NFA(exprs[i])));
					break;
				}

				case '[':
				{
					//< 字符集合
					i++;
					vector<CharSet*> cs_vec;
					for(; i<exprs.size() && exprs[i]!="]"; i++){
						if(exprs[i] == "-"){
							//< 后面已经没有了字符
							if(i+1>=exprs.size() || exprs[i+1]=="]"){
								cs_vec.push_back(new CharSet(ORDINARY, "-"));
							//< 或者前面没有字符
							}else if(cs_vec.size()<=0 || cs_vec.back()->type!=ORDINARY){
								cs_vec.push_back(new CharSet(ORDINARY, "-"));
							//< 否则，当成RANGE字符集
							}else{
								CharSet* cs = cs_vec.back();
								cs_vec.pop_back();
								i++;
								cs_vec.push_back(new CharSet(cs->str, exprs[i], RANGE));
								delete cs;
							}
						}else if(exprs[i] == "^"){
							//< 后面已经没有了字符，则表达式非法
							if(i+1>=exprs.size() || exprs[i+1]=="]")
								YES(true, "expression illegal!");
							cs_vec.push_back(new CharSet(NOT, exprs[i]));
						}else if(exprs[i] == "\\"){
							i++;
							YES(i>=exprs.size(), "expression illegal!");
							YES(exprs[i].size()!=1 || meta_chars.find(exprs[i][0])==meta_chars.end(),
							"\\must be followed by meta characters like * + . ^ $ !");
							cs_vec.push_back(new CharSet(ORDINARY, exprs[i+1]));
						}else{
							cs_vec.push_back(new CharSet(ORDINARY, exprs[i]));
						}
					}
					if(i==exprs.size()){
						//< 若表达式非法,先释放空间再抛出异常
						for(size_t j=0; j<cs_vec.size(); j++)
							delete cs_vec[j];
						cs_vec.clear();
						YES(true, "expression illegal!");
					}
					NFA* pnfa 	 = new NFA();
					State* start = new State();
					State* end   = new State();
					//< 处理栈里面的非字符集
					for(size_t j=0; j<cs_vec.size(); j++){
						if(cs_vec[j]->type == NOT){
							YES(j+1>=cs_vec.size(), "expression illegal!");
							//< 先删除当前的CharSet
							delete cs_vec[j];
							j++;
							if(cs_vec[j]->type == ORDINARY){
								cs_vec[j]->type   = NOT;
								cs_vec[j]->except = cs_vec[j]->str;
								cs_vec[j]->str 	  = "";
							}else if(cs_vec[j]->type == RANGE){
								cs_vec[j]->type   = NOTRANGE;
							}else{
								delete start;
								delete end;
								delete pnfa;
								YES(true, "expression illegal!");
							}
						}
						Link* pl = new Link(start, end, cs_vec[j]);
						pnfa->width++;
					}
					start->sflag = true;
					end->eflag = true;
					pnfa->start = start;
					pnfa->end   = end;
					cs_vec.clear();
					nfa_vec.push_back(new Nfa_Char_Union(pnfa));
					break;
				}

				case ']':
				{
					nfa_vec.push_back(new Nfa_Char_Union(new NFA(exprs[i])));
					break;
				}

				case '.':
				{
					//< 通配符
					nfa_vec.push_back(new Nfa_Char_Union(new NFA(exprs[i], ANY)));
					break;
				}

				case '<':
				{
					//< 变量开始的标记
					string var_name = "";
					for(++i; i<exprs.size() && exprs[i]!=">"; i++)
						var_name += exprs[i];
					if(i==exprs.size() || var_name.size()==0)
						YES(true, "expression illegal! '<' must follow '\\' or variable names ended by '>'!");
					if(var_table.find(var_name) == var_table.end()){
						string msg = "expression illegal! variable " + var_name + " undefined!";
						YES(true, msg);
					}else if(var_table[var_name]->pnfa_ == NULL){
						Variable *var = var_table[var_name];
						var->pnfa_ = build_NFA(var->var_value_.c_str(), var_table, false);
					}
					nfa_vec.push_back(new Nfa_Char_Union(new NFA(var_table[var_name]->pnfa_)));
					break;
				}

				case '>':
				{
					nfa_vec.push_back(new Nfa_Char_Union(new NFA(exprs[i])));
					break;
				}

				case '&':
				{	//< 数据库标志，说明该变量来自数据库
					YES(i+1>=exprs.size()||exprs[i+1]!="<", "expression illegal for case &!");
					string set_name;
					for(i += 2; i<exprs.size()&&exprs[i]!=">"; i++)
						set_name += exprs[i];
					YES(i>=exprs.size(), "expression illegal for case &!");
					nfa_vec.push_back(new Nfa_Char_Union(new NFA(set_name, DATABASE)));
					break;
				}

				case '(':
				{
					//< 说明入栈了一个左括号
					nfa_vec.push_back(new Nfa_Char_Union('('));
					brac_num++;
					break;
				}

				case ')':
				{
					//< 往前合并，直到遇见第一个左括号'('为止
					merge(nfa_vec, ')', brac_num, group_num, expression);
					break;
				}

				case ':':
				{
					//< 往后读该匿名组的名称,匿名组名称只能包含大小写字母
					string group_name = "";
					i++;
					while(i<exprs.size() && exprs[i] == " ")
						i++;
					while(i<exprs.size() && exprs[i].size()==1){
						bool flag = false;
						if(exprs[i][0]>=65 && exprs[i][0]<=90)
							flag = true;
						else if(exprs[i][0]>=97 && exprs[i][0]<=122)
							flag = true;
						else if(exprs[i][0]=='_' || (exprs[i][0]>=48 && exprs[i][0]<=57 && group_name.size()>0))
							flag = true;
						if(!flag)
							break;
						else
							group_name += exprs[i];
						i++;
					}
					YES(group_name.size()<=0, "':' must be followed by a group name consist of alphabet, digits and '_'!")
					//< 以':'定义的匿名组名称只能包含在最外层的匿名组中
					//< YES(brac_num!=1, "':' group name must be used in first layer '()' group!");
					YES(nfa_vec.empty() || nfa_vec.back()->pnfa==NULL, "':' must follow NFAs!");
					set<State*> closure = nfa_vec.back()->pnfa->end->re_epsilon_closure();
					for(set<State*>::iterator it=closure.begin(); it!=closure.end(); it++){
						(*it)->group_name = group_name;
					}
					i--;
					break;
				}

				default:
				{
					//< 单字情况，直接push一个新的NFA即可
					nfa_vec.push_back(new Nfa_Char_Union(new NFA(exprs[i])));
					break;
				}
			}
		}else{
            string word = exprs[i++];
            while(i<exprs.size() && exprs[i].size()>1){
				bool tflag = false;
				//< 若是最后一个字符,自然可以添加
				if(i+1 == exprs.size())
					tflag = true;
				//< 否则,后一个字符必须不为{'?', '+', '{'}中的字符
				else if(exprs[i+1]!="?" && exprs[i+1]!="+" && exprs[i+1]!="{")
					tflag = true;
				if(!tflag)
					break;
                word += exprs[i];
                i++;
            } i--;
			nfa_vec.push_back(new Nfa_Char_Union(new NFA(word)));
		}
	}

	//< 合并nfa_vec中的所有NFA
	YES(nfa_vec.empty(), "empty regular expression!");
	merge(nfa_vec, '|', brac_num, group_num);

	//< 为变量建立NFA时，不计算epsilon闭包
	//< 为正则表达式建立NFA时，要计算epsilon闭包
	if(expression){
		nfa_vec.back()->pnfa->epsilon_closure();
		nfa_vec.back()->pnfa->group_num = group_num;
	}

	return nfa_vec.back()->pnfa;
}

}
