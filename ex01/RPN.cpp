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
		if (content[i] == '+' || content[i] == '-' || content[i] == '*' || content[i] == '/')
			_op = content[i];
		if (isdigit(content[i]))
			_stack.push(content[i]);
	}
}

void RPN::calculator()
{
	int right;
	int left;
	int result;

	right = _stack.top() - '0';
	_stack.pop();

	left = _stack.top() - '0';
	_stack.pop();

	std::cout << "DEBUG " << left << std::endl;
	std::cout << "DEBUG " << right << std::endl;
	switch (_op)
	{
	case '+':
		result = left + right;
		break;
	case '-':
		result = left - right;
		break;
	case '*':
		result = left * right;
		break;
	case '/':
		if (right == 0)
			throw std::runtime_error("division by zero");
		result = left / right;
		break;
	default:
		throw std::runtime_error("unknown op");
	}

	_stack.push(result);
}

void RPN::printResult(){
	if (_stack.size() != 1)
		throw std::runtime_error("invalid expression");

	std::cout << _stack.top() << std::endl;
}



RPN::~RPN(){}
