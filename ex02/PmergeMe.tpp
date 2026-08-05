#include "PmergeMe.hpp"
#include <climits>
#include <algorithm>
#include <ctime>

template <template <typename, typename > class Container>
PmergeMe<Container>::PmergeMe()
					: _rest(-1),
					  _asRest(false)
{}

template <template <typename, typename > class Container>
PmergeMe<Container>::PmergeMe(const PmergeMe& copy)
					: _rest(copy._rest),
					  _asRest(copy._asRest),
					  _input(copy._input),
					  _sorted(copy._sorted),
					  _pending(copy._pending),
					  _groups(copy._groups)
{}

template <template <typename, typename > class Container>
PmergeMe<Container>& PmergeMe<Container>::operator=(const PmergeMe& other){
	if (this != &other)
	{
		_rest = other._rest;
		_asRest = other._asRest;
		_input = other._input;
		_groups = other._groups;
		_pending = other._pending;
		_sorted = other._sorted;
	}
	return (*this);
}

inline bool isNumber(char *str){
	size_t i = 0;

	if (str[0] == '\0')
		return false;

	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return false;
		i++;
	}

	return true;
}

template <template <typename, typename > class Container>
void PmergeMe<Container>::initContainer(int ac, char **av){
	for (int i = 1; i < ac; i++)
	{
		if (!isNumber(av[i]))
			throw std::runtime_error("Error");

		unsigned long value = std::atol(av[i]);
		if (value > UINT_MAX)
			throw std::runtime_error("Error");
		if (std::find(_input.begin(), _input.end(), value) != _input.end())
			throw std::runtime_error("Error");

		_input.push_back(static_cast<unsigned int>(value));
	}
}

