#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include <map>
#include <vector>
#include <string>
#include <ctime>
#include <stdexcept>
#include <pthread.h>

#include "logger.h"
#include "comm_def.h"
#include "spregex.h"

using namespace std;

typedef struct _ParseArgs{
	SemanticRegex::Regex *parser;
	ostream* out;
	string use_domain;
	int n_best;
}ParseArgs;

typedef struct _Pack{
	ParseArgs *pargs;
	string query;
	int thread_id;
}Pack;

void* work(void *arg){
	Pack *pack 		= static_cast<Pack*>(arg);
	ParseArgs *parg = pack->pargs;
	SemanticRegex::Regex *parser = parg->parser;
	vector<string> sems = parser->parse(pack->query, parg->use_domain, parg->n_best);
	for(size_t i=0; i<sems.size(); i++)
		*(parg->out) << "[THREAD " << pack->thread_id << "]: " << sems[i] << endl;
	if(sems.size()<=0)
		*(parg->out) << "[THREAD " << pack->thread_id << "]: " << pack->query << endl;
}

void print_help(const char* program_name){
	cout << "This is a regular expression engine for semantic parsing." << endl << endl;

	cout << "Usage:" << endl;
	cout << "\t" << program_name << "-c config_file [options]" << endl;
	cout << "Options:" << endl;
	cout << "\t-c" << "\tconfiguration file, json format.\t\trequired" << endl;
	cout << "\t-s" << "\tsentence to be parsed by engine.\t\toptional" << endl;
	cout << "\t-f" << "\tfile to be parsed by engine.    \t\toptional" << endl;
	cout << "\t-o" << "\toutput parsing reuslt to file.  \t\toptional" << endl;
	cout << "\t-d" << "\tonly parse in this domain.      \t\toptional" << endl;
	cout << "\t-p" << "\tthread number, positive integer.\t\toptional" << endl;
	cout << "\t-n" << "\tn-best number, if not specified,\t\t        " << endl;
	cout << "\t  " << "\tall results will be printed.    \t\toptional" << endl;
	cout << "\t-h" << "\tprint this help message.        \t\toptional" << endl;

	cout << "Example:" << endl;
	cout << "\t" << program_name << " -c ../cfg/semantic.cfg -s 我想听青花瓷" << endl;
	cout << "\t" << program_name << " -c ../cfg/semantic.cfg -f test.txt -o result.txt" << endl;

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
				return -1;
			if(argv[i][1]=='c' || argv[i][1]=='s' || argv[i][1]=='f' || argv[i][1]=='o' || argv[i][1]=='d' || argv[i][1]=='p' || argv[i][1]=='n'){
				pre_arg_char = argv[i][1];
				state = WAIT_ARG;
			}else if(argv[i][1] == 'h'){
				pre_arg_char = argv[i][1];
			}else
				return -1;
		// expect an argument
		}else{
			if(argv[i][0] == '-')
				return -1;
			else if(WAIT_OPT == state)
				return -1;
			else
				args[pre_arg_char] = argv[i];
				state = WAIT_OPT;
		}
	}
	return 0;
}

int main(int argc, const char** argv){
	map<char, string> args;
	// if(argc<2){
	// 	print_help(argv[0]);
	// 	return -1;
	// }

	int ret = parse_args(argc, argv, args);
	if(ret != 0){
		print_help(argv[0]);
		return -1;
	}

	if(args.find('h') != args.end()){
		print_help(argv[0]);
		return 0;
	}

	if(args.find('c') == args.end()){
		print_help(argv[0]);
		return -1;
	}

	if(args.find('s') == args.end()){
		if(args.find('f') == args.end()){
			print_help(argv[0]);
			return -1;
		}
	}else{
		if(args.find('f') != args.end()){
			print_help(argv[0]);
			return -1;
		}
	}

	int n_best = -1;
	if(args.find('n') != args.end()){
		n_best = atoi(args['n'].c_str());
		if(n_best == 0)
			// std::cerr << ">>>>>> WARNING: n-best number error, all results will be printed." << endl;
			n_best = -1;
	}

	int thread_num = 1;
	if(args.find('p') != args.end()){
		thread_num = atoi(args['p'].c_str());
		if(thread_num == 0){
			// std::cerr << ">>>>>> WARNING: thread number error, using 1 as default." << endl;
			thread_num = 1;
		}
	}

	ofstream ouf;
	ostream* out = &cout;
	if(args.find('o') != args.end()){
		ouf.open(args['o'].c_str());
		if(!ouf.is_open()){
			cout << ">>>>>> ERROR: Cannot open " << args['o'] << " for writing." << endl;
			return -1;
		}
		out = dynamic_cast<ostream*>(&ouf);
	}

	hobotlog::LogModule *loginstance = new hobotlog::LogModule("./conf/log.cfg");

	SemanticRegex::Regex *parser = new SemanticRegex::Regex(args['c'], loginstance);
	// loginstance->debug(">>>>>> DEBUG: Initialized SemanticRegex::Regex successfully!"); 

	//< 若指定领域不存在
	string use_domain = "";
	if(args.find('d') != args.end()){
		use_domain = args['d'];
		if(parser->domain_lex.find(use_domain) == parser->domain_lex.end()){
			std::cerr << ">>>>>> ERROR: Cannot find domain \"" << use_domain << "\" in config file, please have a check." << endl;
			delete parser;
			return -1;
		}
	}

	ParseArgs *pargs = new ParseArgs();
	pargs->parser = parser;
	pargs->out = out;
	pargs->use_domain = use_domain;
	pargs->n_best = n_best;

	clock_t start = clock();

	if(args.find('f') != args.end()){
		ifstream inf(args['f'].c_str());
		if(!inf){
			std::cerr << ">>>>>> ERROR: Cannot open input file!" << endl;
			delete parser;
			return -1;
		}

		string line;
		vector<string> querys;
		querys.clear();
		pthread_t* thread_arr = new pthread_t[thread_num];
		Pack* packs = new Pack[thread_num];

		while(getline(inf, line)){
			querys.push_back(line);
			if(querys.size()>=thread_num){
				for(int i=0; i<thread_num; i++){
					packs[i].pargs = pargs;
					packs[i].query = querys[i];
					packs[i].thread_id = i;
					if(0 != pthread_create(&thread_arr[i], NULL, work, (void*)(&packs[i]))){
						std::cerr << ">>>>>> ERROR: thread create failed!" << endl;
						return -1;
					}
				}

				for(int i=0; i<thread_num; i++){
					pthread_join(thread_arr[i], NULL);
				}
				querys.clear();
			}
		}
		inf.close();
		delete [] thread_arr;
		delete [] packs;
	}else{
		vector<string> sems = parser->parse(args['s'], use_domain, n_best);
        *out << endl;
		for(size_t i=0; i<sems.size(); i++)
			*out << sems[i] << endl;
		if(sems.size()<=0)
			std::cerr << "<<<<<< No regex matched!" << endl;
	}

	clock_t end = clock();
	cout << "match time: " << (end - start)*1.0/CLOCKS_PER_SEC << endl;

	if(args.find('o') != args.end())
		ouf.close();
	delete parser;
	return 0;
}

