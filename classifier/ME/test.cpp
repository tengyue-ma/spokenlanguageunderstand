/*************************************************************************
	> File Name: test.cpp
	> Author: TengyueMa
	> Mail: lumujin@icloud.com
	> Created Time: 二  1/23 12:26:48 2018
 ************************************************************************/

#include <iostream>
#include <dlfcn.h>
#include <unistd.h>
#include <stdlib.h>
#include <fstream>
#include <cstring>
#include <map>
#include <vector>
#include <string>
#include "Feature.h"
#include "FeatExtractor.h"
#include "PreProcessor.h"
#include "Classifier.h"
using namespace std;
using namespace feat_extractor;
using namespace classify;

#define ERR_PARSE_ARGS -10000
#define ERR_CHECK_ARGS -10001

ifstream inf;
ofstream outf;
ostream* out = &cout;

//> 文件循环的次数
int cycle_num = 0;

//> args, 输入参数
//> 以下四个参数暂不支持
int thread_num = 1;
int slu_num = 1;
int n_best = 1;
bool cycle_test = 0;

void print_help(const char* program_name){
    cout << "This is a slu engine." << endl << endl;

    cout << "Usage:" << endl;
    cout << "\t" << program_name << " [options]" << endl;
    cout << "Options:" << endl;
    cout << "\t-s" << "\tsentence to be parsed by engine.            \t\toptional" << endl;
    cout << "\t-f" << "\tfile to be parsed by engine.                \t\toptional" << endl;
    cout << "\t-o" << "\toutput parsing reuslt to file.              \t\toptional" << endl;
    //cout << "\t-p" << "\tthread number, positive integer, default: 1.\t\toptional" << endl;
    //cout << "\t-u" << "\tslu number, positive integer,    default: 1.\t\toptional" << endl;
    //cout << "\t-n" << "\tn-best number, positive integer, default: 1.\t\toptional" << endl;
    cout << "\t-y" << "\tcycle test or not, bool, 1 or 0, default: 0.\t\toptional" << endl;
    cout << "\t-h" << "\tprint this help message.                    \t\toptional" << endl;

    cout << "Example:" << endl;
    cout << "\t" << program_name << " -f query.txt -o result.txt" << endl;
    cout << "\t" << program_name << " -n 5 -f query.txt -o result.txt" << endl;
    cout << "\t" << program_name << " -s 我想听青花瓷" << endl;
    cout << "\t" << program_name << " -n 5 -s 唱一首薛之谦的演员吧" << endl;
    cout << "\t" << program_name << " -p 15 -u 5 -f query.txt -o result.txt" << endl;
    cout << "\t" << program_name << " -p 15 -u 5 -n 5 -y 1 -f query.txt -o result.txt" << endl;

    cout << "Note:" << endl;
    cout << "\t1). -s and -f cannot be given(or not given) simultaneously." << endl;
    cout << "\t2). if -o not given, results will be printed to console." << endl;
    cout << endl;
}

int parse_args(int argc, const char** argv, map<char, string>& args){
    const int WAIT_OPT=0, WAIT_ARG=1;
    int state = WAIT_OPT;
    char pre_arg_char = ' ';
    for(int i=1; i<argc; i++){
        // maybe an option
        if(strlen(argv[i])==2 && argv[i][0]=='-'){
            if(WAIT_ARG == state)
                return ERR_PARSE_ARGS;
            if(argv[i][1]=='s' || argv[i][1]=='f' || argv[i][1]=='o' || argv[i][1]=='p' || argv[i][1]=='u' || argv[i][1]=='n'|| argv[i][1]=='y' ){
                pre_arg_char = argv[i][1];
                state = WAIT_ARG;
            }else if(argv[i][1] == 'h'){
                pre_arg_char = argv[i][1];
            }else
                return ERR_PARSE_ARGS;
            // expect an argument
        }else{
            if(argv[i][0] == '-')
                return ERR_PARSE_ARGS;
            else if(WAIT_OPT == state)
                return ERR_PARSE_ARGS;
            else
                args[pre_arg_char] = argv[i];
            state = WAIT_OPT;
        }
    }
    return 0;
}

