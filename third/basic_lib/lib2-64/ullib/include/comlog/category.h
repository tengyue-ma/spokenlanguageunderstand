/***************************************************************************
 *
 * Copyright (c) 2008 Baidu.com, Inc. All Rights Reserved
 * $Id: category.h,v 1.2 2009/03/25 12:34:11 yingxiang Exp $
 *
 **************************************************************************/



/**
 * @file category.h
 * @author xiaowei(xiaowei@baidu.com)
 * @date 2008/01/30 20:10:09
 * @version $Revision: 1.2 $
 * @brief
 *
 **/


#ifndef  __CATEGORY_H_
#define  __CATEGORY_H_

#include <stdio.h>
#include <stdarg.h>

#include "comlog.h"
#include "logstat.h"
#include "appender/appender.h"
#include "event.h"

namespace comspace
{

typedef void(* log_callback_t)(int loglevel, const char *format, va_list ap, void *param);

class Category
{
    class Push
    {
        Category * _cate;
    public:
        Push(Category *);
        Push & operator () (const char *key, const char *fmt, ...);
        int operator () (int level, const char *fmt, ...);
    };

    public:
        Event *_event;                                  /**<        */

    protected:
        Category *_parent;    //父亲句柄
        LogStat *_logstat;    //日志状态控制句柄

        Appender *_appenders[COM_MAXDEVICENUM];          /**<        */
        int _appnum;                                  /**<        */
        int _appender_mask[COM_MAXDEVICENUM];
        int _mask_flag;

    protected:
        Category(Category *parent = NULL);

        static log_callback_t _g_callback;          /**<  回调函数      */
        static void *_g_callback_param;              /**<  回调函数自定义参数      */
    public:
        virtual ~Category();

        int setLogStat(LogStat *logstat = NULL);    //重新设置控制句柄
        int print(u_int level, const char *fmt, ...);    //根据日志等级打印日志

        //添加,删除和清空appender
        int addAppender(Appender *);
        int removeAppender(Appender *);
        int clearAppender();
        //添加，删除对某个appender的mask
        int addAppenderMask(char *appender_type);
        //int removeAppenderMask(char *appender_type);

        int print(u_int level, const char *fmt, va_list args);
        inline LogStat * logstat() {
            return _logstat;
        }
        inline int appender_num() {
            return _appnum;
        }

        // stream
        Category& setLogInfo(int level);
        void flush(void);

        Category& operator << (Category& (*endl_)(Category&)) { return endl_(*this); }
        Category& operator << (const char* str);
        Category& operator << (const unsigned char* str);
        Category& operator << (void* ptr);
        Category& operator << (unsigned char value);
        Category& operator << (char value);
        Category& operator << (unsigned short value);
        Category& operator << (short value);
        Category& operator << (unsigned int value);
        Category& operator << (int value);
        Category& operator << (unsigned long value);
        Category& operator << (long value);
        Category& operator << (unsigned long long value);
        Category& operator << (long long value);
        Category& operator << (float value);
        Category& operator << (double value);
        Category& operator << (long double value);

    private:
        template<typename T>
        Category& serialize(const char* fmt, T value);

    public:
        Push push;

    public:
        //static pthread_mutex_t glock;
        static Category * getInstance(const char *name);
        static void setCallback(log_callback_t callback, void *callback_param)
        {
            _g_callback = callback;
            _g_callback_param = callback_param;
        }
        //自定义日志等级
        //建立日志等级到日志名的对应关系
        //自定义日志id从USERLOGLEVEL(32)到
        //根据等级名得到等级id，如果等级名不存在，那么会自动分配一个id，最多不超过个自动分配id。
        //static unsigned int getLogID(const char *name);
};
};

namespace std
{
    inline comspace::Category& endl(comspace::Category& category_)
    {
        category_.flush();
        return category_;
    }
}

#endif  //__CATEGORY_H_

/* vim: set ts=4 sw=4 sts=4 tw=100 */
