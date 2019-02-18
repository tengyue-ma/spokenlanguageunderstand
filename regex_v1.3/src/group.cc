#include "group.h"

namespace SemanticRegex{

/*
 * 默认构造函数,grp_idx=0,
 * 说明不表示任何一个匿名组
 */
Group::Group(){
	this->str	  = "";
	this->grp_idx = 0;
	this->start	  = 0;
	this->end	  = 0;
	this->name 	  = "";
	this->pre_group = NULL;
	this->total_group_len = 0;
}

Group::~Group(){
}

}
