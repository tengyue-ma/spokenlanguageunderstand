/*************************************************************************
	> File Name: slotType.h
	> Author: TengyueMa
	> Mail: lumujin@icloud.com
	> Created Time: 四  1/ 4 15:21:11 2018
 ************************************************************************/

#ifndef _SLOTTYPE_H
#define _SLOTTYPE_H

#include <map>
#include <string>
#include <vector>

using std::string;
using std::vector;

class slotType{
public:
    /*
     * @brief 构造函数
     * @param pair.first  slotname
     * @param pair.second dictpath
     */
    slotType();
    slotType(const vector<std::pair<string, string> > vec_dict);

    /*
     * @brief 析构函数
     */
    ~slotType();

public:
    /*
     * @brief 查询key的slotname
     */
    string getSlotname(const string value, 
                       string name);

    /*
     * @brief 加载词典信息到map
     */
    bool loadDict2Map(const string slotname, 
                      const string dictpath);

private:
    //< map
    std::map<string, string> dictmap;
};

#endif
/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
