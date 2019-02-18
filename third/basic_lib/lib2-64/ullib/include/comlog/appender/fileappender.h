/***************************************************************************
 *
 * Copyright (c) 2008 Baidu.com, Inc. All Rights Reserved
 * $Id: fileappender.h,v 1.2 2009/03/25 12:34:11 yingxiang Exp $
 *
 **************************************************************************/



/**
 * @file fileappender.h
 * @author xiaowei(com@baidu.com)
 * @date 2008/03/18 22:15:24
 * @version $Revision: 1.2 $
 * @brief
 *
 **/


#ifndef  __FILEAPPENDER_H_
#define  __FILEAPPENDER_H_

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <string>

#include "comlog/appender/appender.h"
#include "comlog/xqueue.h"
#include "comlog/xthread.h"

namespace comspace
{

struct filetime {
    char filepath[256];
    int64_t time;
};

class FileAppender : public Appender
{
    char _file[COM_MAXFILENAME];
    struct stat _file_stat;
    int _page;
    time_t _lastcutime;

    int _quota_size;        //最大的磁盘文件总大小，单位是M
    int _quota_day;            //保存的最长时间，单位是天
    int _quota_hour;        //保存的最长时间，单位是小时
    int _quota_min;            //保存的最长时间，单位是分钟
    int _quota_filenum;        //计算可以保留的文件个数，估算值
    struct filetime *_ft;        //存放具体的文件信息
    int _ftsize;            //已经保存在数组队列中的文件个数
    int _head;            //保存当前刚保存的文件

    static const int UNLINK_QUEUESIZE = 1024;
    static const int UNLINK_DEF_TIMEOUT = 60;
    static const int MAX_UNLINK_FILES = 10;

    XSigQu<std::string> _unlink_queue;
    std::string _unlinkbuf[MAX_UNLINK_FILES];
    XThread _async_thread;

    pthread_key_t _file_fd_key;
public:
    int open(void *);
    int close(void *);
    int print(Event *evt);
    int binprint(void *, int size);
    int reset();
    int check_quota(const char *mvfile, bool first);
    int unlink_log_files();
    ~FileAppender();
protected:
    int init();
    int syncid(void *);
    int read_conf();
    int check_quota_first();
    void cleanup_unlink_queue();

    FileAppender();

    int set_file_id(int fd);
    int get_file_id(void);
    int get_file_inode(void);
    int get_file_info(int* fd, ino_t* finode);

public:
    static Appender *getAppender(com_device_t &dev);
    static Appender *tryAppender(com_device_t &dev);
protected:
    friend class Appender;
    //friend class UlFileAppender;
    friend class UlLogAppender;
    friend class NetAppender;
};

};












#endif  //__FILEAPPENDER_H_

/* vim: set ts=4 sw=4 sts=4 tw=100 */
