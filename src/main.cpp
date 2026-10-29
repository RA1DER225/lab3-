#include<iostream>
#include "stack.h"

using namespace std;

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
	bool Stack<T>:: IsEmpty() {
		return top < 0;
	};
	bool Stack<T>::IsFull(){
		return top >= memsize;
	};
	void Stack::Push(const T& val){
		if (!IsFull())
		{
			pMem[++top] = val;
		}
	}
	void Stack<T>::Pop() {
		if (!IsEmpty())
		{
			--top;
		}
	}
	T Stack<T>::Top() {
		if (IsEmpty())
		{
			throw - 1;
		}
		T tmp = pMem[top];
		return tmp;
	}
};

int main()
{
	cout << "helloworld";
}