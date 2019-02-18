#include <string>
#include <vector>
#include "ul_log.h"
#include "Configure.h"
#include "spreg.h"

using namespace std;

typedef struct _global_tagging_conf_t {
    string _tagging_name;
    string _tagging_model;
    vector<string> _feat_vec;
}global_tagging_conf_t;

typedef struct _classifier_conf_t {
    string _classifier_name;
    string _classifier_model;
    string _default_classifier;
    vector<string> _feat_vec;
}classifier_conf_t;

typedef struct _domain_conf_t {
    string _domain_name;
    string _tagging_model;
    vector<string> _feat_vec;
    string _subdomain_detect_model;
	string _intent_detect_model;
}domain_conf_t;

typedef struct _conf_data_t {
	//global tagging conf
    vector<_global_tagging_conf_t> _gt_conf;

    //classifier conf
    vector<classifier_conf_t> _classifier_conf;

    //domain conf
    map<string, domain_conf_t> _domain_conf;
} conf_data_t;

int main(){
	int ret = 0;
	const char* conf_path = "./";
	const char* conf_file = "my_test.conf";
	conf_data_t* p_gconf = new conf_data_t();
	
	// load conf
    comcfg::Configure config;
    ret = config.load(conf_path, conf_file);
    if (ret < 0) {
        printf("failed to load config file in %s/%s\n", conf_path, conf_file);
        return -1;
    }

	//load global tagging configure
    try {
        int global_tagging_size = static_cast<int>(config["GLOBAL_TAGGING"].size());//目前就一个
        for (int i=0; i<global_tagging_size; i++) {
            _global_tagging_conf_t gt_conf;
            gt_conf._tagging_name  = config["GLOBAL_TAGGING"][i]["TAGGING_NAME"].to_cstr();
            gt_conf._tagging_model = config["GLOBAL_TAGGING"][i]["TAGGING_MODEL"].to_cstr();
            for (int j = 0; j < (int)config["GLOBAL_TAGGING"][i]["FEATURE"].size(); j++) {
                gt_conf._feat_vec.push_back(config["GLOBAL_TAGGING"][i]["FEATURE"][j].to_cstr());
            }
            p_gconf->_gt_conf.push_back(gt_conf);
            printf("global_tagging_name: %s, feat_num: %d.\n", 
			    gt_conf._tagging_name.c_str(), gt_conf._feat_vec.size());
        }
    } catch(...) {
        printf("load global tagging configure failed.");
        return -1;
    }

	//load domain configure
    try {
        int domain_size = static_cast<int>(config["DOMAIN"].size());
        for (int i=0; i<domain_size; i++) {
            domain_conf_t domain_conf;
            domain_conf._domain_name       = config["DOMAIN"][i]["DOMAIN_NAME"].to_cstr();
            domain_conf._tagging_model     = config["DOMAIN"][i]["TAGGING_MODEL"].to_cstr();
            for (int j = 0; j < (int)config["DOMAIN"][i]["FEATURE"].size(); j++) {
                domain_conf._feat_vec.push_back(config["DOMAIN"][i]["FEATURE"][j].to_cstr());
            }
            domain_conf._subdomain_detect_model = config["DOMAIN"][i]["SUBDOMAIN_DETECT_MODEL"].to_cstr();
            p_gconf->_domain_conf[domain_conf._domain_name] = domain_conf;
            printf("domain name: %s, feat_num: %d, tagging model: %s.\n", 
			    domain_conf._domain_name.c_str(), domain_conf._feat_vec.size(), domain_conf._tagging_model.c_str());			
        }
    } catch(...) {
        printf("load domain configure failed.\n");
        return -1;
    }
	
	return 0;
}