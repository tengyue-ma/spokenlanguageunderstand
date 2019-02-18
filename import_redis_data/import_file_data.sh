#!/bin/bash

if [ $# -lt 1 ]; then
	echo "Usage:"
	echo "    $0  filename  [prefix]"
	echo "Arguments:"
	echo "    1. dict filename"
	echo "    2. prefix for each line"
	echo "Example1:"
    echo "    command:  $0  artist  music_artist"
	echo "    function: import data like:"
    echo "              music_artist_周 0"
    echo "              music_artist_周杰 0"
    echo "              music_artist_周杰伦 1"
    echo "Example2:"
    echo "    command:  $0  artist"
	echo "    function: import data like:"
    echo "              周 0"
    echo "              周杰 0"
    echo "              周杰伦 1"
	exit;
fi

indir=$1
base_dir=$(cd `dirname $0`; pwd)
mkdir -p $base_dir/cmds
mkdir -p $base_dir/procs

# 生成redis命令文件,如:
#   set music_artist_周 0
if [ $# == 2 ]; then
    python $base_dir/gen_redis_cmd.py $indir $base_dir/cmds/redis.cmd $2
else
    python $base_dir/gen_redis_cmd.py $indir $base_dir/cmds/redis.cmd
fi

# 生成redis的protocol
python $base_dir/filecmd2pipe.py $base_dir/cmds/redis.cmd $base_dir/procs/redis.proc

## 导入到redis
# 旧开发环境（已经被IT收回）
#cat $base_dir/procs/redis.proc | ./redis-4.0.2/src/redis-cli -a myredis -h 10.10.10.102 -p 6379 --pipe

#新开发环境
cat $base_dir/procs/redis.proc | ./redis-4.0.2/src/redis-cli -a myredis -h 10.10.108.39 -p 6379 --pipe

# 测试环境
#cat $base_dir/procs/redis.proc | ./redis-4.0.2/src/redis-cli -a hobot-redis-ha -h 10.10.100.198 -p 6379 --pipe

# 线上环境
#cat $base_dir/procs/redis.proc | ./redis-4.0.2/src/redis-cli -a myredis -h 172.17.16.194 -p 29736 --pipe

