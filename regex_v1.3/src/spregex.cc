#include <iostream>
#include <cstring>
#include <fstream>
#include <stdexcept>
#include <unordered_map>

#include "spregex.h"
#include "utils.h"
#include "cJSON.h"
#define MAXLEN 1024
# include <vector>
#include "thread_pool.h"
using namespace progschj;

using std::vector;
using std::cout;
using std::endl;
using std::ifstream;
using std::unordered_map;

namespace SemanticRegex{

/*
 * 从配置文件中读取所有的领域,及其规则文件所在目录;
 * 并为每个领域的每条正则表达式创建一个nfa
 */
Regex::Regex(const string& cfg_file, hobotlog::LogModule *loginstance){
	wrd_max_len = 0;
	//< 设置log
	this->loginstance = loginstance;

	//< 读取配置文件
	loginstance->debug("[SLU]: reading config file ...");
	int ret = load_config(cfg_file);
	if(ret != 0)
		throw std::runtime_error("Mistakes found in onfiguration file!");

	//< 连接redis
	loginstance->debug("[SLU]: connecting redis ...");
	redisContext *con = redisConnect(redis_server.c_str(), redis_port);
	if(con!=NULL && con->err){
		string msg = "redis connection error: " + con->err;
		throw std::runtime_error(msg.c_str());
	}
    redisReply *reply;
    string cmd = "AUTH " + redis_pswd;
    reply = (redisReply*)redisCommand(con, cmd.c_str());
    if(reply == NULL){
        throw std::runtime_error("failed to authenticate redis, please check your password.");
    }
    freeReplyObject(reply);
	redis_wrapper = new RedisWrapper(con);

	//< 创建两个iconv_t用于编码转换
	cd_g2u = iconv_open("UTF-8", "GBK");
	if(cd_g2u == (iconv_t)-1){
		throw std::runtime_error("Cannot open iconv!");
	}
	cd_u2g = iconv_open("GBK", "UTF-8");
	if(cd_u2g == (iconv_t)-1){
		throw std::runtime_error("Cannot open iconv!");
	}
}

Regex::~Regex(){
	for(map<string, DomainLex*>::iterator iter=domain_lex.begin();
		iter!=domain_lex.end(); iter++){
		delete iter->second;
	}
	iconv_close(cd_g2u);
	iconv_close(cd_u2g);
	//< 释放跟数据库的连接
	delete redis_wrapper;
}

/*
 * 本函数完成两件事情:
 *	1. 读取redis ip以及端口port
 *	2. 读取多个领域的配置列表,各领域含:
 *		1). 领域名
 *		2). 领域规则所在目录
 */
int Regex::load_config(const string& cfg_file){
	//< 获取cfg_file的所在目录
	string cfg_dir = "./";
	size_t idx = cfg_file.rfind('/');
	if(idx != string::npos){
		cfg_dir = cfg_file.substr(0, idx+1);
	}

	//< 读取config文件
	ifstream inf(cfg_file.c_str());
	VERIFY(inf, "Failed to open config file!");
	string json_str = "";
	string line;
	while(getline(inf, line)){
		json_str += line;
	}

	cJSON* root = cJSON_Parse(json_str.c_str());
	VERIFY(root, "config json format error!");

	//< 提取redis的配置信息
	cJSON *predis 	   = cJSON_GetObjectItem(root, "redis");
	VERIFY(predis, "config json format error!");

	cJSON *predis_ip   = cJSON_GetObjectItem(predis, "server");
	VERIFY(predis_ip, "config json format error!");

	cJSON *predis_port = cJSON_GetObjectItem(predis, "port");
	VERIFY(predis_port, "config json format error!");

	cJSON *predis_pswd = cJSON_GetObjectItem(predis, "password");
	VERIFY(predis_pswd, "config json format error!");

	redis_server = predis_ip->valuestring;
	redis_port = predis_port->valueint;
	redis_pswd = predis_pswd->valuestring;

	//< 提取领域列表
	cJSON *domains = cJSON_GetObjectItem(root, "domains");
	VERIFY(domains, "config json format error!");
	size_t n_domain = cJSON_GetArraySize(domains);

    // 领域层级的多线程加载
    ThreadPool pool(6);
    vector< std::future<DomainLex*> > results;
    vector<string> names;

	for(size_t i=0; i<n_domain; i++){
		cJSON *domain = cJSON_GetArrayItem(domains, i);
		VERIFY(domain, "config json format error!");
		cJSON *pname = cJSON_GetObjectItem(domain, "name");
		cJSON *plex_dir = cJSON_GetObjectItem(domain, "lex_dir");
		VERIFY(pname, "config json format error!");
		VERIFY(plex_dir, "config json format error!");
		string name = pname->valuestring;
		string lex_dir = plex_dir->valuestring;
		if(find(names.begin(), names.end(), name) != names.end()){
			std::cerr << "Warning: domain " << name << " redeclared!" << endl;
			delete domain_lex[name];
		}
        names.push_back(name);
		loginstance->debug("[SLU]: reading lex for " + name + " ...");
        results.emplace_back(
            pool.enqueue([name, lex_dir] {
                DomainLex* domain_lex = new DomainLex(name, lex_dir);
                return domain_lex;
            })
    
        );
	}
    pool.wait_until_empty();

	for(size_t i=0; i<n_domain; i++){
		string name = names[i];
		domain_lex[name] = results[i].get();
		if(wrd_max_len < domain_lex[name]->wrd_max_len)
			wrd_max_len = domain_lex[name]->wrd_max_len;
	}
	return 0;
}

/*
 * 利用快速排序算法,对若干个语义结果按照权重从大到小排序
 *	sem_vec:	多个语义结果
 *	start:		快排算法的起始位置
 *	end:		快排算法的结束位置
 */
void quick_sort(vector<Semantic*>& sem_vec, int start, int end){
	if(start >= end)
		return;
	Semantic* tmp = sem_vec[start];
	int i=start, j=end;
	while(i<j){
		while(i<j && sem_vec[j]->weight < tmp->weight) j--;
		if(i<j){
			sem_vec[i] = sem_vec[j];
			i++;
		}
		while(i<j && sem_vec[i]->weight > tmp->weight) i++;
		if(i<j){
			sem_vec[j] = sem_vec[i];
			j--;
		}
	}
	sem_vec[i] = tmp;
	quick_sort(sem_vec, start, i-1);
	quick_sort(sem_vec, i+1, end);
}

/*
注释原因: 没有调用的地方, 并且是私有函数, 单元测试测不到
时间: 2018.12.18
by leilei.zhou
*/

//int Regex::gbk_sem(Semantic* sem){
//	int ret = 0;
//	string tmp_str;
//
//	ret = utf82gbk(sem->query, tmp_str);
//	if(ret<0) return ret;
//	sem->query  = tmp_str;
//
//	ret = utf82gbk(sem->domain, tmp_str);
//	if(ret<0) return ret;
//	sem->domain  = tmp_str;
//
//	ret = utf82gbk(sem->intent, tmp_str);
//	if(ret<0) return ret;
//	sem->intent  = tmp_str;
//
//	for(size_t i=0; i<sem->slots.size(); i++){
//		ret = utf82gbk(sem->slots[i]->name, tmp_str);
//		if(ret<0) continue;
//		sem->slots[i]->name = tmp_str;
//
//		ret = utf82gbk(sem->slots[i]->value, tmp_str);
//		if(ret<0) continue;
//		sem->slots[i]->value = tmp_str;
//	}
//
//	return 0;
//}

/*
 * 按以下步骤进行:
 *	1. 抽取出所有可能的词,例如对于句子"我要听青花瓷",若wrd_max_len=2,其所有可能的词为
 *		["我", "我要", "要", "要听", ..., "花", "花瓷", "瓷"]
 *	2. 调用所有领域对应的DomainLex进行匹配
 *	3. 汇总所有领域的匹配结果,按照权重从大到小的原则进行排序,选择权重最大的前n条
 */
vector<Semantic*> Regex::match(const string& gbk_sent,
			const string& use_domain, int n_best, bool gbk){
	vector<Semantic*> sem_vec;
	//< 若为gbk格式,先转换为utf-8格式
	string sentence;
	if(gbk){
		int ret = gbk2utf8(gbk_sent, sentence);
		if(ret<0) return sem_vec;
	}else{
		sentence = gbk_sent;
	}

	//< 按utf-8字符切分成string并放到vector中
	vector<string> chars;
	Utils::utf8strs(sentence.c_str(), chars);

	//< 抽取出所有可能的词
	unordered_map< string, vector<size_t> > substr_idx_map;
	for(size_t i=0; i<chars.size(); i++){
		for(size_t j=1; j<=wrd_max_len; j++){
			string tmp = Utils::join(chars, i, i+j);
			if(substr_idx_map.find(tmp) == substr_idx_map.end())
				substr_idx_map[tmp] = vector<size_t>();
			substr_idx_map[tmp].push_back(i+j);
		}
	}

    map<string, int> redis_key_val;
    string matched_string = "";
	//< 若未指定领域，则全部领域都匹配一遍
	if(use_domain == "" || use_domain == "subnav"){
		// loginstance->debug("match domain not specified, match in all domains.");
		for(map<string, DomainLex*>::iterator iter=domain_lex.begin();
			iter!=domain_lex.end(); iter++){
			if(use_domain == "" && iter->first == "subnav") continue;
			// loginstance->debug("matching in domain " + iter->first + ".");
			vector<Semantic*> dom_sem_vec;
			if(iter->first == "vehicle_control" && chars.size() <= 22)
				dom_sem_vec = iter->second->split_match(sentence,
												chars, &redis_key_val, matched_string,
												redis_wrapper, substr_idx_map, loginstance);
			else
				dom_sem_vec = iter->second->match(sentence,
												chars, &redis_key_val, matched_string,
												redis_wrapper, substr_idx_map, loginstance);
			for(size_t i=0; i<dom_sem_vec.size(); i++){
				// loginstance->debug(dom_sem_vec[i]->dumps());
				bool flag = true;
				for(size_t j=0; j<i; j++){
					//< 去除重复
					if(NULL != dom_sem_vec[j] && *(dom_sem_vec[i]) == *(dom_sem_vec[j])){
						flag = false;
						break;
					}
				}
				if(flag){
			    	sem_vec.push_back(dom_sem_vec[i]);
                }else{
					delete dom_sem_vec[i];
                    dom_sem_vec[i] = NULL;
                }
			}
		}
	//< 否则，只用指定领域下的规则进行匹配
	}else{
		// loginstance->debug("matching in specified domain [" + use_domain + "].");
		if(domain_lex.find(use_domain) == domain_lex.end()){
			// loginstance->error("domain [" + use_domain + "] not found in config file!");
			return sem_vec;
		}
		vector<Semantic*> dom_sem_vec = domain_lex[use_domain]->match(
		sentence, chars, &redis_key_val, matched_string, redis_wrapper, substr_idx_map, loginstance);
		for(size_t i=0; i<dom_sem_vec.size(); i++){
			// loginstance->debug(dom_sem_vec[i]->dumps());
			bool flag = true;
			for(size_t j=0; j<i; j++){
				//< 去除重复
		   		if(NULL != dom_sem_vec[j] && *(dom_sem_vec[i]) == *(dom_sem_vec[j])){
		   			flag = false;
					break;
				}
			}
			if(flag){
				sem_vec.push_back(dom_sem_vec[i]);
            }else{
				delete dom_sem_vec[i];
                dom_sem_vec[i] = NULL;
            }
		}
	}
	vector<Semantic*> nbest_sem_vec;
	int min =(n_best < sem_vec.size())? n_best:sem_vec.size();
	if(n_best == -1)
		min = sem_vec.size();
	quick_sort(sem_vec, 0, sem_vec.size()-1);

	for(int i=0; i<min; i++){
		nbest_sem_vec.push_back(sem_vec[i]);
	}

	//< 删除n-best以后的结果,释放空间
	for(int i=min; i<sem_vec.size(); i++)
		delete sem_vec[i];

	return nbest_sem_vec;
}

int Regex::gbk2utf8(const string& str_src, string& str_des){
	char src[MAXLEN];
	char des[MAXLEN];
	size_t len = str_src.size();
	memset(src, 0, MAXLEN);
	strcpy(src, str_src.c_str());
	memset(des, 0, MAXLEN);
	char *in  = src;
	char *out = des;
	size_t inlen = len;
	size_t oulen = MAXLEN;
	if(iconv(cd_g2u, &in, &inlen, &out, &oulen) == -1)
		return -1;
	str_des = des;
	return 0;
}

/*
注释原因: 没有调用的地方, 并且是私有函数, 单元测试测不到
时间: 2018.12.18
by leilei.zhou
*/
//int Regex::utf82gbk(const string& str_src, string& str_des){
//	char src[MAXLEN];
//	char des[MAXLEN];
//	size_t len = str_src.size();
//	memset(src, 0, MAXLEN);
//	strcpy(src, str_src.c_str());
//	memset(des, 0, MAXLEN);
//	char *in  = src;
//	char *out = des;
//	size_t inlen = len;
//	size_t oulen = MAXLEN;
//	if(iconv(cd_u2g, &in, &inlen, &out, &oulen) == -1)
//		return -1;
//	str_des = des;
//	return 0;
//}

vector<string> Regex::parse(const string& sentence,
							const string& use_domain,
							int n_best){
	vector<string> results;

	vector<Semantic*> sems = match(sentence, use_domain, n_best, false);
	for(size_t i=0; i<sems.size(); i++){
		results.push_back(sems[i]->dumps());
		delete sems[i];
	}
	sems.clear();
	return results;
}

}	// end of namespace SemanticRegex

