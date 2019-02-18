/***************************************************************************
 *
 * Copyright (c) 2008 Baidu.com, Inc. All Rights Reserved
 * $Id: asyncfileappender.h,v 1.4 2009/09/18 11:02:57 sun_xiao Exp $
 *
 **************************************************************************/



/**
 * @file asyncfileappender.h
 * @author yingxiang(com@baidu.com)
 * @date 2008/08/28 18:06:09
 * @version $Revision: 1.4 $
 * @brief
 *
 **/


#ifndef  __ASYNCFILEAPPENDER_H_
#define  __ASYNCFILEAPPENDER_H_

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/uio.h>

#include "comlog/appender/appender.h"
#include "comlog/appender/fileappender.h"
#include "comlog/appender/netappender.h"
#include "comlog/xqueue.h"
#include "comlog/xthread.h"

#ifdef _XLOG_MEMPOOL_USED_
#include "comlog/comempool/mempool.h"
#endif

#include "fixed_buffer.h"
#include <vector>

namespace comspace
{

const int fixed_buffer_size = 1 * 1024 * 1024;
const int fixed_array_size = 16;

class AsyncFileAppender : public Appender
{
    struct stat _stat;
    char _file[COM_MAXFILENAME];
    int _page;
    time_t _lastcutime;

    typedef fixed_buffer<fixed_buffer_size> _fixed_buffer;
    typedef _fixed_buffer* _fixed_buffer_ptr;
    typedef std::vector<_fixed_buffer_ptr> _fixed_array;
    // pthread_mutex_t        _current_buffer_lock;
    _fixed_buffer_ptr    _current_buffer;
    // 当_current_buffer被放进_queue的时候，需要为它分配新的空间，
    // 可以先到_free_buffer_array中取出相关元素，如果它为空再调用new申请；
    // 当_queue中的元素被取出的时候，需要先放到_free_buffer_array中，
    // 如果放不下再释放掉。_free_buffer_array相当于一个简单的内存池。
    // 其容量固定不变。 -- by zhaoxin08   Aug, 2014.
    _fixed_array        _free_buffer_array;

    typedef NetAppender::netdata_t _netdata_t;
    typedef void * vptr_t;
    XSigQu<vptr_t> _queue;
    XThread _async_thread;

    static const int AFILE_MAXREADITEMS = 1024;
    vptr_t _readbuf[AFILE_MAXREADITEMS];
    iovec afio[AFILE_MAXREADITEMS];

    static const int AFILE_DEFPOPTO = 5;
    static const int AFILE_QUEUESIZE = 20*1024;

#ifdef _XLOG_MEMPOOL_USED_
    comspace::MemPool _mempool;
#endif
public:
    int open(void *);
    int close(void *);
    int print(Event *evt);
    int binprint(void *, int siz);
    int writeData();//like run
    //int flush();
    ~AsyncFileAppender();
protected:
    int init();
    int syncid(void *);
    int erasedata(int);

    AsyncFileAppender();

public:
    static Appender *getAppender(com_device_t &dev);
    static Appender *tryAppender(com_device_t &dev);
protected:
    friend class Appender;
    friend class FileAppender;
    friend class UlLogAppender;
    friend class NetAppender;
};

};












#endif  //__ASYNCFILEAPPENDER_H_

/* vim: set ts=4 sw=4 sts=4 tw=100 */
