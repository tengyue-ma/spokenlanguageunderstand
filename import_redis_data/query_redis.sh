#!/bin/bash

#新开发环境
./query_redis -h 10.10.108.39 -p 6379 -a myredis -w music_title_慢慢喜欢你

# 旧开发环境（已经被IT收回）
#./query_redis -h 10.10.10.102 -p 6379 -a myredis -w music_program_家人

# 测试环境
#./query_redis -h 10.10.100.198 -p 6379 -a hobot-redis-ha -w loc_city_南京市

# 线上环境
#./query_redis -h 172.17.16.194 -p 29736 -a myredis -w music_program_家人
