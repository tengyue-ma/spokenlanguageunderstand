#ifndef _CHARSET_H_
#define _CHARSET_H_

#include <string>
#include <vector>

using std::string;
using std::vector;

namespace SemanticRegex{

/*
 * 字符集类型
 *	ORDINARY: 	普通字符,如"我"
 *	RANGE:		min和max之间如"[0-9]"
 *	EPSILON:	epsilon字符类型
 *	NOT:		非字符如"[^我]"
 *	NOTRANGE:	非min和max之间如"[^0-9]"
 *	ANY:		通配符"."
 *	DATABASE:	数据库类型
 */
typedef enum _CharSetType{
	ORDINARY,
	RANGE,
	EPSILON,
	NOT,
	NOTRANGE,
	ANY,
	DATABASE
}CharSetType;


/*
 * 字符集类,存储Link上的字符
 */
class CharSet{
	public:
		CharSet(const CharSet* cs);
		CharSet(CharSetType type=EPSILON, const string& str="");
		CharSet(const string& min, const string& max, CharSetType=RANGE);

	public:
		//< 该字符的类型
		CharSetType type;

		//< 当为ORDINARY字符时，str为相应的字符串
		//< vec_str为str按utf-8分割组成的字符串数组
		//< str_idx为字符串str在nfa的字符串词典中的index，即id号
		string str;
        vector<string> vec_str;
		size_t str_idx;

		//< type为RANGE时，min和max表示范围的最小值和最大值
		string min;
		string max;

		//< type为NOT的情况下，except表示非字符串
		string except;

		//< type为DATABASE，set_name才使用
		string db_set;
};

}

#endif

