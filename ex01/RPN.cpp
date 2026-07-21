#include "RPN.hpp"
#include <cstdlib>

RPN::RPN() : _op('O') {}
RPN::RPN(const RPN& copy) : _stack(copy._stack), _op(copy._op) {}

RPN RPN::operator=(const RPN& other){
	if (this != &other)
	{
		this->_stack = other._stack;
		this->_op = other._op;
	}
	return (*this);
}

void RPN::initStack(std::string content){
	for (size_t i = 0; i < content.size(); i++)
	{
		if (!isdigit(content[i]) && content[i] != '+' && content[i] != '-' && content[i] != '*' && content[i] != '/' && content[i] != ' ')
		{
			throw std::runtime_error("Invalid argument");
		}
		if (content[i] == ' ')
			i++;
		if (content[i] == '+' || content[i] == '-' || content[i] == '*' || content[i] == '/')
		{
			_op = content[i];
			if (_stack.size() >= 2)
				calculator();
			else
				throw std::runtime_error("Invalid argument");
		}
		if (isdigit(content[i]))
			_stack.push(content[i] - '0');
	}
}

void RPN::calculator(){
	int right;
	int left;
	right = _stack.top();
	_stack.pop();

	left = _stack.top();
	_stack.pop();

	switch (_op)
	{
	case '+':
		left += right;
		break;
	case '-':
		left -= right;
		break;
	case '*':
		left *= right;
		break;
	case '/':
		if (right == 0)
			throw std::runtime_error("division by zero");
		left /= right;
		break;
	default:
		throw std::runtime_error("unknown op");
	}
	_stack.push(left);
}

void RPN::printResult(){
	if (_stack.size() != 1)
		throw std::runtime_error("error");
	std::cout << _stack.top() << std::endl;
}

RPN::~RPN(){}
