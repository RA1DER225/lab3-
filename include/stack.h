#pragma once
#include<iostream>
template<typename T>
class Stack {
	int top;
	size_t memsize;
	T* pMem;
public:
	Stack(size_t _memsize = 1) :top(-1), memsize(_memsize), pMem(new T[]memsize) {}
	~Stack() {
		delete[] pMem;
	}
	bool IsEmpty();
	bool IsFull();
	void Push(const T& val);
	void Pop();
	T Top();

};
