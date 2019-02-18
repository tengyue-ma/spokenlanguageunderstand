/*************************************************************************
	> File Name: SLU.h
	> Author: TengyueMa
	> Mail: lumujin@icloud.com
	> Created Time: 四  2/22 10:53:36 2018
 ************************************************************************/

#ifndef _SLU_H
#define _SLU_H

#include <map>
#include <dlfcn.h>
#include <unistd.h>
#include "SemanticInfo.h"
#include "PreProcessor.h"
#include "Feature.h"
#include "FeatExtractor.h"
#include "PreProcessor.h"
#include "Classifier.h"
#include "SlotFilling.h"
#include "WorkFlow.h"
#include "DomainManage.h"
#include "ul_log.h"
#include "Configure.h"
#include "spreg.h"
#include "utils.h"
#include "SluErrorCode.h"
//< 规则头文件
#include "spregex.h"
#include "semantic.h"

using std::vector;
using std::string;
using std::map;
using slot_filling::SlotFilling;
using classify::Classifier;
using feat_extractor::FeatExtractor;
using feature::Feature;
using SemanticRegex::Regex;
using SemanticRegex::Semantic;
using namespace utils;
using namespace std;

namespace slu{

class SLU{
public:
    /*
     * @brief 构造函数
     */
    SLU();

    /*
     * @brief 析构函数
     */
    ~SLU();

public:
    /*
     * @brief 创建各个模块并初始化
     * @param conf_file : 配置文件名称
     * @param conf_path : 路径
     */
    int init_slu(const string conf_file, 
                 const string conf_path = "./");

    /*
     * @brief 自定义workflow
     * @param workflow : 解析架构
     */ 
    int set_workflow(const WorkFlow& workflow);
    
    /*
     * @brief SLU解析接口
     * @param query : 待解析句子
     * @param sem_info : 解析结果对象
     * @param id : 相关id
     * @param workflow : 定义的解析架构
     */
    int semantic_interface(string query, 
                           SemanticInfo& sem_info, 
                           string id);
    int semantic_interface(string query, 
                           SemanticInfo& sem_info, 
                           string id, 
                           WorkFlow workflow);

private:
    /*
     * @brief get input
     * @param param : 获取参数的类型
     * @param sem_info : 待获取的信息对象
     */
    string get_input(const string param, 
                     SemanticInfo& sem_info);

    /*
     * @brief set output
     * @param param : 输出写入的目标
     * @param output : 输出的具体内容
     * @param sem_info : 输出的对象
     */
    int set_output(const string param, 
                   vector<string> output, 
                   SemanticInfo& sem_info);
    int set_output(const string param, 
                   vector<pair<string, float> > output, 
                   SemanticInfo& sem_info);
    //< 写入规则结果
    int set_output(vector<Semantic*>& sems, 
                   SemanticInfo& sem_info);
    
    /*
     * @brief 单步操作
     * @param workstep : 每个节点的函数操作定义
     * @param sem_info : 函数操作的目标对象
     * @param flag : 定义是否操作结束如regex有结果直接退出
     * @param ids : 各类id信息
     */
    int stepfunc(_workstep& workstep, 
                 SemanticInfo& sem_info, 
                 bool& flag, 
                 vector<string> ids);

    /*
     * @brief 特征提取
     * @param feat_ext : 特征提取器
     * @param query : 待处理的句子
     * @param feat : 特征提取结果
     * @param ids : 各类id信息
     */
    int get_feat(FeatExtractor* feat_ext, 
                 const string& query, 
                 Feature& feat, 
                 vector<string> ids);

    /*
     * @brief 分类
     * @param classifier : 分类器
     * @param feat_ext : 特征提取器
     * @param query : 待处理句子
     * @param class_res : 分类结果
     * @param ids : 各类id信息
     */
    int get_classes(Classifier* classifier, 
                    FeatExtractor* feat_ext, 
                    const string& query, 
                    vector<pair<string, float> >& class_res, 
                    vector<string> ids);

    /*
     * @brief NER
     * @param ner : 命名实体识别器
     * @param feat_ext : 特征提取器
     * @param query : 待处理句子
     * @param slots : 解析结果
     * @param ids : 各类id信息
     */
    int get_ner(SlotFilling* ner, 
                FeatExtractor* feat_ext, 
                const string& query, 
                vector<_slot>& slots, 
                vector<string> ids);

