/**
 * FILE        : fixed_buffer.h
 * DESC        : string buffer with fixed size
 * AUTHOR      : v1.0 written by zhaoxin08@baidu.com
 * DATE        : on July 21, 2014
 * LICENCE     : Baidu Inc. all rights reserved.
 * MOD         :
 **/

#ifndef __FIXED_BUFFER__H__
#define __FIXED_BUFFER__H__

#include <string.h>

namespace comspace
{

template<int SIZE>
class fixed_buffer
{
    public:
        fixed_buffer() : cur_(data_) {}
        ~fixed_buffer() {}

        int append(const char* buf, size_t len) {
            if (static_cast<size_t>(avail()) > len) {
                memcpy(cur_, buf, len);
                cur_ += len;

                return 0;
            }

            return -1;
        }

        const char* data() const {
            return data_;
        }

        size_t length() const {
            return static_cast<size_t>(cur_ - data_);
        }

        // write to data_ directly
        char* current() {
            return cur_;
        }

        size_t avail() const {
            return static_cast<size_t>(end() - cur_);
        }

        void add(size_t len) {
            cur_ += len;
        }

        void reset() {
            cur_ = data_;
        }

        void bzero() {
            memset(data_, sizeof(data_), 0);
        }

    private:
        fixed_buffer(const fixed_buffer&) {}
        fixed_buffer& operator == (const fixed_buffer&) {}

    private:
        const char* end() const {
            return data_ + sizeof(data_);
        }

        char    data_[SIZE];
        char*    cur_;
};

}

#endif

