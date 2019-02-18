#include <iostream>
#include <stdexcept>
#include <vector>

#include "charset.h"
#include "utils.h"

using std::vector;

namespace SemanticRegex{

/*
 * 拷贝构造函数
 */
CharSet::CharSet(const CharSet* cs){
	type = cs->type;
	if(type == ORDINARY){
		str = cs->str;
        vec_str = cs->vec_str;
		str_idx = 0;
    } else if(type == NOT)
		except = cs->except;
	else if(type == RANGE || type == NOTRANGE){
		min = cs->min;
		max = cs->max;
	}else if(type == DATABASE){
		db_set = cs->db_set;
	}
}

/*
 * 根据字符集类型，创建相应的charset
 */
CharSet::CharSet(CharSetType type, const string& str){
	this->type = type;
	//< 若为普通字符集，按utf-8分割成字符串数组
	if(type == ORDINARY){
		this->str  = str;
        Utils::utf8strs(str.c_str(), this->vec_str);
		str_idx = 0;
	//< 若为非字符集，存到except字符串中
    }else if(type == NOT)
		this->except = str;
	//< 数据库类型
	else if(type == DATABASE){
		this->db_set = str;
	//< 范围字符或范围非字符
	}else if(type == RANGE || type == NOTRANGE){
		throw std::runtime_error("for RANGE charset type, "
		"you have to use another constructor!");
	}
}

/*
 * 构造范围字符的charset
 */
CharSet::CharSet(const string& min, const string& max, CharSetType type){
	this->type = type;
	this->min  = min;
	this->max  = max;
}

}

