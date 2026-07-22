#include "PmergeMe.hpp"
#include <cstdlib>

PmergeMe::PmergeMe() : _rest(-1), _asRest(false) {}

PmergeMe::PmergeMe(const PmergeMe& copy) : _rest(copy._rest), _asRest(copy._asRest), _input(copy._input), _pairs(copy._pairs), _pending(copy._pending) ,_sorted(copy._sorted) {}

PmergeMe& PmergeMe::operator=(const PmergeMe& other){
	if (this != &other)
	{
		_rest = other._rest;
		_asRest = other._asRest;
		_input = other._input;
		_pairs = other._pairs;
		_pending = other._pending;
		_sorted = other._sorted;
	}
	return (*this);
}

void PmergeMe::DebugPrintContain(){
	std::cout << "_pairs" << std::endl;
	for (size_t i = 0; i < _pairs.size(); i++)
	{
		std::cout << _pairs[i].small << " , " << _pairs[i].big << std::endl;
	}
	std::cout << "_rest" << std::endl;
	std::cout << _rest << std::endl;
}


void PmergeMe::DebugPrintSorted(){
	std::cout << "_sorted: ";

	for (size_t i = 0; i < _sorted.size(); i++)
		std::cout << _sorted[i] << " ";

	std::cout << std::endl;
}

void PmergeMe::DebugPrintPending(){
	std::cout << "_pending: ";

	for (size_t i = 0; i < _pending.size(); i++)
		std::cout << _pending[i] << " ";

	std::cout << std::endl;
}

void PmergeMe::initContainer(int ac, char **av){
	for (int i = 1; i < ac; i++)
	{
		if (std::atoi(av[i]) < 0)
			std::cout << "negativ number" << std::endl;

		_input.push_back(std::atoi(av[i]));
	}
}

void PmergeMe::initPairs()
{
	size_t pairIndex = 0;

	_pairs.resize(_input.size() / 2);
	_asRest = false;

	for (size_t i = 0; i + 1 < _input.size(); i += 2)
	{
		if (_input[i] < _input[i + 1])
		{
			_pairs[pairIndex].small = _input[i];
			_pairs[pairIndex].big = _input[i + 1];
		}
		else
		{
			_pairs[pairIndex].small = _input[i + 1];
			_pairs[pairIndex].big = _input[i];
		}
		pairIndex++;
	}

	if (_input.size() % 2 != 0)
	{
		_rest = _input.back();
		_asRest = true;
	}
}

void PmergeMe::initSorted()
{
	_sorted.push_back(_pairs[0].small);

	for (size_t i = 0; i < _pairs.size(); i++)
		_sorted.push_back(_pairs[i].big);
}

void PmergeMe::initPending(){
	for (size_t i = 1; i < _pairs.size(); i++)
		_pending.push_back(_pairs[i].small);
}


void PmergeMe::init(int ac, char **av){
	initContainer(ac, av);
	initPairs();
	sortBigPairs();
	initSorted();
	initPending();
}


void PmergeMe::sortBigPairs(){
	for (size_t i = 0; i < _pairs.size(); i++)
	{
		for (size_t j = 0; j + 1 < _pairs.size() - i; j++)
		{
			if (_pairs[j].big > _pairs[j + 1].big)
			{
                Pair tmp;
				tmp = _pairs[j];
				_pairs[j] = _pairs[j + 1];
				_pairs[j + 1] = tmp;
			}
		}
	}
}

PmergeMe::~PmergeMe(){}
