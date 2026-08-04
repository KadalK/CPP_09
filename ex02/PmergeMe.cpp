#include "PmergeMe.hpp"

#include <algorithm>
#include <cstdlib>

PmergeMe::PmergeMe()
					: _rest(-1),
					  _asRest(false),
					  _groupSize(2)
{}

PmergeMe::PmergeMe(const PmergeMe& copy)
	: _rest(copy._rest),
	  _asRest(copy._asRest),
	  _groupSize(copy._groupSize),
	  _input(copy._input),
	  _sorted(copy._sorted),
	  _pending(copy._pending),
	  _groups(copy._groups)
{}

PmergeMe& PmergeMe::operator=(const PmergeMe& other){
	if (this != &other)
	{
		_rest = other._rest;
		_asRest = other._asRest;
		_groupSize = other._groupSize;
		_input = other._input;
		_groups = other._groups;
		_pending = other._pending;
		_sorted = other._sorted;
	}
	return (*this);
}

// void PmergeMe::DebugPrintContain(){
// 	std::cout << "_groups" << std::endl;
// 	for (size_t i = 0; i < _groups.size(); i++)
// 	{
// 		for (size_t j = 0; j < _bucketSize; j++)
// 		std::cout << _groups[i].bucket[i] << std::endl;
// 	}
// 	std::cout << "_rest" << std::endl;
// 	std::cout << _rest << std::endl;
// }


void PmergeMe::DebugPrintSorted(){
	std::cout << "_sorted: ";

	for (size_t i = 0; i < _sorted.size(); i++)
		std::cout << _sorted[i] << " ";

	std::cout << std::endl;
}

// void PmergeMe::DebugPrintPending(){
// 	std::cout << "_pending: ";
//
// 	for (size_t i = 0; i < _pending.size(); i++)
// 		std::cout << _pending[i] << " ";
//
// 	std::cout << std::endl;
// }

// void PmergeMe::DebugPrintContain()
// {
// 	std::cout << "_groups" << std::endl;
//
// 	for (size_t i = 0; i < _groups.size(); i++)
// 	{
// 		std::cout << "Group " << i << ": ";
//
// 		for (size_t j = 0; j < _groups[i].bucket.size(); j++)
// 			std::cout << _groups[i].bucket[j] << " ";
//
// 		std::cout << std::endl;
// 	}
// 	std::cout << "_rest" << std::endl;
// 	std::cout << _rest << std::endl;
// }

void PmergeMe::initContainer(int ac, char **av)
{
	for (int i = 1; i < ac; i++)
	{
		if (std::atoi(av[i]) < 0)
			std::cout << "negativ number" << std::endl;

		_input.push_back(std::atoi(av[i]));
	}
}

void PmergeMe::initGroups()
{
	_groups.clear();

	for (size_t i = 0; i + 1 < _input.size(); i += 2)
	{
		std::vector<unsigned int> group;

		group.push_back(_input[i]);
		group.push_back(_input[i + 1]);

		if (group[0] > group[1])
			std::swap(group[0], group[1]);

		_groups.push_back(group);
	}

	if (_input.size() % 2 != 0)
	{
		_asRest = true;
		_rest = _input.back();
	}
}

void    PmergeMe::initMain(){
	_sorted.clear();

	if (_groups.empty())
		return;

	for (size_t i = 0; i < _groups.size(); i++)
	{
		_sorted.push_back(_groups[i][1]);
	}
	_sorted.insert(_sorted.begin(), _groups[0][0]);
}

void PmergeMe::initPending(){
	_pending.clear();

	for (size_t i = 1; i < _groups.size(); i++)
		_pending.push_back(_groups[i]);
}

// void PmergeMe::initGroups(){
// 	_groups.clear();
//
// 	for (size_t i = 0; i + 1 < _input.size(); i += 2)
// 	{
// 		std::vector<unsigned int> group;
//
// 		group.push_back(_input[i]);
// 		group.push_back(_input[i + 1]);
//
// 		if (group[0] > group[1])
// 			std::swap(group[0], group[1]);
//
// 		_groups.push_back(group);
// 	}
//
// 	if (_input.size() % 2 != 0)
// 	{
// 		_asRest = true;
// 		_rest = _input.back();
// 	}
// }

