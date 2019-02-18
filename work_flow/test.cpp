#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include <map>
#include <vector>
#include <string>
#include <pthread.h>
#include <sys/time.h>
#include <float.h>
#include <limits.h>
#include <iomanip>
#include "SLU.h"
#include "WorkFlow.h"
#include "SemanticInfo.h"
#include "Feature.h"
#include "FeatExtractor.h"
#include "PreProcessor.h"
#include "Classifier.h"
#include "SlotFilling.h"

#define ERR_PARSE_ARGS -10000
#define ERR_CHECK_ARGS -10001

using namespace std;
using namespace feat_extractor;
using namespace classify;
using namespace slot_filling;
using namespace slu;

vector<pthread_mutex_t> slu_locks;
pthread_mutex_t mutex_in;
pthread_mutex_t mutex_out;
pthread_mutex_t mutex_time;
ifstream inf;
ofstream outf;
ostream* out = &cout;
//> 文件循环的次数
int cycle_num = 0;
//> query总条数
int query_num = 0;
//> 最小耗时和最大耗时
float time_min = FLT_MAX;
float time_max = FLT_MIN;
//> 最小耗时和最大耗时的句子
string query_min;
string query_max;

//> args, 输入参数
string workflow_conf = "../data/conf/workflow.conf";
//> 线程数
int thread_num = 1;
//> 实例数
int slu_num = 1;
//> n_best结果数
int n_best = 1;
//> 是否无限循环测试
bool cycle_test = 0;


