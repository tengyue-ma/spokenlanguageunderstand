#include <iostream>
#include "hiredis.h"
using namespace std;

int main(){
	redisContext *con = redisConnect("127.0.0.1", 6379);
	if(con!=NULL && con->err){
		cout << "Failed to connect!" << endl;
		return 0;
	}
	string cmd = "SISMEMBER song 青花瓷";
	redisReply *reply = (redisReply*)redisCommand(con, cmd.c_str());
	if(reply->integer == 1){
		cout << "yes, it's in the set!" << endl;
	}
	freeReplyObject(reply);
	redisFree(con);
	return 0;
}
