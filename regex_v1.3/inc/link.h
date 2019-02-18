#ifndef _LINK_H_
#define _LINK_H_

#include "charset.h"

namespace SemanticRegex{

class State;

/*
 * 状态之间连接边的类型，有以下几类:
 *	STRING:	字符类型，匹配时经过某些字符才能通过这条边
 *	EMPTY:	空边，表示可以不输入任何字符到达下一状态
 *	CAPS:	功能边，表示匿名组左括号"("
 *	CAPE:	功能边，表示匿名组右括号")"
 *	HEAD:	功能边，表示必须以某些字符开头
 *	TAIL:	功能边，表示必须以某些字符结尾
 */
typedef enum _LinkType{
	STRING,
	EMPTY,
	CAPS, CAPE,
	HEAD, TAIL
}LinkType;


/*
 * 状态之间的连接边
 */
class Link{
public:
	Link();
	~Link();

	/*
	 * 同时设置出发状态，结束状态，以及字符集cs
	 */
	Link(State *start, State *end, CharSet *cs);

	/*
	 * 同时设置出发状态，结束状态，边类型
	 * 该构造函数用于构造功能边
	 */
	Link(State *start, State *end, LinkType lt, unsigned index=0);

public:
	/*
	 * 设置该link的出发状态，同时把当前link加入到start的olinks变量中
	 */
	void set_start(State *start);

	/*
	 * 设置该link的到达状态，同时将该link加入到end的ilinks变量中
	 */
	void set_end(State *end);

public:
	//< 出发状态
	State *start;

	//< 到达状态
	State *end;

	//< 字符集指针
	CharSet *cs;

	//< 该link的类型
	LinkType ltype;

	//< 匿名捕获后所在的index
	unsigned index;
};

}

#endif

