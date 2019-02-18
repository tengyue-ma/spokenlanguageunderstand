#include <fstream>
#include <iostream>
#include <string>
#include <cstring>
#include <stdlib.h>
#include <map>
#include "hiredis.h"
using namespace std;

#define ERR_PARSE_ARGS -10000
#define ERR_CHECK_ARGS -10001

//> 输入参数默认值
//> 服务器ip
string server = "10.10.10.102";
//> 端口号
int port = 6379;
//> 密码
string password = "myredis";

void print_help(const char* program_name){
	cout << "This is a query redis database tool." << endl << endl;

	cout << "Usage:" << endl;
	cout << "\t" << program_name << " [options]" << endl;
	cout << "Options:" << endl;
	cout << "\t-h" << "\tserver ip.   default:10.10.10.102       \t\toptional" << endl;
	cout << "\t-p" << "\tport number. default:6379               \t\toptional" << endl;
	cout << "\t-a" << "\tpassword.    default:myredis            \t\toptional" << endl;
	cout << "\t-w" << "\tquery redis word.                       \t\toptional" << endl;

	cout << "Example:" << endl;
	cout << "\t" << program_name << " -h 10.10.10.102 -p 6379 -a myredis -w music_title_青花瓷" << endl;

	cout << "Note:" << endl;
	cout << "\t1). -h -p -a can use default value." << endl;
	cout << "\t2). -w can not use default value." << endl;

	cout << endl;
}

int parse_args(int argc, const char** argv, map<char, string>& args){
	const int WAIT_OPT=0, WAIT_ARG=1;
	int state = WAIT_OPT;
	char pre_arg_char = ' ';
	for(int i=1; i<argc; i++){
		if(strlen(argv[i])==2 && argv[i][0]=='-'){
			if(WAIT_ARG == state)
				return ERR_PARSE_ARGS;
			if(argv[i][1]=='h' || argv[i][1]=='p' || argv[i][1]=='a'  || argv[i][1]=='w'){
				pre_arg_char = argv[i][1];
				state = WAIT_ARG;
			}else
				return ERR_PARSE_ARGS;
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

	//必须有的参数: w
	if(args.find('w') == args.end()){
		print_help(program_name);
		cout << ">>>>>> ERROR: args w unexist !"<< endl;
		return ERR_CHECK_ARGS;
	}

	//如果没有哪个参数, 提醒使用默认值
	if(args.find('h') == args.end())
		cout << ">>>>>> Warning: args -h unexist ! use default: "<< server << endl;
	else
		server = args['h'].c_str();

	if(args.find('p') == args.end())
		cout << ">>>>>> Warning: args -p unexist ! use default: "<< port << endl;
	else
		port = atoi(args['p'].c_str());

	if(args.find('a') == args.end())
		cout << ">>>>>> Warning: args -a unexist ! use default: "<< password << endl;
	else
		password = args['a'].c_str();

	cout<< ">>>>>> server: " << server << ", port: " << port << ", password: " << password <<endl;

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

	//1.连接redis服务
	redisContext *con = redisConnect(server.c_str(), port);
	if(con != NULL && con->err){
		cout << ">>>>>> Failed to connect redis server!" << endl;
		return -1;
	}
	cout << ">>>>>> Success to connect redis server!" <<endl;

	//2.验证密码
    redisReply *rp;
	rp = (redisReply*)redisCommand(con, ("AUTH " + password).c_str());
	if (rp->type == REDIS_REPLY_ERROR) {
		cout << ">>>>>> Failed to Authentication, password wrong!" << endl;
	}
	else
		cout << ">>>>>> Success to Authentication!" << endl;
	
	//3.查询某词是否存在
	string cmd = "GET " + args['w'];
	cout << ">>>>>> Execute redis command: " << cmd << endl;
	rp = (redisReply*)redisCommand(con, cmd.c_str());
	if(rp->str!=NULL){
	    cout << rp->str << endl;
	}else{
		cout << "NULL" << endl;
	}

	//4.释放
	freeReplyObject(rp);
	redisFree(con);

	return 0;
}

