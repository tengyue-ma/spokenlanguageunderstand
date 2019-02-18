#ifndef _GROUP_H_
#define _GROUP_H_

#include <string>
using std::string;

namespace SemanticRegex{

/*
 * 匿名捕获时用到的数据结构
 * 正则匹配后,要在原句子中作出特征标注,
 * 所以记录匿名捕获的开始和结束位置
 */
class Group{
public:
	Group();
	~Group();

public:
	//< 匿名组捕获的字符串
	string str;

	//< 匿名组开始位置
	size_t start;

	//< 匿名组结束位置，注意是不包含end位置的，例如
	//< "青花瓷播放一下",则"青花瓷"的start=0,end=3
	size_t end;
	
	//< 该匿名组的组号,组号从1开始,0表示不代表任何匿名组
	size_t grp_idx;

	//< 该匿名组的名称，一般存slot名
	string name;

	//< 前一个匿名组
	Group *pre_group;

	//< 到当前这个匿名组为止,所有匿名组的长度之和
	int total_group_len;
};

}

#endif