int check_args(const char* program_name, map<char, string> args){

    if(args.find('h') != args.end()){
        print_help(program_name);
        return 0;
    }

    //必须有的参数: s or f
    if(args.find('s') == args.end()){
        if(args.find('f') == args.end()){
            print_help(program_name);
            cout << ">>>>>> ERROR: args -s and -f are all unexist !"<< endl;
            return ERR_CHECK_ARGS;
        }
    }else{
        if(args.find('f') != args.end()){
            print_help(program_name);
            cout << ">>>>>> ERROR: args -s and -f are all exist !"<< endl;
            return ERR_CHECK_ARGS;
        }
    }

    //< 解析n_best数量、线程数量、slu数量, 如果参数列表未设置, 默认值都为1
    if(args.find('n') != args.end()){
        n_best = atoi(args['n'].c_str());
        if(n_best < 1) {
            print_help(program_name);
            cout << ">>>>>> ERROR: args -n wrong! n_best: " << n_best << endl;
            return ERR_CHECK_ARGS;
        }
    }

    if(args.find('p') != args.end()){
        thread_num = atoi(args['p'].c_str());
        if(thread_num <= 0){
            print_help(program_name);
            cout << ">>>>>> ERROR: args -p wrong! thread_num: "<< thread_num << endl;
            return ERR_CHECK_ARGS;
        }
    }

    if(args.find('u') != args.end()){
        slu_num = atoi(args['u'].c_str());
        if(slu_num <= 0){
            print_help(program_name);
            cout << ">>>>>> ERROR: args -u wrong! slu_num: "<< slu_num << endl;
            return ERR_CHECK_ARGS;
        }
    }

    //< 是否开启无限循环测试, 默认值为0
    if(args.find('y') != args.end()){
        cycle_test = atoi(args['y'].c_str());
        if(cycle_test != 0 && cycle_test !=1){
            print_help(program_name);
            cout << ">>>>>> ERROR: args -y wrong! cycle_test: "<< cycle_test << endl;
            return ERR_CHECK_ARGS;
        }
    }

    //< 输出文件
    if(args.find('o') != args.end()){
        outf.open(args['o'].c_str());
        if(!outf.is_open()){
            cout << ">>>>>> ERROR: Cannot open " << args['o'] << " for writing." << endl;
            return ERR_CHECK_ARGS;
        }
        //如果没有定义输出文件, 则打印到屏幕上
        out = dynamic_cast<ostream*>(&outf);
    }

    cout<< ">>>>>> cycle_test: " << cycle_test <<endl;

    return 0;
}

int print_result(string query, vector<pair<string, float> > class_name){

    *out << query << "\t" << class_name[0].first << "\t" << class_name[0].second << endl;

    return 0;
}

