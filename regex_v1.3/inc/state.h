#ifndef _STATE_H_
#define _STATE_H_

#include <set>
#include <string>
#include <vector>

using std::vector;
using std::string;
using std::set;

namespace SemanticRegex{

class Link;

/*
 * NFA中的状态节点类型
 */
class State{
public:
	State();
	~State();

public:
	/*
	 * 计算该节点的逆向epsilon闭包，即只通过epsilon边可达此状态的状态集合
	 */
	set<State*> re_epsilon_closure();

public:
	//< 入边列表
	vector<Link*> ilinks;
	//< 出边列表
	vector<Link*> olinks;

	//< epsilon闭包，表示仅通过epsilon边能到达的状态集合
	set<State*> closure;
	//< 关键状态闭包
	set<State*> key_closure;

	//< 开始状态标志位，若为NFA的开始状态，则为true，否则为false
	bool sflag;
	//< 结束状态标志位，若为NFA的结束状态，则为true，否则为false
	bool eflag;
	//< 关键状态标志位，若有epsilon出边，且出边数大于等于2，
	//< 则说明不是关键状态，即可以不通过该状态到达NFA的结束状态
	bool kflag;

	//< 当前状态所在匿名组的名称
	string group_name;
};

}

#endif
