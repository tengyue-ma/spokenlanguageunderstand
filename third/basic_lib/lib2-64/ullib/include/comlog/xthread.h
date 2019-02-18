/***************************************************************************
 * 
 * Copyright (c) 2014 Baidu.com, Inc. All Rights Reserved
 * $Id: xthread.h,v 1.0 2014/11/05 11:02:57 linjieqiong Exp $ 
 * 
 **************************************************************************/
 
/**
 * @file xthread.h
 * @author linjieqiong(linjieqiong@baidu.com)
 * @date 2014/11/05 15:44:19
 * @version $Revision: 1.0 $ 
 * @brief 
 *  
 **/
#ifndef LIBSRC_ULLIB_INCLUDE_COMLOG_XTHREAD_H
#define LIBSRC_ULLIB_INCLUDE_COMLOG_XTHREAD_H

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

namespace comspace {

typedef void* VoidPtr;
typedef int (*UserCallback)(VoidPtr param);

enum XThreadAction {
    XTHREAD_CREATE = 0,
    XTHREAD_RUN = 1,
    XTHREAD_STOP = -1,
}; // enum XThreadAction

struct XThreadCallback {
    UserCallback user_callback;
    VoidPtr param;
}; // struct XThreadCallback 
typedef XThreadCallback* XThreadCallbackPtr;

class XThread {
public:
    XThread();
    ~XThread();
    int start(UserCallback callback, VoidPtr param);
    int join();

protected:
    static VoidPtr routine(VoidPtr param);

private:
    XThreadCallbackPtr _callback;
    pthread_t _tid;
}; // class XThread

} // namespace comspace

#endif // LIBSRC_ULLIB_INCLUDE_COMLOG_XTHREAD_H
