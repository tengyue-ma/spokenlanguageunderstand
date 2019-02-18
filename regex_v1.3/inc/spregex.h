#ifndef _PARSER_H_
#define _PARSER_H_

#include <map>
#include <vector>
#include <string>
#include <iconv.h>
#include <stdexcept>

#include "logger.h"
#include "comm_def.h"
#include "semantic.h"
#include "domain_lex.h"
#include "redis_wrapper.h"

using std::map;
using std::string;
using std::vector;

namespace SemanticRegex{

/*
 * 用于语义解析的正则匹配引擎
 */

class Regex{
public:
	/*
	 * 从配置文件构造一个Regex
	 */
	Regex(const string& cfg_file, hobotlog::LogModule *loginstance);
	~Regex();

public:
	/*
	 * 对所给句子进行匹配,返回所有能匹配到的Semantic
	 * 	sentence: 	待匹配句子
	 *	use_domain:指定在该领域下匹配,若不指定,则置为""
	 *	n_best:	nbest个数,默认为1,若要返回匹配到的所有结果,设为-1
	 *	gbk:		待匹配句子是否为gbk编码
	 *	ret:		nbest个Semantic指针组成的vector,注意,
	 *				必须外部释放这些指针指向的内存
	 */
	vector<Semantic*> match(const string& sentence,
							const string& use_domain="",
							int n_best=1, bool gbk=true);

	/*
	 * 对所给句子进行匹配,返回所有能匹配到的Semantic对应的json串
	 *	例如对于上面的例子，其返回结果为:
	 *		{"domain": "music", "artist": "周杰伦", "title": "青花瓷"}
	 *	sentence:	待匹配句子
	 *	use_domain:指定在该领域下匹配,若不指定,则置为""
	 *	n_best:	nbest个数,默认为1,若要返回匹配到的所有结果,设为-1
	 *	ret:		nbest个Semantic对应的json串组成的vector
	 */
	vector<string> parse(const string& sentence, 
				 		 const string& use_domain="",
				 		 int n_best=1);

private:
	/*
	 * 从配置文件载入所需资源,配置文件中指定了
	 * redis的ip及端口,有哪些领域,各领域的规则文件所在目录
	 * 读取完配置文件后,为所有领域创建相应的DomainLex
	 */
	int load_config(const string& cfg_file);

	/*
	 * 编码转换,由gbk转为utf-8
	 *	str_src:	原编码字符
	 *	str_des:	目标编码字符
	 */
	int gbk2utf8(const string& str_src, string& str_des);

	/*
	 * 编码转换,由utf-8转为gbk
	 *	str_src:	原编码字符
	 *	str_des:	目标编码字符
	 */
	int utf82gbk(const string& str_src, string& str_des);

	/*
	 * 把语义结果中的所有slot全部转换为gbk格式
	 */
	int gbk_sem(Semantic* sem);

public:
	//< redis数据库所在的服务器ip
	string redis_server;
	//< redis数据库端口
	int redis_port;
    //< redis数据库密码
    string redis_pswd;
	//< hiredis连接
	RedisWrapper *redis_wrapper;

	//< 领域名跟DomainLex的映射表
	map<string, DomainLex*> domain_lex;

	//< 所有领域下规则里面出现最长词的长度
	size_t wrd_max_len;

	hobotlog::LogModule *loginstance;

	//< 编码转换用的变量
	iconv_t cd_g2u;
	iconv_t cd_u2g;
};

}	// end of namespace SemanticRegex

#define VERIFY(condition, message)	\
	if(!condition){					\
		throw std::runtime_error(message);	\
	}

#endif

