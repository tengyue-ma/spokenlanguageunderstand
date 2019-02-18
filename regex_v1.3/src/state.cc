#include <queue>
#include "charset.h"
#include "link.h"
#include "state.h"
using std::queue;

namespace SemanticRegex{

/*
 * 构造函数，默认不是开始状态
 * 也不是结束状态，是关键状态
 */
State::State(){
	sflag = false;
	eflag = false;
	kflag = true;
	group_name = "";
}

State::~State(){
	vector<Link*>().swap(ilinks);
	vector<Link*>().swap(olinks);
	set<State*>().swap(closure);
	set<State*>().swap(key_closure);
}

/*
 * 计算该状态的逆向epsilon闭包，即只通过epsilon边可达此状态的状态集合
 * 采用队列，广度优先遍历的方法，从当前状态的入边考虑，只要有epsilon边，
 * 则继续往前遍历，否则停止该条路径
 */
set<State*> State::re_epsilon_closure(){
	set<State*> closure;
	set<State*> visited;
	queue<State*> sque;
	sque.push(this);
	while(sque.size()>0){
		State *ps = sque.front();
		sque.pop();
		if(visited.find(ps) != visited.end())
			continue;
		visited.insert(ps);
		bool flag = true;
		for(size_t i=0; i<ps->ilinks.size(); i++){
			Link *pl = ps->ilinks[i];
			State *pstart = pl->start;
			if(pl->ltype == STRING && pl->cs->type == EPSILON){
				sque.push(pstart);
				flag = false;
			}
		}
		if(flag){
			closure.insert(ps);
		}
	}
	return closure;
}

}

