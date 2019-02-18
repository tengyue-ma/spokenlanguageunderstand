/***************************************************************************
 * 
 * Copyright (c) 2014 Baidu.com, Inc. All Rights Reserved
 * $Id: xscoped.h,v 1.0 2014/11/07 20:02:57 linjieqiong Exp $ 
 * 
 **************************************************************************/
 
/**
 * @file xscoped.h
 * @author linjieqiong(linjieqiong@baidu.com)
 * @date 2014/11/07 20:44:19
 * @version $Revision: 1.0 $ 
 * @brief 
 *  
 **/
#ifndef LIBSRC_ULLIB_INCLUDE_COMLOG_XSCOPED_H
#define LIBSRC_ULLIB_INCLUDE_COMLOG_XSCOPED_H

#include <pthread.h>

namespace comspace {

class xAutoLock {
public:
    explicit xAutoLock(pthread_mutex_t* m) : _mutex(m) {
        pthread_mutex_lock(_mutex);
    }
    ~xAutoLock() {
        pthread_mutex_unlock(_mutex);
    }

private:
    xAutoLock(const xAutoLock&);
    void operator=(const xAutoLock&);

    pthread_mutex_t* _mutex;
}; // class xAutoLock

} // namespace comspace

#endif // LIBSRC_ULLIB_INCLUDE_COMLOG_XSCOPED_H
