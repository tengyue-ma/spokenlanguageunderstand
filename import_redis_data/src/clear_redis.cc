#include <iostream>
#include <string>
#include <cstring>
#include <stdlib.h>
#include <map>
#include "hiredis.h"
using namespace std;

#define ERR_PARSE_ARGS -10000
#define ERR_CHECK_ARGS -10001

//> 输入参数, 没有默认值
//> 服务器ip
string server = "";
//> 端口号
int port = 0;
//> 密码
string password = "";

void print_help(const char* program_name){
	cout << "This is a clear redis database tool." << endl << endl;

	cout << "Usage:" << endl;
	cout << "\t" << program_name << " [options]" << endl;
	cout << "Options:" << endl;
	cout << "\t-h" << "\tserver ip.                      \t\toptional" << endl;
	cout << "\t-p" << "\tport number.                    \t\toptional" << endl;
	cout << "\t-a" << "\tpassword.                       \t\toptional" << endl;

	cout << "Example:" << endl;
	cout << "\t" << program_name << " -h 10.10.10.102 -p 6379 -a myredis" << endl;

	cout << "Note:" << endl;
	cout << "\t1). -h -p -a can not use default value, must exist." << endl;

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
			if(argv[i][1]=='h' || argv[i][1]=='p' || argv[i][1]=='a'){
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

	//所有参数都必须有
	if(args.find('h') == args.end() || args.find('p') == args.end() || args.find('a') == args.end()){
		print_help(program_name);
		cout << ">>>>>> ERROR: args input error !"<< endl;
		return ERR_CHECK_ARGS;
	}
	else{
		server = args['h'].c_str();
		port = atoi(args['p'].c_str());
		password = args['a'].c_str();
	}

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

	//3.清空整个数据库
    string cmd = "flushall";
	cout << ">>>>>> Execute redis command: " << cmd << endl;
	rp = (redisReply*)redisCommand(con, cmd.c_str());
	
	//4.释放
	freeReplyObject(rp);
	redisFree(con);

	return 0;
}
