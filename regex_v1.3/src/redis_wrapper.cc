#include <iostream>
#include "redis_wrapper.h"
using std::cout;
using std::endl;

namespace SemanticRegex{

RedisWrapper::RedisWrapper(redisContext* con){
	this->con = con;
	pthread_mutex_init(&redis_mutex, NULL);
}

RedisWrapper::~RedisWrapper(){
	redisFree(con);
	pthread_mutex_destroy(&redis_mutex);
}

/*
 * 查询接口,先设置互斥锁
 * 再查询,然后释放互斥锁
 */
redisReply* RedisWrapper::query_redis(const string& cmd){
	redisReply *rp;
	pthread_mutex_lock(&redis_mutex);
	rp = (redisReply*)redisCommand(con, cmd.c_str());
	pthread_mutex_unlock(&redis_mutex);
	return rp;
}

}	// end of namespace SemanticRegex
