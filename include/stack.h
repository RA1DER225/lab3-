#pragma once
#include<iostream>
#include<vector>

using namespace std;

template<typename T>
class Stack {
	int top;
	std::vector<T>vec;
public:
	Stack() :top(-1) {};
	size_t size() const {
		return top + 1;
	}
	bool IsEmpty() const
	{
		return top == -1;
	}
	void Push(const T& val)
	{
		vec.push_back(val);
		top++;
	}
	void Pop() {
		if (!IsEmpty())
		{
			vec.pop_back();
			top--;
		}
	}
	T Top() {
		T tmp = vec[top];
		return tmp;
	}
};