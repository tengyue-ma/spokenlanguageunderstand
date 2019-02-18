#include <iostream>
#include "nfa_char_union.h"

namespace SemanticRegex{

Nfa_Char_Union::Nfa_Char_Union(NFA* pnfa){
	this->pnfa	= pnfa;
	this->c		= '\0';
}

Nfa_Char_Union::Nfa_Char_Union(char c){
	this->pnfa	= NULL;
	this->c		= c;
}

Nfa_Char_Union::~Nfa_Char_Union(){
}

}
