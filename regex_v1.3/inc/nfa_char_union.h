#ifndef _NFA_CHAR_UNION_H_
#define _NFA_CHAR_UNION_H_

namespace SemanticRegex{

class NFA;

/*
 * 构建状态机时用到的class
 * 可以表示一个字符，或者一个有限状态机
 * 若NFA指针为空，则为一个字符，存与c中,如'(', '|'这种
 * 否则表示一个NFA指针
 */
class Nfa_Char_Union{
public:
	Nfa_Char_Union(NFA* pnfa);
	Nfa_Char_Union(char c);
	~Nfa_Char_Union();

public:
	NFA* pnfa;
	char c;
};

}

#endif