    /*
     * @brief regex
     * @param query : 待处理的句子
     * @param sems : 解析结果
     * @param ids : 各类id信息
     * @param domain : 限定领域 默认为空指代不限定领域
     * @param n_best : n-best个数 默认为-1指代不限定个数
     * @param is_gbk : 默认query编码 默认为false指代utf8编码
     */
    int get_regex(const string query, 
                  vector<Semantic*>& sems, 
                  vector<string> ids, 
                  const string domain = "", 
                  const int n_best = -1, 
                  const bool is_gbk = false);

    /*
     * @brief get input
     */
    string get_input(const string param, 
                     SemanticInfo& sem_info,
                     const string domain);

    /*
     * @brief set output
     */
    int set_output_ner(const string param, 
                       vector<_slot> output, 
                       SemanticInfo& sem_info, 
                       vector<string> ids);
    int set_output_clf(const string param, 
                       vector<pair<string, float> > output, 
                       SemanticInfo& sem_info, 
                       vector<string> ids, 
                       const string domain = "");
private:
    //< @brief 自定义函数节点
    /*
     * @brief 领域管理
     * @param workstep : 节点操作定义
     * @param sem_info : 操作对象
     * @param ids : 相关id
     */ 
    int domain_manager(_workstep& workstep,
                       SemanticInfo& sem_info,
                       vector<string> ids);

    /*
     * @brief 通用规则引擎 REGEX_v1.5
     * @param workstep : 节点操作定义
     * @param sem_info : 操作对象
     * @param flag : 定义regex解析结果是否为空 是否直接退出flow
     * @param ids : 相关id
     */
    int regex_global(_workstep& workstep, 
                     SemanticInfo& sem_info, 
                     bool& flag, 
                     vector<string> ids);

    /*
     * @brief 指定模型&特征提取器的NER
     * @param workstep : 节点操作定义
     *        (其中参数1为输入, 参数2为识别器name, 参数3为特征提取器name, 
     *        输出到workstep.output定义的位置)
     * @param sem_info : 操作对象
     * @param ids : 相关id
     */
    int ner_fixed(_workstep& workstep,
                  SemanticInfo& sem_info,
                  vector<string> ids);

    /*
     * @brief 默认对sem_info中已有的领域做NER解析
     *        (其中识别器和特征提取器的名字均为各个领域的名字,
     *        默认输出到各个领域下的slots中)
     * @param workstep : 节点操作定义
     * @param sem_info : 操作对象
     * @param ids : 相关id
     */
    int ner_every_domain(_workstep& workstep, 
                         SemanticInfo& sem_info, 
                         vector<string> ids);

    /*
     * @brief 指定模型&特征提取器的CLF
     *        (其中参数1为输入, 参数2为分类器name, 参数3为特征提取器name,
     *        输出到workstep.output定义的位置)
     * @param workstep : 节点操作定义
     * @param sem_info : 操作对象
     * @param ids : 相关id
     */
    int clf_fixed(_workstep& workstep,
                  SemanticInfo& sem_info,
                  vector<string> ids);

    /*
     * @brief 默认对sem_info中已有的领域做分类 最常用于intent分类
     *        与ner_every_domain类似
     * @param workstep : 节点操作定义
     * @param sem_info : 操作对象
     * @param ids : 相关id
     */ 
    int clf_every_domain(_workstep& workstep, 
                         SemanticInfo& sem_info,
                         vector<string> ids);

    /*
     * @brief 对sem_info中指定的领域信息做分类 最常用于play再分类
     * @param workstep : 节点操作定义
     * @param sem_info : 操作对象
     * @param ids : 相关id
     */
    int clf_if_domain(_workstep& workstep, 
                      SemanticInfo& sem_info,
                      vector<string> ids);

private:
    //< ltp接口
    PreProcessor* pre;
    //< 特征提取器
    map<string, FeatExtractor *> feat_ext_map;
    //< 分类器
    map<string, Classifier *> classifier_map;
    //< 命名实体识别器
    map<string, SlotFilling *> ner_map;
    //< 规则引擎
    Regex* parser;
    //< 领域管理
    DomainManage* domain_manage;

    //< handles
    map<string, void *> handle_feat_map;
    map<string, void *> handle_classifier_map;
    map<string, void *> handle_ner_map;
public:
    //< workflow
    WorkFlow workflow;
};

} //< namespace slu;

#endif
/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
