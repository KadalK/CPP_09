#include "PmergeMe.hpp"
#include <climits>
#include <algorithm>
#include <ctime>

template <template <typename, typename > class Container>
PmergeMe<Container>::PmergeMe()
					: _asRest(false)
{}

template <template <typename, typename > class Container>
PmergeMe<Container>::PmergeMe(const PmergeMe& copy)
					: _rest(copy._rest),
					  _asRest(copy._asRest),
					  _input(copy._input),
					  _sorted(copy._sorted),
					  _stash(copy._stash),
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
		_sorted = other._sorted;
		_stash = other._stash;
		_groups = other._groups;
		_pending = other._pending;
	}
	return (*this);
}

template <template <typename, typename > class Container>
void PmergeMe<Container>::printamere(){
	std::cout << "_groups" << std::endl;
	for (size_t i = 0; i < _groups.size(); i++)
	{
		std::cout << _groups[i].front() << std::endl;
		std::cout << _groups[i].back() << std::endl;
	}
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

		// if (group[0] > group[1])
		// 	std::swap(group[0], group[1]);

		_groups.push_back(group);
	}

	if (_input.size() % 2 != 0)
	{
		_asRest = true;
		_rest.push_back(_input.back());
	}

	sortBigs(_groups);
}

//TO REWORK (adapter aux groupes de taille variable)
template <template <typename, typename > class Container>
void PmergeMe<Container>::initMain(){
	_sorted.clear();

	if (_groups.empty())
		return;

	for (size_t i = 0; i < _groups.size(); i++)
		_sorted.push_back(_groups[i][1]);

	_sorted.insert(_sorted.begin(), _groups[0][0]);
}

//TO REWORK (insertion de groupes au lieu de simples valeurs)
template <template <typename, typename > class Container>
void PmergeMe<Container>::initPending(){
	_pending.clear();

	for (size_t i = 1; i < _groups.size(); i++)
		_pending.push_back(_groups[i]);
}



// template <template <typename, typename > class Container>
// void PmergeMe<Container>::merge(Vec& left, Vec& right, Vec& result){
// 	Vec tmp;
//
// 	size_t i = 0;
// 	size_t j = 0;
//
// 	while (i < left.size() && j < right.size())
// 	{
// 		if (left[i] <= right[j])
// 			tmp.push_back(left[i++]);
// 		else
// 			tmp.push_back(right[j++]);
// 	}
//
// 	while (i < left.size())
// 		tmp.push_back(left[i++]);
//
// 	while (j < right.size())
// 		tmp.push_back(right[j++]);
//
// 	result = tmp;
// }


template <template <typename, typename > class Container>
Container<unsigned int, std::allocator<unsigned int> > PmergeMe<Container>::merge(Vec& left, Vec& right){
	Vec tmp;

	std::cout << "ICI\n";
	for (size_t i = 0; i < left.size(); i++)
	{
		tmp.push_back(left[i]);
	}
	for (size_t i = 0; i < right.size(); i++)
	{
		tmp.push_back(right[i]);
	}

	for (size_t i = 0; i < tmp.size(); i++)
	{
		std::cout << "tmp[i] : " << tmp[i] << '\n';
	}
	return tmp;
}

template <template <typename, typename > class Container>
void PmergeMe<Container>::splitGroups(Groups &g){
	Groups tmp;

	size_t pairSize = g[0].size() / 2;

	for (size_t i = 0; i < g.size(); i++)
	{
		Vec left;
		Vec right;

		for (size_t j = 0; j  < pairSize; j++)
		{
			left.push_back(g[i][j]);
			right.push_back(g[i][j + pairSize]);
		}
		tmp.push_back(left);
		tmp.push_back(right);
	}
	g = tmp;
}


//TO REWORK (delete ?)
template <template <typename, typename > class Container>
void PmergeMe<Container>::sortBigs(Groups& g){
	// printamere();
	// if (g.size() <= 3)
	// {
	// 	return;
	// }

	Groups tmp;
	size_t indexTmp = 0;

	for (size_t i = 0; i < g.size(); i+= 2)
	{
		if (i + 1 >= g.size())
		{
			// for (size_t j = 0; i < g.size() - _input.size(); i++)
			// {
			// 	_input[j]
			// }
			tmp.push_back(_groups[i]);
			_groups = tmp;
			return;
		}
		if (g[i].back() > g[i + 1].back())
		{
			std::swap(g[i], g[i + 1]);
		}
		std::cout << "g.size() = " << g.size() << "\n";
		std::cout << "_input.size() = " << _input.size()<< "\n";
		// if (g.begin()->size() * 2 < _input.size())
		// {
			// for (size_t i = 0; i < g.size() * 2 < _input.size(); i++)
			// {
			//
			// }
			Vec left = g[i];
			Vec right = g[i + 1];
			tmp.push_back(merge(left, right));
			indexTmp++;
		// }
		// else
		// {
		// 	// for (size_t i = )
		// 	_groups = tmp;
		// 	return;
		// }
		// indexTmp++
	}
	if (g.begin()->size() * 2 < _input.size())
		sortBigs(tmp);
	else
	{
		_groups = tmp;
		return ;
	}
}

// template <template <typename, typename > class Container>
// void PmergeMe<Container>::recursiveSortUp(Groups& v){
//
// }

//TO REWORK (A REFAIRE COMPLETEMENT)
template <template <typename, typename > class Container>
void PmergeMe<Container>::recursiveSort(Groups& v){
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

	size_t n1 = 1;
	size_t n2 = 3;

	while (n2 < size)
	{
		for (size_t i = n2; i > n1; i--)
			order.push_back(i - 1);

		size_t next = n2 + 2 * n1;

		n1 = n2;
		n2 = next;
	}

	for (size_t i = size; i > n1; i--)
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
		pos = binarySearch(small, limit);

		_sorted.insert(_sorted.begin() + pos, small);
	}
}

template <template <typename, typename > class Container>
void PmergeMe<Container>::init(int ac, char **av){
	initContainer(ac, av);
	initGroups();
	//debug
	// splitGroups(_groups);
}

template <template <typename, typename > class Container>
void PmergeMe<Container>::run(std::string type)
{
	printInput();

	clock_t start = clock();

	recursiveSort(_groups);

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
