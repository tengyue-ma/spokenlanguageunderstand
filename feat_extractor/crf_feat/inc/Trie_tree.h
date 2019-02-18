/*************************************************************************
	> File Name: Trie_tree.h
	> Author: TengyueMa
	> Mail: lumujin@icloud.com
	> Created Time: 一  1/29 14:00:32 2018
 ************************************************************************/

#ifndef _TRIE_TREE_H
#define _TRIE_TREE_H

#include <fstream>
#include <vector>
#include <string>
//#include <boost/unordered_map.hpp>
#include <unordered_map>

using namespace std;

//< 节点类
class TrieNode{
public:
    /*
     * @brief 构造函数
     */
    TrieNode();
    
    /*
     * @brief 析构函数
     */
    ~TrieNode();
public:
    friend class TrieTree;

private:
    //< flag : 根结点到当前节点路径是否匹配
    bool flag;
    //< 子节点
    unordered_map<string, TrieNode*> children;
};

//< 字典树
class TrieTree{
public:
    /*
     * @brief 构造函数
     */
    TrieTree(const string filepath = "");
    TrieTree(const vector<string> filepaths);

    /*
     * @brief 析构函数
     */
    ~TrieTree();

public:
    /*
     * @brief load dict
     * @param filepath : 词典路径
     */
    int load_dict(const string filepath);

    /*
     * @brief 插入数据到tree
     * @param seq : 插入的数据
     */
    int insert(const string seq);

    /*
     * @brief 查找是否存在该路径
     */
    bool has_dict(const string seq);

    /*
     * @brief 按照最大匹配原则匹配并返回匹配长度
     */
    int match(const vector<string>& seq_vec, 
              int idx);

private:
    //< 根结点
    TrieNode* root;
    //< char矩阵
    vector<string> cha_vec;
};

#endif
/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
