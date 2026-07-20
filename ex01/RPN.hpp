#ifndef RPN_HPP
#define  RPN_HPP

#include <iostream>
#include <stack>

class RPN{
private:
	std::stack<int> _stack;
	char _op;
public:
	RPN();
	RPN(const RPN& copy);
	RPN operator=(const RPN& other);

	void initStack(std::string content);
	void calculator();
	void printResult();

	~RPN();
};

#endif