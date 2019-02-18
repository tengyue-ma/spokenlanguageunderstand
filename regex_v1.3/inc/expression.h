#ifndef _EXPRESSION_H_
#define _EXPRESSION_H_

#include <utility>
#include <string>
#include <vector>
#include <map>

#include "nfa.h"
#include "group.h"
#include "redis_wrapper.h"

using std::string;
using std::vector;
using std::pair;
using std::map;

namespace SemanticRegex{

/*
 * Expression类用来表示一条正则表达式,形如:
 *	export exp=我(要|想)(听|听一下)(&<music_title>)(这首歌) => request(title="$3") <0.9>;
 * 该正则表达式的名称为exp,表达式的值为"我(要|想)...(这首歌)"
 */

class Expression{
public:
    Expression();
    ~Expression();
	/*
	 * 构造函数
	 *	expr_name:	正则名称,如上面的"exp"
	 *	expr_value:	正则表达式字符串,如上面的"我(要|想)...(这首歌)"
	 *	requests:	该表达式的返回值,如上面的 title="$3"
	 *	weight:		该表达式的权重,如上面的0.9
	 *	pnfa:		正则表达式字符串对应的NFA
	 */
    Expression(const string& expr_name,
               const string& expr_value,
               const vector< pair<string, size_t> > requests,
               const map<string, string> requests1,
               const double& weight,
               NFA* pnfa);

public:
	/*
	 * 匹配函数,给定一个句子,进行匹配,返回一个匿名组
	 *	raw_sentence:	原始句子,如"我要听青花瓷这首歌"
	 *	sentence:		原始句子按utf-8分割后得到的string向量
	 *	log:			key到value的映射表,查询数据库时用来加速
	 *	matched_string:	最终匹配到的字符串
	 *	vec_idx:		所属领域下的所有词在原始句子中的结束位置,若为0,表示不存在
	 *	return:			匿名组
	 */
	vector<Group> match(const string& raw_sentence,
                        const vector<string>& sentence, 
                        map<string, int>* log, 
                        string& matched_string, 
						RedisWrapper *redis_wrapper,
						const vector< vector<size_t> >& vec_idx);

	/*
	 * 判断两个正则表达式是否相等,若
	 * 两者的表达式内容完全相同,则相等
	 */
    bool operator==(const Expression& e2);
    void print();

public:
	//< 该正则表达式的名称,如一开始的"exp"
    string expr_name_;
	//< 该正则表达式的内容,如一开始的"我(要|想)...(这首歌)"
    string expr_value_;

	//< 返回请求的匿名组内容,如 "title" => 3
    vector< pair<string, size_t> > requests_;

	//< 返回请求的其他内容,如 "domain" => "music"
    map<string, string> requests1_;

	//< 正则表达式的权重
    double weight_;

	//< 正则表达式的内容对应的NFA
    NFA* pnfa_;
};

}

#endif


