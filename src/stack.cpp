#include<iostream>
#include<vector>
#include<string>
#include "stack.h"
#include "TArithmeticExpression.h"
#include <map>
#include <cctype>
#include <limits>
#include <set>

using namespace std;

std::map<char, double> ReadVariablesFor(const std::string& expr) {
    // 1. Собираем уникальные буквы из выражения
    std::set<char> names;
    for (char c : expr)
        if (std::isalpha(static_cast<unsigned char>(c)))
            names.insert(c);

    if (names.empty()) {
        std::cout << "In str not variables\n";
        return {};
    }

    // 2. Спрашиваем значение для каждой
    map<char, double> vars;
    cout << "Find variables: " << names.size() << " (";
    for (char c : names)
    {
        cout << c << ' ';
    }
    cout << ")\n\n";

    for (char name : names) {
        double value;
        while (true) {
            cout << "  " << name << " = ";
            if (std::cin >> value) break;

            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            cout << "  --! No value, return-- \n";
        }
        vars[name] = value;
    }

    return vars;
}

int main() {
	cout << "Input infix :";//a*b+c-a
	string str;
	cin >> str;
	cout << "str : " << str << endl;
	cout << "Enter the value for the variables:\n";
    map<char, double> operands=ReadVariablesFor(str);

	for (const auto& [key, value] : operands) {
		cout << key << " = " << value << '\n';
	}
	cout << endl;
	TArithmeticExpression F (str);
	cout << F.GetPostfix() << endl;
    double sum =F.Calculate(operands);
	cout << "sum : " << sum << endl;
}