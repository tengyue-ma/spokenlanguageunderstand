#ifndef _VARIABLE_H_
#define _VARIABLE_H_

#include <string>

#include "nfa.h"

using std::string;

namespace SemanticRegex{

/*
 * Variable类用来表示变量定义
 */

class Variable{
public:
	/*
	 * 构造函数
	 *	var_name:	变量名
	 *	var_value:	变量值
	 * 例如对于变量定义: 操作=放|播放|唱
	 * 其变量名为"操作",变量值为"放|播放|唱"
	 */
    Variable(const string& var_name, const string& var_value);

    ~Variable();

public:
	//< 变量名,解释见构造函数
    string var_name_;

	//< 变量值,解释见构造函数
    string var_value_;

	//< 对应的NFA
    NFA* pnfa_;
};

}

#endif
