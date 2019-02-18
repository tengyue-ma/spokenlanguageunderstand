#ifndef _REDIS_WRAPPER_H_
#define _REDIS_WRAPPER_H_

#include <string>
#include <pthread.h>
#include "hiredis.h"
using std::string;

namespace SemanticRegex{
	
/*
 * 由于hiredis在多线程查询时容易卡死
 * 故对它做一个封装,每次查询之前,
 * 先设置互斥锁,再进行查询,确保安全
 */
class RedisWrapper{
public:
	RedisWrapper(redisContext *con);
	~RedisWrapper();

public:
	/*
	 * 查询接口
	 */
	redisReply* query_redis(const string& cmd);

public:
	redisContext *con;
	pthread_mutex_t redis_mutex;
};

}	// end of namespace SemanticRegex

#endif
