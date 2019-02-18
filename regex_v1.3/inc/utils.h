#ifndef _UTILS_H_
#define _UTILS_H_

#include <vector>
#include <string>

using std::vector;
using std::string;

namespace Utils{
	/*
	 * 删除字符串左端的空格以及tab键
	 *	pstr:	源字符串指针
	 *	index:	左边第一个非空格及非tab的字符位置
	 */
	void lstrip(const char* pstr, unsigned& index);

	/*
	 * 删除字符串右端的空格以及tab键
	 *	pstr:	源字符串指针
	 *	index:	右边第一个非空格及非tab的字符位置
	 */
	void rstrip(const char* pstr, unsigned& index);

	/*
	 * 删除字符串左右两端的空格以及tab键
	 *	pstr:	源字符串指针
	 *	index1:	左边第一个非空格及非tab的字符位置
	 *	index2:	右边第一个非空格及非tab的字符位置
	 */
	void strip(const char* pstr, unsigned& idx1, unsigned& idx2);

	/*
	 * 删除字符串左右两端的空格以及tab键
	 *	return:	结果字符串
	 */
	string strip(const string& pstr);

	/*
	 * 按某指定字符切割字符串，并将切割结果放在vector字符串中
	 *	pstr:	源字符串
	 *	strs:	string组成的vector，存放切割结果
	 *	sep:	切割字符
	 */
	void split(const char* pstr, vector<string>& strs, const char* sep=NULL);

	/*
	 * 按utf-8编码，将字符串分成多个字符，主要是切分中文字符
	 *	str:		源字符串
	 *	split_strs:	切分结果
	 *	ret:		切分失败(如解码有误)返回-1，否则返回0
	 */
	int utf8strs(const char* str, vector<string>& split_strs);

	/*
	 * 按gbk编码，将字符串分成多个字符，主要是切分中文字符
	 *	str:		源字符串
	 *	split_strs:	切分结果
	 *	ret:		切分失败(如解码有误)返回-1，否则返回0
	 */
	int gbkstrs(const char* str, vector<string>& split_strs);

	/*
	 * 将一个vector v1中的元素添加到vector v中
	 */
	template<typename T> void extend(vector<T>& v, const vector<T>& v1);

	/*
	 * 字符串转成正整数，例如: "125" => 125
	 */
	unsigned str2uint(const string& str);

	/*
	 * 正整数转字符串，例如: 125 => "125"
	 */
	string uint2str(unsigned number);

	/*
	 * 从start位置开始，到end位置结束(不包含end位置)，把vector<string>中的字符串连接起来
	 *	strs:	string数组
	 *	start:	起始位置
	 *	end:	结束位置，默认为到最后
	 */
	string join(const vector<string>& strs, int start=0, int end=-1);

	/*
	 * 字符串替换函数，将字符串str中的subc字符全部替换为字符repc
	 */
	void replace(string& str, char subc, char repc);
}

#endif
