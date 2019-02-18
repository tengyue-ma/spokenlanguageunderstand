#include <stdio.h>
#include "hiredis.h"

int main(){
	redisContext *con = redisConnect("127.0.0.1", 6379);
	if(con!=NULL && con->err){
		printf("Failed to connect!\n");
		return 0;
	}
	char cmd[] = "SISMEMBER song 花瓷";
	redisReply *reply = (redisReply*)redisCommand(con, cmd);
	if(reply->integer == 1){
		printf("yes, it's in the set!\n");
	}else{
		printf("no, it's not in the set!\n");
	}
	freeReplyObject(reply);
	redisFree(con);
	return 0;
}
