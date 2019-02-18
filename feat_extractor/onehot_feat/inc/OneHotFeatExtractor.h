/*************************************************************************
	> File Name: OneHotFeatExtractor.h
	> Author: TengyueMa
	> Mail: lumujin@icloud.com
	> Created Time: Thu 08 Nov 2018 11:10:56 AM CST
 ************************************************************************/

#ifndef _ONEHOTFEATEXTRACTOR_H
#define _ONEHOTFEATEXTRACTOR_H

#include <string>
#include <vector>
#include <map>
#include "Feature.h"
#include "PreProcessor.h"
#include "FeatExtractor.h"

using std::vector;
using std::string;
using std::map;
using namespace feature;

namespace feat_extractor{

class OneHotFeatExtractor:public FeatExtractor{
public:
    /*
     * @brief 构造函数
     * @param confpath : 配置文件路径
     * @param pre : ltp接口
     */
    OneHotFeatExtractor(const string confpath, 
                        PreProcessor* pre);

    /*
     * @brief 析构函数
     */
    ~OneHotFeatExtractor();

public:
    /*
     * @brief get feat
     * @param query : 用户string query 
     * @param Feature feats
     *
     * @input:
     *  周杰伦的青花瓷
     * @output:
     *  (假设词表大小为10, 长度为10)
     *  PAD : 1 0 0 0 0 0 0 0 0 0
     *  PAD : 1 0 0 0 0 0 0 0 0 0
     *  PAD : 1 0 0 0 0 0 0 0 0 0
     *  周  : 0 0 0 0 0 0 0 0 1 0 
     *  杰  : 0 0 1 0 0 0 0 0 0 0
     *  伦  : 0 1 0 0 0 0 0 0 0 0
     *  的  : 0 0 0 0 0 1 0 0 0 0
     *  青  : 0 0 0 0 0 0 0 1 0 0
     *  花  : 0 0 0 0 1 0 0 0 0 0
     *  瓷  : 0 0 0 0 0 0 0 0 0 1
     *
     *  @action1:
     *      query切分(分词/单字)
     *  @action2:
     *      切分后结果映射
     *  @action3:
     *      按照固定格式写入到feats里
     */
    int get_feat(feature::Feature& feats, 
                 const string query);
     
    /*
     * @brief 初始化conf
     * @param confpath : 配置文件路径
     */
    int init_conf(const string confpath, 
                  PreProcessor* pre);

    /* 
     * @brief 初始化ltp
     */ 
    int init_ltp(PreProcessor* pre);

private:
    /*
     * @brief 分词
     * @param query : 待分词句子
     * @param ws_vec : 分词结果
     * @input : 
     *      周杰伦的青花瓷
     * @output :
     *      周杰伦 的 青花瓷
     *
     * @input :
     *      放一首[artist]的[title]
     * @output :
     *      放 一首 [artist] 的 [title]
     */
    int get_wsg(string query, 
                vector<string>& ws_vec);

    /*
     * @brief 单字切分
     * @param query : 待切分句子
     * @param sip_vec : 切分结果
     * @input :
     *      周杰伦的青花瓷
     * @output :
     *      周 杰 伦 的 青 花 瓷
     *
     * @input :
     *      [artist]的[title]
     * @output :
     *      [artist] 的 [title]
     */
    int get_sip(string query, 
                vector<string>& sip_vec);

    /*
     * @brief get query
     * @input  : [周杰伦:artist]的[青花瓷:title]
     * @output : 周杰伦的青花瓷
     */
    string get_query(string query);

    /*
     * @brief get pattern
     * @input  : [周杰伦:artist]的[青花瓷:title]
     * @output : [artist]的[title]
     */
    vector<string> get_pattern(string query);

    /*
     * @brief pad_seq
     * @input :
     *      周 杰 伦 的 青 花 瓷
     * @output :
     *      <PAD> <PAD> <PAD> 周 杰 伦 的 青 花 瓷
     */
    int pad_seq(vector<string>& res, 
                vector<string> ws_vec);

    /*
     * @brief onehot 映射
     * @param word : 输入的词
     * @return 返回 0 1 0 0 0 一维string
     */
    string onehot_mapping(string word);

    /*
     * @brief 词典读取
     */
    int load_dict(map<string, int>& word_map, const string dict_path);


public:
    //< 特征提取类型
    string ext_type;

private:
    //< ltp
    //< 分词&词性标注&命名实体识别&句法分析
    PreProcessor* pre; 

    /*
     * @param use_q2feat : 使用query特征提取
     * @param use_p2feat : 使用pattern特征提取
     * @param word_map : 词表
     * @param seq_size : 句子长度
     * @param use_wsg : 是否分词(默认单字)
     * @param default_pad : 默认的pad
     * @param default_unk : 默认的unk
     */
    bool use_q2feat;
    bool use_p2feat;
    bool use_wsg;
    map<string, int> word_map;
    int map_size;
    size_t seq_size;
    string default_pad;
    string default_unk;
    //< 下标／ 0 1 0 0 vector
    string tensor_type; 
};

} //< namespace feat_extractor;

#endif
/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