void print_help(const char* program_name){
    cout << "This is a slu engine." << endl << endl;

    cout << "Usage:" << endl;
    cout << "\t" << program_name << " [options]" << endl;
    cout << "Options:" << endl;
    cout << "\t-w" << "\tworkflow configure, default: ../data/conf/workflow.conf.   \t\toptional" << endl;
    cout << "\t-s" << "\tsentence to be parsed by engine.                           \t\toptional" << endl;
    cout << "\t-f" << "\tfile to be parsed by engine.                               \t\toptional" << endl;
    cout << "\t-o" << "\toutput parsing reuslt to file.                             \t\toptional" << endl;
    cout << "\t-p" << "\tthread number, positive integer, default: 1.               \t\toptional" << endl;
    cout << "\t-u" << "\tslu number, positive integer,    default: 1.               \t\toptional" << endl;
    cout << "\t-n" << "\tn-best number, positive integer, default: 1.               \t\toptional" << endl;
    cout << "\t-y" << "\tcycle test or not, bool, 1 or 0, default: 0.               \t\toptional" << endl;
    cout << "\t-h" << "\tprint this help message.                                   \t\toptional" << endl;

    cout << "Example:" << endl;
    cout << "\t" << program_name << " -f query.txt -o result.txt" << endl;
    cout << "\t" << program_name << " -s 我想听青花瓷" << endl;
    cout << "\t" << program_name << " -w ../data/conf/workflow_nn.conf -f query.txt -o result.txt" << endl;
    cout << "\t" << program_name << " -w ../data/conf/workflow.conf -f query.txt -o result.txt" << endl;
    cout << "\t" << program_name << " -w ../data/conf/workflow.conf -s 我想听青花瓷" << endl;
    cout << "\t" << program_name << " -w ../data/conf/workflow.conf -n 5 -f query.txt -o result.txt" << endl;
    cout << "\t" << program_name << " -w ../data/conf/workflow.conf -n 5 -s 唱一首薛之谦的演员吧" << endl;
    cout << "\t" << program_name << " -w ../data/conf/workflow.conf -p 15 -u 5 -f query.txt -o result.txt" << endl;
    cout << "\t" << program_name << " -w ../data/conf/workflow.conf -p 15 -u 5 -n 5 -y 1 -f query.txt -o result.txt" << endl;

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
            if(argv[i][1]=='w' || argv[i][1]=='s' || argv[i][1]=='f' || argv[i][1]=='o' || argv[i][1]=='p' || argv[i][1]=='u' || argv[i][1]=='n'|| argv[i][1]=='y' ){
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
    
    //< 解析workflow配置, 如果参数表未设置, 默认值为 "../data/conf/workflow.conf"
    if(args.find('w') != args.end()){
        workflow_conf = args['w'];
        if(workflow_conf.empty()) {
            print_help(program_name);
            cout << ">>>>>> ERROR: args -w wrong! " << endl;
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

    cout<< ">>>>>> thread_num: " << thread_num << ", slu_num: " << slu_num << ", n_best: " << n_best << ", cycle_test: " << cycle_test <<endl;

    return 0;
}

int print_result(string query, SemanticInfo sem_info, float match_time){
    string match_mode = sem_info.get_match_mode();
    vector<_form> forms = sem_info.get_forms();
	string global_ner_query = sem_info.get_ner_query("null");
    int cur_n_best = n_best;

    //更新n_best值
    if(forms.size() < n_best){
        cur_n_best = forms.size();
    }
    for (size_t i = 0; i < cur_n_best; i++){
        clf_info domain = forms[i].domain;
        string classes = domain.classes;
        string intent = forms[i].intent.classes;
        float score = domain.score;
        vector<_slot> slots = forms[i].slots;

        //输出格式：music	inform	喜羊羊与灰太狼主题曲怎么唱呢  program=喜羊羊与灰太狼,tag=主题曲	1.000000    regex    65.34ms
        //写文件,加锁
        pthread_mutex_lock(&mutex_out);
        *out << classes << "\t" << intent << "\t" << query << "\t";
		//*out << classes << "\t" << intent << "\t" << query << "\t" << global_ner_query << "\t" ;
        for(int j = 0; j < slots.size(); j++){
            if(j < slots.size()-1) {
                *out << slots[j].name << "=" << slots[j].value << ",";
            }
            else {
                *out << slots[j].name << "=" << slots[j].value;//最后一个slot后面不打印逗号
            }
        }
        if(slots.size()>0)
            *out << "\t";
        *out << score << "\t" << match_mode << "\t" << fixed << setprecision(2) << match_time*1000 << "ms";
        *out << endl;
        pthread_mutex_unlock(&mutex_out);
    }

    return 0;
}

void * test(void *slu_vec){
    vector<SLU*> slu = *((vector<SLU*> *)slu_vec);
    int ret = 0;
    string id = "aaa&bbb&ccc";
    string line;

    while(true){
        //> 读文件，加锁
        pthread_mutex_lock(&mutex_in);
        getline(inf, line);

        //> 读到文件结尾
        if(inf.eof()){
            cycle_num++;
            if(cycle_test){
                inf.clear();
                inf.seekg(0); //回到文件头,用于循环测试
                cout<< ">>>>>> The end of the file, continue cycle test. cycle_num: " << cycle_num <<endl;
                pthread_mutex_unlock(&mutex_in);
                continue;
            }
            else {
                cout<< ">>>>>> The end of the file, stop." <<endl;
                pthread_mutex_unlock(&mutex_in);
                break;
            }
        }
        pthread_mutex_unlock(&mutex_in);

        //> 获取空闲的slu
        srand(pthread_self() + time(NULL));
        int index = rand() % slu_num;

        SemanticInfo sem_info;
        string query = line;
        query.erase(remove(query.begin(),query.end(),'\n'), query.end()); //> 删掉所有的换行符

        //> 计时开始
        struct  timeval start;
        struct  timeval end;
        gettimeofday(&start,NULL);
        //> slu解析
        ret = slu[index]->semantic_interface(query, sem_info, id);
        if(ret < 0){
            fprintf(stderr,"query:%s\n", query.c_str());
            fprintf(stderr,"semantic interface failed, error code:%d\n", ret);
            //return ret;
        }
        //> 计时结束
        gettimeofday(&end,NULL);
        float match_time = ((end.tv_sec - start.tv_sec) * 1000.0 + (end.tv_usec - start.tv_usec) / 1000.0)/1000.0;
        //> 统计最大耗时和最小耗时
        pthread_mutex_lock(&mutex_time);
        if(match_time > time_max){
            time_max = match_time;
            query_max = query;
        }
        if(match_time < time_min){
            time_min = match_time;
            query_min = query;
        }
        pthread_mutex_unlock(&mutex_time);
        query_num++;

        ret = print_result(query, sem_info, match_time);
        if(ret < 0){
            fprintf(stderr,"print result failed, error code:%d\n", ret);
            //return ret;
        }
    }

}

int main(int argc, const char** argv) {
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

    //< 开始slu, slu的配置没有写到参数控制里面
    //1. load workflow conf
    cout << ">>>>>> 正在加载 " << "workflow配置" << " ……" << endl;
    struct  timeval start;
    struct  timeval end;
    gettimeofday(&start,NULL);
    WorkFlow workflow;
    ret = workflow.load_conf(workflow_conf);
    if(ret < 0){
        fprintf(stderr,"workflow load conf failed, error code:%d\n", ret);
        return ret;
    }

    //2. init SLU
    vector<SLU*> slu_vec;
    for (int i = 0; i < slu_num; ++i) {
        cout << ">>>>>> 正在加载 " << "slu_" << i << " ……" << endl;

        SLU* slu = new SLU();
        ret = slu->init_slu("../data/conf/resource.conf");
        if(ret < 0){
            fprintf(stderr,"slu init failed, error code:%d\n", ret);
            return ret;
        }

        //3.
        ret = slu->set_workflow(workflow);
        if(ret < 0){
            fprintf(stderr,"slu set workflow failed, error code:%d\n", ret);
            return ret;
        }

        slu_vec.push_back(slu);
        cout << ">>>>>> 加载完毕 " << "slu_" << i << endl;

        pthread_mutex_t mutex;
        slu_locks.push_back(mutex);
    }

    gettimeofday(&end,NULL);
    float load_time = ((end.tv_sec - start.tv_sec) * 1000.0 + (end.tv_usec - start.tv_usec) / 1000.0)/1000.0;
    cout << ">>>>>> 加载资源耗时: " << load_time << "s" << endl;

    //< 如果输入是文件, 需要多线程
    if(args.find('f') != args.end()){
        inf.open(args['f']);
        if(!inf){
            fprintf(stderr,">>>>>> ERROR: Cannot open input file!\n");
            return -1;
        }
        cout << ">>>>>> 正在解析文件: " << args['f'] << ", 请等待……" << endl;

        gettimeofday(&start,NULL);
        pthread_t tids[thread_num];
        for (int i = 0; i < thread_num; ++i){
            ret = pthread_create(&tids[i], NULL, test, (void *)&(slu_vec));
            if (ret != 0) {
                fprintf(stderr,"pthread_create error: error_code:%d\n", ret);
                return -1;
            }
        }

        void *status;
        for (int i = 0; i < thread_num; ++i){
            ret = pthread_join(tids[i], &status);
        }
        inf.close();

        gettimeofday(&end,NULL);
        float file_match_time = ((end.tv_sec - start.tv_sec) * 1000.0 + (end.tv_usec - start.tv_usec) / 1000.0)/1000.0;

        cout << ">>>>>> 解析总条数   : " << query_num << endl;
        cout << ">>>>>> 解析总耗时   : " << fixed << setprecision(2) << file_match_time << "s" << endl;
        cout << ">>>>>> 解析最小耗时 : " << fixed << setprecision(2) << time_min*1000 << "ms, query : " << query_min << endl;
        cout << ">>>>>> 解析最大耗时 : " << fixed << setprecision(2) << time_max*1000 << "ms, query : " << query_max << endl;
        cout << ">>>>>> 解析平均耗时 : " << fixed << setprecision(2) << file_match_time/query_num*1000 << "ms" << endl;
    }
    //< 如果输入是句子, 不需要多线程
    else{
        SemanticInfo sem_info;
        string query = args['s'];
        query.erase(remove(query.begin(),query.end(),'\n'), query.end());
        gettimeofday(&start,NULL);
        ret = slu_vec[0]->semantic_interface(query, sem_info, "aaa&bbb&ccc");
        if(ret < 0){
            fprintf(stderr,"semantic interface failed, error code:%d\n", ret);
            return ret;
        }
        gettimeofday(&end,NULL);
        float sentence_match_time = ((end.tv_sec - start.tv_sec) * 1000.0 + (end.tv_usec - start.tv_usec) / 1000.0)/1000.0;

        cout << ">>>>>> 解析结果: \n" <<  endl;
        ret = print_result(query, sem_info, sentence_match_time);
        if(ret < 0){
            fprintf(stderr,"print result failed, error code:%d\n", ret);
            return ret;
        }
        cout <<  endl;
    }

    if(args.find('o') != args.end())
        outf.close();

    return 0;
}