int main(int argc, const char** argv){
    int ret = 0;
    map<char, string> args;

    //< 解析输入参数,保存在args
    ret = parse_args(argc, argv, args);
    if (ret < 0) {
        print_help(argv[0]);
        fprintf(stderr,"parse args failed, error code:%d\n", ret);
        return ret;
    }

    //< 检查并设置参数值
    ret = check_args(argv[0], args);
    if (ret < 0) {
        fprintf(stderr,"check args failed, error code:%d\n", ret);
        return ret;
    }

    cout << ">>>>>> 正在加载配置……" << endl;
    //< 1. 初始化ltp
    PreProcessor* pre = new PreProcessor();
    pre->create_wsg_engine("../../data/model/ltp_model/cws.model");
    pre->create_pos_engine("../../data/model/ltp_model/pos.model");
    pre->create_ner_engine("../../data/model/ltp_model/ner.model");
    pre->create_pas_engine("../../data/model/ltp_model/parser.model");

    //< 2. 指定特征提取库, me的库, 配置, 这个没有写在argv参数里
    string feat_so = "../../feat_extractor/me_feat/lib/libmefeat.so";
    string me_so = "lib/libme.so";
    string conf = "feat.conf";
    string log_conf = "log_path.conf";

    //< 3. 加载特征提取配置文件
    typedef FeatExtractor* (*CREATE_FEAT_EXTR)(const string, PreProcessor*);
    void *handle = dlopen(feat_so.c_str(), RTLD_NOW);
    if (!handle){
        cout << dlerror() << endl;
        exit(-1);
    }
    CREATE_FEAT_EXTR create_feat_extr = reinterpret_cast<CREATE_FEAT_EXTR>(dlsym(handle, "create_feat_extr"));
    FeatExtractor* feat_ext = create_feat_extr(conf, pre);
    if (feat_ext == NULL){
        cout << ">>>>>> ERROR: feat_ext error." << endl;
        exit(-1);
    }

    //< 4. 加载分类模型
    typedef Classifier* (*CREATE_CLASSIFIER)(const string, const string);
    void *handle_clf = dlopen(me_so.c_str(), RTLD_NOW);
    if (!handle_clf){
        cout << dlerror() << endl;
        exit(-1);
    }
    CREATE_CLASSIFIER create_classifier = reinterpret_cast<CREATE_CLASSIFIER>(dlsym(handle_clf, "create_classifier"));
    Classifier* clf = create_classifier("../../data/model/clf_model/play_other_classifier.txt", log_conf);
    if (clf == NULL){
        cout << ">>>>>> ERROR: load clf model error." << endl;
        exit(-1);
    }
    cout << ">>>>>> 配置加载完毕" << endl;

    //< 5. 进行分类
    //< 如果输入是文件, 需要循环处理
    if(args.find('f') != args.end()) {
        inf.open(args['f']);
        if(!inf){
            fprintf(stderr,">>>>>> ERROR: Cannot open input file!\n");
            return -1;
        }
        cout << ">>>>>> 正在解析文件: " << args['f'] << ", 请等待……" << endl;

        string line;
        while (true) {
            getline(inf, line);
            //> 读到文件结尾
            if (inf.eof()) {
                cycle_num++;
                if(cycle_test) {
                    inf.clear();
                    inf.seekg(0); //回到文件头,用于循环测试
                    cout<< ">>>>>> The end of the file, continue cycle test. cycle_num: " << cycle_num <<endl;
                    continue;
                }
                else{
                    cout<< ">>>>>> The end of the file, stop." <<endl;
                    break;
                }
            }

            string query = line;
            vector<pair<string, float> > class_name;
            ret = clf->classify(query, class_name, feat_ext);
            if (ret < 0) {
                cout<< ">>>>>> ERROR: clf failed" <<endl;
                cout<< ">>>>>> ERROR: error code: " << ret <<endl;
            }
            print_result(query, class_name);
        }
        inf.close();
    }
    //< 如果输入是句子, 只处理一次
    else{
        string query = args['s'];
        vector<pair<string, float> > class_name;
        ret = clf->classify(query, class_name, feat_ext);
        if (ret < 0) {
            cout<< ">>>>>> ERROR: clf failed" <<endl;
            cout<< ">>>>>> ERROR: error code: " << ret <<endl;
        }
        print_result(query, class_name);
    }


    //< 6. delete
    typedef FeatExtractor* (*DELETE_FEAT_EXTR)(FeatExtractor *);
    DELETE_FEAT_EXTR delete_feat_extr = reinterpret_cast<DELETE_FEAT_EXTR>(dlsym(handle, "delete_feat_extr"));
    delete_feat_extr(feat_ext);

    typedef Classifier* (*DELETE_CLASSIFIER)(Classifier *);
    DELETE_CLASSIFIER delete_classifier = reinterpret_cast<DELETE_CLASSIFIER>(dlsym(handle_clf, "delete_classifier"));
    delete_classifier(clf);

    dlclose(handle);
    dlclose(handle_clf);

    return 0;
}

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
