#ifndef _SEMANTIC_H_
#define _SEMANTIC_H_

#include <vector>
#include <string>

using std::string;
using std::vector;

namespace SemanticRegex{

/*
 * 表示一个slot
 */
class Slot{
	public:
		Slot();
		Slot(const Slot& slot);
		Slot(string name, string value, int start=-1, int end=-1);
		~Slot();

	public:
		/*
		 * 判断当前slot与给定的slot是否相等
		 * 若slot名和slot值完全相等,返回true,
		 * 否则返回false
		 */
		bool operator==(const Slot& s);

	public:
		//< slot值在句子中的开始位置
		int start;
		//< slot值在句子中的结束位置
		int end;
		//< slot名,如"artist"
		string name;
		//< slot值,如"周杰伦"
		string value;
};

/*
 * 表示一句话的语义结果
 * 包括 领域、意图、slots、原始句子
 */
class Semantic{
	public:
		Semantic();
		~Semantic();

	public:
		/*
		 * 把语义结果写成json字符串
		 */
		string dumps();

		/*
		 * 判断两个语义结果是否完全相等
		 * 若领域,意图,slot个数及名字,取值完全相等,返回true
		 * 否则返回false
		 */
		bool operator==(const Semantic& sem);

		/*
		 * 赋值操作
		 */
		Semantic& operator=(const Semantic& sem);

		/*
		 * 增加一个slot,各项内容从slot复制
		 */
		void add_slot(Slot* slot);

		/*
		 * 增加一个slot
		 *	name:	slot名
		 *	value:	slot值
		 *	start:	slot值在原始句子的开始位置
		 *	end:	slot值在原始句子的结束位置
		 */
		void add_slot(string name, string value, int start=-1, int end=-1);

		/*
		 * 判断该语义结果是否包含某个slot
		 *	name:	slot名
		 */
		int has_slot(string name);

	public:
		//< 匹配字段总长度
        int match_len;      
		//< 用户输入的query
		string query;
		//< 语义项
		vector<Slot*> slots;
		//< 意图
		string intent;
		//< 领域
		string domain;
		//< 权重
		double weight;
};

}	// end of namespace SemanticRegex

#endif

