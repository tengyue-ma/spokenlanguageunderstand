#ifndef _NFA_H_
#define _NFA_H_

#include <map>
#include <vector>
#include <string>
#include <stdexcept>
#include <unordered_map>

#include "state.h"
#include "link.h"
#include "group.h"
#include "redis_wrapper.h"

using std::string;
using std::vector;
using std::map;
using std::unordered_map;

namespace SemanticRegex{

/*
 * 非确定有限状态自动机
 */
class NFA{
public:
	NFA();

	/*
	 * 拷贝构造函数，复制一个NFA
	 */
	NFA(const NFA* pnfa);

	/*
	 * 建立一个仅含两个状态和一条边的NFA
	 * 该条边的类型为ltype，注意，必须是功能边
	 */
	NFA(LinkType ltype);

	/*
	 * 建立一个仅含两个状态和一条边的NFA
	 * 该条边为普通类型，上面包含字符集
	 *	@str: 	字符串
	 *	@ctype:	字符串类型(普通，范围，非，数据库等）
	 */
	NFA(const string& str, CharSetType ctype=ORDINARY);

	~NFA();

public:
	/*
	 * 匹配用户输入的句子
	 *	raw_sentence  :	原始句子，如"我要听青花瓷"
	 *	sentence      :	原始句子按utf-8编码分成的多个字符,如上:
	 *					{"我", "要", "听", "青", "花", "瓷"}
	 *	vec_idx       :	原始句子所有可能词组在词表中的id号，如上句可能的词组为:
	 *					"我","我要","我要听","要","要听","要听青"...
	 *					若"要听"在词表中的id为3,则vec_idx[3]=3,表示"要听"的结束位置
	 *	log           :	查询数据库时,为了避免一些不必要的重复查询,把已查询的值记录下来
	 *	matched_string:	最后匹配到的字符串
	 *	con			  :	redis连接指针,查询redis数据库用
	 * 	return		  :	返回Group组成的数组,每个Group表示一个匿名组
	 * 说明: vec_idx是为了加快预匹配的速度,快速地过滤掉那些明显匹配不上的正则;
	 *		 log是为了加快匹配的速度,缩小redis的查询时间
	 */
	vector<Group> match(const string& raw_sentence, 
                        const vector<string>& sentence, 
						const vector< vector<size_t> >& vec_idx,
                        map<string, int>* log,
                        string& matched_string, 
                        RedisWrapper *redis_wrapper=NULL);

	/*
	 * 预匹配用户输入的句子,会大概地check一遍该正则中有哪些词必须出现,
	 * 否则肯定匹配不上.返回结果为与匹配成功的开始位置,比如从位置3开始
	 * 能匹配上,那么返回3.若肯定匹配不上,则返回最后一个位置,即字符串长度
	 *	raw_sentence:	原始句子，如"我要听青花瓷"
	 *	sentence    :	原始句子按utf-8编码分成的多个字符,如上:
	 *					{"我", "要", "听", "青", "花", "瓷"}
	 *	vec_idx     :	含义同match函数的注释
	 *	return      :	若预匹配成功,返回匹配成功的开始位置,例如:
	 *					 正则为: 听(青花瓷|吻别)
	 *					 句子为:	我要听青花瓷
	 *					 则返回值为2,表示告知match函数直接从2位置开始匹配,不要从0开始
	 *					若预匹配不成功,则返回最后一个字符,match函数相当于不再匹配
	 */
	size_t pre_match(const string& raw_sentence,
					const vector<string>& sentence,
					const vector< vector<size_t> >& vec_idx);

	/*
	 * 串联两个NFA,把pnfa指向的NFA链到当前NFA中,例如:
	 *	当前NFA为:	A -> B -> C
	 *	pnfa为:		D -> E -> F
	 * 那么执行完本函数后,当前NFA变为: A -> B -> C -> D -> E -> F
	 * 同时pnfa的内存空间将被释放,但是其内部状态节点并不会释放
	 */
	void concat(NFA* pnfa);

	/*
	 * 并联两个NFA,用于支持"|"或字符,把pnfa指向的NFA并联到当前NFA中,例如:
	 *	当前NFA为:	A -> B -> C
	 *	pnfa为:		D -> E -> F
	 * 则执行完本函数后,当前NFA变为:
	 *	     ↗ A -> B -> C ↘
	 *	  start 			end
	 *		 ↘ D -> E -> F ↗
	 */
	void multiple(NFA* pnfa);

	/*
	 * 使当前NFA变成可选状态,用于支持"?"操作,例如:
	 *	当前NFA为:	A -> B -> C
	 * 则执行完本函数后,当前NFA变为:
	 *		A -> B -> C
	 *		 \_______↗
	 */
	void optional();

	/*
	 * 使当前NFA重复0到无穷多次,用于支持"*"操作,例如:
	 *	当前NFA为:	A -> B -> C
	 * 则执行完本函数后,当前NFA变为:
	 *    ________
	 *   ↙        \
	 *	A -> B -> C
	 *   \________↗
	 */
	void repeat0();

	/*
	 * 使当前NFA重复1到无穷多次,用于支持"+"操作,例如:
	 *	当前NFA为:	A -> B -> C
	 * 则执行完本函数后,当前NFA变为:
	 *     ________
	 *    ↙        \
	 *	  A -> B -> C
	 */
	void repeat();

	/*
	 * 计算NFA中每个状态的epsilon闭包,方便后面匹配时获取,加快匹配速度
	 */
	void epsilon_closure();

	/*
	 * 增加捕获功能边以支持匿名捕获功能,用于支持"()"操作,例如:
	 *	当前NFA为:	A -> B -> C
	 * 则执行完本函数后,当前NFA变为:
	 *	  start --CAPS--> A -> B -> C --CAPE--> end
	 */
	void add_cap_links(unsigned idx);// 增加边以捕获该NFA中的内容

	/*
	 * 打印当前NFA,打印方法为广度优先遍历,输出所有状态,以及所有状态上的所有边
	 */
	void print(const char* dot_file=NULL);

	/*
	 * 释放当前NFA的占用内存,包括所有状态以及边的内存
	 */
	void destruct();

	/*
	 * 使当前NFA重复min_num到max_num次,用于支持"{m, n}",例如:
	 *	当前NFA为:	A -> B -> C
	 *	且要重复2到3次, 则执行完本函数后,当前NFA变为:
	 *	  A -> B -> C -> A -> B -> C -> A -> B -> C
	 *                                  \________↗
	 *	min_num	:	最小重复次数
	 *	max_num :	最大重复次数
	 */
	void repeatn(int min_num, int max_num);

public:
	//< NFA的开始状态
	State* start;
	//< NFA的结束状态
	State* end;

	//< 匿名组数量,正则里面有多少个括号,group_num就是多少
	unsigned group_num;

	//< NFA的宽度,例如:
	//<	 ↗ B -> C ↘
	//<	A          E
	//<  ↘ C -> D ↗
	//< 则其宽度为2
	size_t width;
};

}


#define YES(condition, message)				\
	if(condition){							\
		throw std::runtime_error(message);	\
	}

#define NO(condition, message) 				\
	if(!(condition)){						\
		throw std::runtime_error(message);	\
	}

#endif

