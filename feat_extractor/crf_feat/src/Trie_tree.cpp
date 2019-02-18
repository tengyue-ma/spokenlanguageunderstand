/*************************************************************************
	> File Name: Trie_tree.cpp
	> Author: TengyueMa
	> Mail: lumujin@icloud.com
	> Created Time: 一  1/29 14:17:08 2018
 ************************************************************************/

#include <iostream>
#include "utils.h"
#include "Trie_tree.h"
#include "CrfFeatErrorCode.h"
#include "SluLog.h"
using namespace std;
using namespace utils;

/*
 * @brief 节点类
 */

//< 构造函数
TrieNode::TrieNode(){
    flag = false;
}

//< 析构函数
TrieNode::~TrieNode(){
    for (unordered_map<string, TrieNode*>::iterator iter = children.begin(); iter != children.end(); iter++){
        delete iter->second;
    }
}

/*
 * @brief 字典树
 */

//< 构造函数
TrieTree::TrieTree(const string filepath){
    //< 如果路径为空则root = NULL
    if (filepath == ""){
        root = NULL;
    }
    //< load 词典
    else{
        root = NULL;
        load_dict(filepath);
    }
}

//< 批量load不同位置的词典到同一棵树中
TrieTree::TrieTree(const vector<string> filepaths){
    //< 如果0条路径
    if (filepaths.size() <= 0){
        root = NULL;
    }
    else {
        root = NULL;
        //< flag 标志着是否存在一个非空路径
        bool flag = false;
        for (size_t i = 0; i < filepaths.size(); i++){
            if (filepaths[i] != ""){
                flag = true;
                load_dict(filepaths[i]);
            }
        }

        //< 如果所有路径都为空root = NULL
        if (!flag){
            root = NULL;
        }
    }
}

//< 析构函数
TrieTree::~TrieTree(){
    delete root; 
}

//< load dict 可以重复load
int TrieTree::load_dict(const string filepath){
    ifstream dict;
    //< 打开文件
    dict.open(filepath);

    if (!dict){
        loginstance->error("[SLU]: failed to open dict file: " + filepath );
        return ERR_TRIE_TREE_OPEN_DICT_FILE;
    }

    string line;
    while(getline(dict, line)){
        insert(line);
    }

    //< 关闭文件
    dict.close();

    loginstance->info("[SLU]: load dict success! dict name: " + filepath);

    return 0;
}

//< 插入数据到Tree
int TrieTree::insert(const string seq){
    if (root == NULL){
        root = new TrieNode();
    }

    TrieNode *ptr = root;
    //< 单字切分 utf8
    strSplit2simple(seq, cha_vec);
    
    size_t count = cha_vec.size();
    for (size_t i = 0; i < count; i++){
        if (ptr->children.find(cha_vec[i]) == ptr->children.end()){
            ptr->children[cha_vec[i]] = new TrieNode();
        }
        ptr = ptr->children[cha_vec[i]];
    }

    ptr->flag = true;

    return 0;
}

//< 查询是否存在该路径
bool TrieTree::has_dict(const string seq){
    if (root == NULL || seq.size() == 0){
        return false;
    }

    TrieNode* ptr = root;
    //< 单字切分
    strSplit2simple(seq, cha_vec);

    size_t count = cha_vec.size();
    size_t i;
    for (i = 0; i < count; i++){
        if (ptr->children.find(cha_vec[i]) == ptr->children.end()){
            break;
        }
        ptr = ptr->children[cha_vec[i]];
    }

    if (i < count || !ptr->flag){
        return false;
    }

    return true;
}

//< 最大匹配
int TrieTree::match(const vector<string>& seq_vec, 
                    int idx){
    if (root == NULL || seq_vec.size() == 0){
        return 0;
    }

    TrieNode* ptr = root;

    size_t count = seq_vec.size();
    int len = 0;
    int n = 0;

    for (size_t i = idx; i < count; i++){
        if (ptr->children.find(seq_vec[i]) == ptr->children.end()){
            break;
        }
        else {
            n++;
            ptr = ptr->children[seq_vec[i]];
            if (ptr->flag){
                len += n;
                n = 0;
            }
        }
    }

    return len;
}

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
