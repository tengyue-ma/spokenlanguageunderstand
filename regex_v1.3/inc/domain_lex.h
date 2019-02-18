#ifndef _RESOLUTION_H_
#define _RESOLUTION_H_

#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <set>
#include <utility>

#include "logger.h"
#include "comm_def.h"
#include "semantic.h"
#include "variable.h"
#include "expression.h"
#include "redis_wrapper.h"

using std::string;
using std::vector;
using std::map;
using std::set;
using std::pair;
using std::unordered_map;

namespace SemanticRegex{

/*
 * 定义规则文件中每一行的类型
 *	INCLUDE:	包含文件声明,如: #include "comm/digit.lex"
 *	VARIABLE:	变量声明,如: 播放=播放|放|唱;
 *	EXPRESSION:	正则表达式声明,如:
 				export exp1=我要听(&<music_title>) => request(title="$1") <0.9>;
 *	VOID:		空行
 *	ANNOTATION:	注释行,如: #这是一行注释
 */
typedef enum _LineType{
    INCLUDE,
    VARIABLE,
    EXPRESSION,
    VOID,
    ANNOTATION
}LineType;

/*
 * 定义每条规则最终返回的内容,即request的内容,包含两种:
 *	requests1:	这是保存形如 (title="$1", artist="$2") 这种返回值 
 *				key分别为"title"和"artist", value分别为1和2
 *	requests2:	这是保存形如 (domain="music")这种返回值,
 *				key为"domain",相应的value为"music"
 */
typedef struct _PassRequests{
    vector< pair<string, size_t> > requests1;
    map<string, string> requests2;
}PassRequests;

/*
 * 定义整个领域的所有变量和正则表达式等
 */
class DomainLex{
public:
	/*
	 * 构造函数
	 *	name:		领域名
	 *	lex_dir:	该领域的规则所在目录
	 */
    DomainLex(const string& name, const string& lex_dir);

    ~DomainLex();

public:
	/* 用该正则表达式匹配一句话,由于支持n-best结果,所以返回vector<Semantic*>
	 *	raw_sentence:	原始句子,如"我要听青花瓷"
	 *	sentence:		按utf-8编码切割成单个字
	 *	log:			数据库查询缓存,用于避免重复查询数据库中相同的key
	 *	matched_string:	匹配到的字符串
	 *	con:			redis数据库连接
	 *	substr_idx_map:	原始句子各子串在原句子中的位置,用于预匹配加速
	 *	return:			返回多个Semantic结果
	 */
	vector<Semantic*> match(const string& raw_sentence,
                            const vector<string>& sentence,
                            map<string, int>* log,
                            string& matched_string,
							RedisWrapper *redis_wrapper,
							unordered_map< string, vector<size_t> >& substr_idx_map,
							hobotlog::LogModule *loginstance);

	vector<Semantic*> split_match(const string& raw_sentence,
                            const vector<string>& sentence,
                            map<string, int>* log,
                            string& matched_string,
							RedisWrapper *redis_wrapper,
							unordered_map< string, vector<size_t> >& substr_idx_map,
							hobotlog::LogModule *loginstance);

public:
	//< 保存一个domain下的所有变量
    map<string, Variable*> map_var_;
	//< 保存一个domain下的所有正则
    vector<Expression*> vec_expr_;

	//< 保存一个domain下所有正则中出现的字符串
	//< 并给每个字符串建立一个id映射表
	unordered_map<string, size_t> wrd_idx_map;

	//< 最大字符串字个数
	size_t wrd_max_len;

private:
	/*
	 * 遍历某目录下的所有规则文件
	 */
    void list_file(const string& field);

	/*
	 * 读取某文件中的正则,并为其建立相应的NFA
	 */
    void process_file(const string& filename, bool root = 1);

	/*
	 * 处理文件包含,把包含的文件的内容全部读取一遍
	 */
    void hash_line(const string& line);

	/*
	 * 处理正则表达式行,建立相应的NFA
	 */
    Expression* expr_line(const string& line);

	/*
	 * 处理变量定义行,建立相应的NFA
	 */
    void var_line(const string& line);

	/*
	 * 处理注释行
	 */
    void anno_line(const string& line);

	/*
	 * 处理空行
	 */
    void void_line(const string& line);

	/*
	 * 比较两个正则表达式的权重,若第一个正则的权重大于第二个,则返回true,否则返回false
	 */
    static bool compare_weight(const Expression* e1, const Expression* e2);

	/*
	 * 根据权重大小对所有的正则进行排序,从大到小
	 */
    void sort_expr();

	/*
	 * 判断一行是什么类型,是正则,变量定义,注释,还是文件包含等
	 */
    LineType check_line(const string& file_name, const string& line, const int& line_num);

	/*
	 * 获取某正则的request内容
	 */
    PassRequests get_requests(const string& str_requests); 

	/*
	 * 添加NFA中所有出现过的词到词表中
	 * 同时,生成每个NFA的边上的词对应的id
	 */
	void extend_vocab(NFA *pnfa);

private:
	//< 单个领域规则文件目录
	string lex_dir_;

	//< 领域名
    string field_name_;

	//< 领域下所有规则文件
    vector<string> files_;
};

}

#endif
