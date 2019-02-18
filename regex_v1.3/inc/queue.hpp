#include "queue.h"

namespace SemanticRegex{

template<typename T>
Queue<T>::Queue(size_t capacity){
	_capacity = capacity+20;
	_data = new T[_capacity];
	_head = 0;
	_tail = 0;
}

template<typename T>
Queue<T>::~Queue(){
	delete [] _data;
	_head = 0;
	_tail = 0;
}

/*
 * 入队一个元素,步骤为
 *	1. 复制t到_tail指向的位置
 *	2. tail往后移动一位
 *	3. 若tail超过数组大小,取模,构成环形队列
 */
template<typename T>
void Queue<T>::push(const T& t){
	_data[_tail++] = t;
	if(_tail>=_capacity)
		_tail = _tail%_capacity;
}

/*
 * 队首元素出队,步骤为
 *	1. head往后移动一位
 *	2. 若head超过数组大小,取模
 */
template<typename T>
T Queue<T>::pop(){
	size_t idx = _head;
	_head++;
	if(_head>=_capacity)
		_head = _head%_capacity;
	return _data[idx];
}

/*
 * 清空队列.把head和tail置0即可
 */
template<typename T>
void Queue<T>::clear(){
	_head = _tail = 0;
}

/*
 * 获取队列长度
 */
template<typename T>
size_t Queue<T>::size(){
	if(_tail>=_head)
		return _tail-_head;
	else
		return _capacity+_tail-_head;
}

/*
 * 获取队首元素
 */
template<typename T>
T Queue<T>::front(){
	return _data[_head];
}

}