void PmergeMe::sortGroups()
{
	for (size_t j = 0; j < _groups.size(); j++)
	{
		for (size_t i = 0; i + 1 < _groups.size() - j; i++)
		{
			if (_groups[i][1] > _groups[i + 1][1])
				std::swap(_groups[i], _groups[i + 1]);
		}
	}
}

void PmergeMe::mergeGroups(std::vector<std::vector<unsigned int> >& v)
{
	std::vector<std::vector<unsigned int> > newGroups;

	for (size_t i = 0; i + 1 < v.size(); i += 2)
	{
		std::vector<unsigned int> group;

		group.insert(group.end(), v[i].begin(), v[i].end());

		group.insert(group.end(), v[i + 1].begin(), v[i + 1].end());

		newGroups.push_back(group);
	}

	if (v.size() % 2 != 0)
		newGroups.push_back(v.back());

	v = newGroups;
}

void PmergeMe::recursiveSort(std::vector<std::vector<unsigned int> >& v)
{
	if (v.size() < 2)
		return;

	// sort les groupes selon leur dernier element
	for (size_t j = 0; j < v.size(); j++)
	{
		for (size_t i = 0; i + 1 < v.size() - j; i++)
		{
			if (v[i].back() > v[i + 1].back())
				std::swap(v[i], v[i + 1]);
		}
	}

	// merge deux groupes voisins
	mergeGroups(v);
	_groupSize *= 2;
	recursiveSort(v);
}

size_t PmergeMe::binarySearch(unsigned int value, size_t limit){
	size_t left = 0;
	size_t right = limit;

	while (left < right)
	{
		size_t mid = (left + right) / 2;

		if (_sorted[mid] < value)
			left = mid + 1;
		else
			right = mid;
	}

	return left;
}

// j(n - 1) + 2 * j(n - 2)
std::vector<size_t> PmergeMe::generateJacobsthalOrder(size_t size){
	std::vector<size_t> order;

	size_t previous = 0;
	size_t current = 1;

	while (current < size)
	{
		for (size_t i = current; i > previous; i--)
			order.push_back(i - 1);

		size_t next = current + 2 * previous;

		previous = current;
		current = next;
	}

	for (size_t i = size; i > previous; i--)
		order.push_back(i - 1);

	return order;
}

size_t PmergeMe::findPosition(unsigned int value)
{
	for (size_t i = 0; i < _sorted.size(); i++)
	{
		if (_sorted[i] == value)
			return i;
	}

	return _sorted.size();
}

// void PmergeMe::insertPending()
// {
// 	for (size_t i = 0; i < _pending.size(); i++)
// 	{
// 		unsigned int small = _pending[i][0];
// 		unsigned int big = _pending[i][1];
//
// 		size_t limit = findPosition(big);
//
// 		size_t pos = binarySearch(small, limit);
//
// 		_sorted.insert(_sorted.begin() + pos, small);
// 	}
// }

void	PmergeMe::insertPending(){
	std::vector<size_t> order = generateJacobsthalOrder(_pending.size());

	for (size_t i = 0; i < order.size(); i++)
	{
		size_t index = order[i];

		unsigned int small = _pending[index][0];
		unsigned int big = _pending[index][1];

		size_t limit = findPosition(big);
		size_t pos = binarySearch(small, limit);

		_sorted.insert(_sorted.begin() + pos, small);
	}
}


void PmergeMe::init(int ac, char **av){
	initContainer(ac, av);
	initGroups();
}

void PmergeMe::run()
{
	sortGroups();
	initMain();
	initPending();

	insertPending();

	if (_asRest)
	{
		size_t pos = binarySearch(_rest, _sorted.size());
		_sorted.insert(_sorted.begin() + pos, _rest);
	}

	DebugPrintSorted();
}



PmergeMe::~PmergeMe(){}