template <template <typename, typename > class Container>
void PmergeMe<Container>::initGroups(){
	_groups.clear();

	for (size_t i = 0; i + 1 < _input.size(); i += 2)
	{
		Vec group;

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

template <template <typename, typename > class Container>
void PmergeMe<Container>::initMain(){
	_sorted.clear();

	if (_groups.empty())
		return;

	for (size_t i = 0; i < _groups.size(); i++)
	{
		_sorted.push_back(_groups[i][1]);
	}

	_sorted.insert(_sorted.begin(), _groups[0][0]);
}

template <template <typename, typename > class Container>
void PmergeMe<Container>::initPending(){
	_pending.clear();

	for (size_t i = 1; i < _groups.size(); i++)
		_pending.push_back(_groups[i]);
}

template <template <typename, typename > class Container>
void PmergeMe<Container>::sortGroups(){
	for (size_t j = 0; j < _groups.size(); j++)
	{
		for (size_t i = 0; i + 1 < _groups.size() - j; i++)
		{
			if (_groups[i][1] > _groups[i + 1][1])
				std::swap(_groups[i], _groups[i + 1]);
		}
	}
}

template <template <typename, typename > class Container>
void PmergeMe<Container>::mergeGroups(Groups& v){
	Groups newGroups;

	for (size_t i = 0; i + 1 < v.size(); i += 2)
	{
		Vec group;

		group.insert(group.end(), v[i].begin(), v[i].end());
		group.insert(group.end(), v[i + 1].begin(), v[i + 1].end());

		newGroups.push_back(group);
	}

	if (v.size() % 2 != 0)
		newGroups.push_back(v.back());
	v = newGroups;
}

template <template <typename, typename > class Container>
void PmergeMe<Container>::merge(Vec& left, Vec& right, Vec& result){
	Vec tmp;

	size_t i = 0;
	size_t j = 0;

	while (i < left.size() && j < right.size())
	{
		if (left[i] <= right[j])
			tmp.push_back(left[i++]);
		else
			tmp.push_back(right[j++]);
	}

	while (i < left.size())
		tmp.push_back(left[i++]);

	while (j < right.size())
		tmp.push_back(right[j++]);

	result = tmp;
}

template <template <typename, typename > class Container>
void PmergeMe<Container>::recursiveSortBigs(Vec& v){
	if (v.size() < 2)
		return;

	Vec left;
	Vec right;

	size_t mid = v.size() / 2;

	for (size_t i = 0; i < mid; i++)
		left.push_back(v[i]);

	for (size_t i = mid; i < v.size(); i++)
		right.push_back(v[i]);

	recursiveSortBigs(left);
	recursiveSortBigs(right);

	merge(left, right, v);
}

template <template <typename, typename > class Container>
void PmergeMe<Container>::recursiveSort(Groups& v)
{
	if (v.size() < 2)
		return;

	Vec bigs;

	for (size_t i = 0; i < v.size(); i++)
		bigs.push_back(v[i].back());

	recursiveSortBigs(bigs);

	Groups ordered;

	for (size_t i = 0; i < bigs.size(); i++)
	{
		for (size_t j = 0; j < v.size(); j++)
		{
			if (v[j].back() == bigs[i])
			{
				ordered.push_back(v[j]);
				break;
			}
		}
	}

	v = ordered;
}

template <template <typename, typename > class Container>
size_t PmergeMe<Container>::binarySearch(unsigned int value, size_t limit){
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

template <template <typename, typename> class Container>
std::vector<size_t> PmergeMe<Container>::generateJacobsthalOrder(size_t size){
	std::vector<size_t> order;

	size_t j0 = 1;
	size_t j1 = 3;

	while (j1 < size)
	{
		for (size_t i = j1; i > j0; i--)
			order.push_back(i - 1);

		size_t next = j1 + 2 * j0;

		j0 = j1;
		j1 = next;
	}

	for (size_t i = size; i > j0; i--)
		order.push_back(i - 1);

	return order;
}

template <template <typename, typename > class Container>
size_t PmergeMe<Container>::findPosition(unsigned int value)
{
	for (size_t i = 0; i < _sorted.size(); i++)
	{
		if (_sorted[i] == value)
			return i;
	}

	return _sorted.size();
}

template <template <typename, typename > class Container>
void	PmergeMe<Container>::insertPending(){
	if (_pending.empty())
		return;

	std::vector<size_t> order = generateJacobsthalOrder(_pending.size());

	size_t pos = binarySearch(_pending[0][0], findPosition(_pending[0][1]));
	_sorted.insert(_sorted.begin() + pos, _pending[0][0]);

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

template <template <typename, typename > class Container>
void PmergeMe<Container>::init(int ac, char **av){
	initContainer(ac, av);
	initGroups();
}

template <template <typename, typename > class Container>
void PmergeMe<Container>::run(std::string type)
{
	printInput();

	clock_t start = clock();



	recursiveSort(_groups);

	initMain();

	initPending();

	insertPending();

	if (_asRest)
	{
		size_t pos = binarySearch(_rest, _sorted.size());
		_sorted.insert(_sorted.begin() + pos, _rest);
	}

	clock_t end = clock();

	printSorted();

	double us = static_cast<double>(end - start) * 1000000.0 / CLOCKS_PER_SEC;

	std::cout << "Time to process a range of "
			  << _input.size()
			  << " elements with "
			  << type
			  << " : "
			  << us
			  << " us"
			  << std::endl;
}

template <template <typename, typename > class Container>
void PmergeMe<Container>::printInput(){
	std::cout << "Before: ";

	for (size_t i = 0; i < _input.size(); i++)
		std::cout << _input[i] << " ";

	std::cout << std::endl;
}

template <template <typename, typename > class Container>
void PmergeMe<Container>::printSorted(){
	std::cout << "After: ";

	for (size_t i = 0; i < _sorted.size(); i++)
		std::cout << _sorted[i] << " ";

	std::cout << std::endl;
}

template <template <typename, typename > class Container>
PmergeMe<Container>::~PmergeMe(){}
