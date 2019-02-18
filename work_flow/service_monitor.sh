#! /bin/bash

#======================================================================
# file: service_monitor.sh

# write by zhouleilei 2018/05/29

# desc: a tool for checking cpu, monitor for every 30 seconds.

# 使用方法：nohup sh service_monitor.sh 程序名称 > /dev/null &
           #在当前目录会生成log文件
           #例如：nohup sh service_monitor.sh test > /dev/null &

#输出log格式： 时间, 用户名 进程id xxx %MEM VIRT(虚拟内存) 内存 ……
              #重点看从%MEM开始的三列, 有没有持续增长的情况, 有波动属于正常
#======================================================================

if [ "$#" -lt 1 ]
then
    echo "Usage: $0 program"
    exit -1;
fi

SLEEP_TIME=10

PROGRAM=$1

LOG_FILE="monitor_check_service_nlu.log"
LOG_SIZE=`expr 10 \* 1024 \* 1024`
# LOG_SIZE=100

echo "monitor_ckeck is checking......"

while true
do
    # log backup
    CUR_SIZE=`du -b ./$LOG_FILE 2>/dev/null 1>&2 | awk '{print $1}'`
    if [ -z $CUR_SIZE ]
    then
        CUR_SIZE=0
    fi

    if [ $CUR_SIZE -gt $LOG_SIZE ]
    then
        mv $LOG_FILE $LOG_FILE.bak 2>/dev/null 1>&2
    fi
    
    PRO_NOW=`ps ux | grep -w $PROGRAM | grep -v grep | grep -v service_monitor.sh | grep -v gdb | wc -l`

    if [ $PRO_NOW -eq 0 ]
    then
        nohup ./$PROGRAM > /dev/null &
    else
        CONTENT=`ps ux | grep -w $PROGRAM | grep -v grep | grep -v service_monitor.sh | grep -v gdb`
 	      echo "`date +"%Y-%m-%d %H_%M_%S"`, $CONTENT" >> ./$LOG_FILE
    fi 
    
    sleep $SLEEP_TIME
        
done

exit 0


