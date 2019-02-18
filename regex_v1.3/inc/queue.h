#ifndef _QUEUE_H_
#define _QUEUE_H_

#include <iostream>

/*
 *	本文件定义环形队列
 *	支持出队,入队等操作
 */

namespace SemanticRegex{

template<typename T>
class Queue{
public:
	Queue(size_t capacity);
	~Queue();

public:
	/*
	 * 队首元素出队,并返回队首元素
	 */
	T pop();

	/*
	 * 值t入队,放入队尾
	 * @t: 待入队的值
	 */
	void push(const T& t);

	/*
	 * 清空队列
	 */
	void clear();

	/*
	 * 返回队列中的元素个数
	 */
	size_t size();

	/*
	 * 返回队首元素
	 */
	T front();

private:
	//< 存放数据的数组指针
	T *_data;

	//< 存放数据的数组大小
	size_t _capacity;

	//< 指向队首的index
	size_t _head;

	//< 指向队尾的index
	size_t _tail;
};

}

#endif
