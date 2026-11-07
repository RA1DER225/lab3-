#pragma once
#include <iostream>
#include <vector>
#include <map>
#include "stack.h"

using namespace std;

class TArithmeticExpression {
	string infix;//обычная запись
	string postfix;//обратная польская запись
	vector<char> lexem;
	map<char, int> priority;
	map<char, double>operands;
public:
	TArithmeticExpression(string str_infix) {
		infix = str_infix;
		priority = { {'+',1},{'-',1},{'*',2},{'/',2} };
		ToPostfix();
	}
	void Parse() {
		for (char c : infix)
		{
			lexem.push_back(c);
		}
	}
	void ToPostfix() {
		Parse();
		//postfix.clear();                    

		Stack<char> st;
		char StackItem;

		for (char Item : lexem) {
			if (isspace(static_cast<unsigned char>(Item)))
				continue;                     //  пробелы

			switch (Item) {
			case '(':
				st.Push(Item);
				break;

			case ')':
				//  снимаем до '('
				while (!st.IsEmpty() && st.Top() != '(') {
					postfix += st.Top();
					st.Pop();
				}
				if (st.IsEmpty()) {
					std::cout << "Error: mismatched ')'\n";
					return;
				}
				st.Pop();                     // выбрасываем '('
				break;

			case '+': case '-': case '*': case '/':
				//  остановка на '(' 
				while (!st.IsEmpty()&& st.Top() != '(' && priority[Item] <= priority[st.Top()]) {
					postfix += st.Top();
					st.Pop();
				}
				st.Push(Item);
				break;

			default:
				operands.insert({ Item, 0.0 });
				postfix += Item;
			}
		}

		//  остаток стека и проверяем на незакрытые '('
		while (!st.IsEmpty()) {
			char c = st.Top();
			st.Pop();
			if (c == '(') {
				std::cout << "Error: mismatched '('\n";
				return;
			}
			postfix += c;
		}
	}

	string GetIndex()const {
		return infix;
	}
	string GetPostfix()const {
		return postfix;
	}
	vector<char> GetOperands()const
	{
		vector<char>op;
		for (const auto& item : operands)
		{
			op.push_back(item.first);
		}
		return op;
	}

	double Calculate(const map <char, double>& values)
	{
		for (auto& val : values)
		{
			operands.at(val.first) = val.second;
		}
		Stack <double> st;
		double left, right;
		for (char lexem : postfix)
		{
			switch (lexem)
			{
			case '+' :
				right = st.Top();
				st.Pop();
				left = st.Top();
				st.Pop();
				st.Push(left + right);
				break;
			case'-':
				right = st.Top();
				st.Pop();
				left = st.Top();
				st.Pop();
				st.Push(left - right);
				break;
			case'*':
				right = st.Top();
				st.Pop();
				left = st.Top();
				st.Pop();
				st.Push(left * right);
				break;
			case'/':
				right = st.Top();
				st.Pop();
				left = st.Top();
				st.Pop();
				st.Push(left / right);
				break;
			default:
				st.Push(operands[lexem]);

			}

		}
		return st.Top();
	}
};





/*void ToPostfix() {
	Parse();
	Stack<char> st;
	char StackItem;
	for (char Item : lexem)
	{
		switch (Item)
		{
		case'(':
			st.Push(Item);
			break;
		case')':
			StackItem = st.Top();
			st.Pop();
			while (StackItem != '(')
			{
				postfix += StackItem;
				StackItem = st.Top();
				st.Pop();
			}
			break;
		case'+':case'-':case '*':case'/':
			while (!st.IsEmpty())
			{
				StackItem = st.Top();
				st.Pop();
				if (priority[Item] <= priority[StackItem])
				{
					postfix += StackItem;
				}
				else
				{
					st.Push(StackItem);
					break;
				}
			}
			st.Push(Item);
			break;
		default:
			operands.insert({ Item,0.0 });
			postfix += Item;
		}
	}
	while (!st.IsEmpty())
	{
		StackItem = st.Top();
		st.Pop();
		postfix += StackItem;
	}
}*/