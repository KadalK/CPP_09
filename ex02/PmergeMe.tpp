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
					  _groups(copy._groups),
					  _main(copy._main),
					  _pending(copy._pending)
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
		_main = other._main;
		_pending = other._pending;
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


//INIT
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
	// if (_input.size() % 2 != 0)
	// {
	// 	_rest.push_back(_input.back());
	// 	_asRest = true;
	// }
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
			_rest.push_back(_input.back());
			_asRest = true;
		}
	recursiveUp(_groups, 2);
}

template <template <typename, typename > class Container>
void PmergeMe<Container>::initMain(Groups& g){
	_main.clear();
	_stash.clear();

	if (g.empty())
		return;
	size_t pairSize = g[0].size();

	_main.push_back(g[0]);

	for (size_t i = 1; i < g.size(); i++)
	{
		if (pairSize != g[i].size())
			continue;
		if (i % 2 != 0)
			_main.push_back(g[i]);
	}
}

//TO REWORK (insertion de groupes au lieu de simples valeurs)
template <template <typename, typename > class Container>
void PmergeMe<Container>::initPending(Groups& g){
	_pending.clear();

	if (g.empty())
		return;
	size_t pairSize = g[0].size();

	for (size_t i = 1; i < g.size(); i++)
	{
		if (pairSize != g[i].size())
		{
			_stash.push_back(g[i]);
		}
		else if (i % 2 == 0)
            _pending.push_back(g[i]);
	}
}

template <template <typename, typename > class Container>
Container<unsigned int, std::allocator<unsigned int> > PmergeMe<Container>::merge(Vec& left, Vec& right){
	Vec tmp;

	for (size_t i = 0; i < left.size(); i++)
		tmp.push_back(left[i]);
	for (size_t i = 0; i < right.size(); i++)
		tmp.push_back(right[i]);

	return tmp;
}

static size_t sizingPair(size_t size){
	size_t n = 1;

	while (n * 2 <= size)
		n *= 2;

	return n;
}

template <template <typename, typename> class Container>
typename PmergeMe<Container>::Groups
PmergeMe<Container>::splitGroups(Groups& g, size_t pairSize){
	Groups tmp;
	size_t half = pairSize / 2;

	for (size_t i = 0; i < g.size(); i++)
	{
		if (g[i].size() == half)
		{
			tmp.push_back(g[i]);
			continue;
		}

		Vec left;
		Vec right;

		for (size_t j = 0; j < half; j++)
		{
			if (j < g[i].size())
				left.push_back(g[i][j]);
			if ((j + half) < g[i].size())
				right.push_back(g[i][j + half]);
		}

		if (!left.empty())
			tmp.push_back(left);
		if (!right.empty())
			tmp.push_back(right);

		for (size_t j = pairSize; j < g[i].size(); j++)
		{
			_asRest = true;
			_rest.push_back(g[i][j]);
			_stash.push_back(g[i]);
		}
	}

	// printWhatever(tmp);
	// std::cout << '\n';
	_groups = tmp;
	printGroups();
	// std::cout << '\n';
	return tmp;
}


//TO REWORK (delete ?)


template <template <typename, typename > class Container>
size_t PmergeMe<Container>::binarySearchGroup(const Groups& main, const Vec& pend, size_t limit){
	size_t left = 0;
	size_t right = limit;

	while ((right - left) > 1)
	{
		size_t mid = (left + right) / 2;
		mid += (left + right) % 2;

		if (main[mid].back() < pend.back())
			left = mid + 1;
		else if (main[mid].back() > pend.back())
			right = mid;
		else
			return mid;
	}

	if (left >= main.size())
		return left;

	if (main[left].back() > pend.back())
		return left;

	return left + 1;
}


template <template <typename, typename> class Container>
std::vector<size_t> PmergeMe<Container>::generateJacobsthalOrder(size_t size){
	std::vector<size_t> order;

	size_t n1 = 0; //changed  1 / 3 to 0 / 1
	size_t n2 = 1;
	size_t next;

	while (n2 < size)
	{
		next = (n1 * 2) + n2;

		n1 = n2;
		n2 = next;
        order.push_back(next);
	}
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
void PmergeMe<Container>::insertPending(Groups& g){
	if (g.empty())
		return;

	size_t pendSize = g.size();

	std::vector<size_t> order = generateJacobsthalOrder(pendSize + 2);
	order.insert(order.begin(), 0);

	std::vector<bool> inserted(pendSize, false);

	for (size_t i = 1; i < order.size(); i++)
	{
		size_t highBound = std::min(order[i], pendSize);
		size_t lowBound = order[i - 1];

		if (lowBound >= pendSize)
			break;

		for (size_t idx = highBound; idx > lowBound; idx--)
		{
			if (idx > pendSize || inserted[idx - 1])
				continue;

			size_t limit = _main.size();
			size_t pos = binarySearchGroup(_main, g[idx - 1], limit);
			_main.insert(_main.begin() + pos, g[idx - 1]);
			inserted[idx - 1] = true;
		}
	}
}

template <template <typename, typename > class Container>
void PmergeMe<Container>::init(int ac, char **av){
	initContainer(ac, av);
	initGroups();
}

template <template <typename, typename > class Container>
void PmergeMe<Container>::recursiveUp(Groups& g, size_t pairsize){
	Groups tmp;
	size_t i = 0;
	for (; i < g.size(); i+= 2)
	{
		if (i + 1 >= g.size())
			break;
		if ((g[i].back() > g[i + 1].back())
			&& (g[i].size() == pairsize && g[i + 1].size() == pairsize))
		{
			std::swap(g[i], g[i + 1]);
		}

		Vec left = g[i];
		Vec right = g[i + 1];
		tmp.push_back(merge(left, right));

	}

	while (i < g.size())
	{
		tmp.push_back(g[i]);
		i++;
	}

	if (tmp[0].size() * 2 <= _input.size())
	{
		recursiveUp(tmp, pairsize * 2);
		return;
	}
	_groups = tmp;
}

template <template <typename, typename> class Container>
void PmergeMe<Container>::recursiveDown(Groups& g, size_t pairSize){
	if (g.empty() || pairSize <= 1)
		return;

	Groups tmp = splitGroups(g, pairSize);

	initMain(tmp);
	initPending(tmp);
	insertPending(_pending);

	if (_asRest && !_rest.empty())
	{
		Vec single;
		single.push_back(_rest.back());
		size_t pos = binarySearchGroup(_main, single, _main.size());
		_main.insert(_main.begin() + pos, single);
		_rest.pop_back();
		if (_rest.empty())
			_asRest = false;
	}

	_groups.clear();
	_groups = _main;
	for (size_t i = 0; i < _stash.size(); i++)
		_groups.push_back(_stash[i]);

	recursiveDown(_groups, pairSize / 2);
}

template <template <typename, typename > class Container>
void PmergeMe<Container>::run(std::string type){
	printInput();

	clock_t start = clock();

	size_t pairSize = sizingPair(_input.size());

	//std::cout << "pairSize: " << pairSize << std::endl;

	recursiveDown(_groups, pairSize);

	printSorted();

	clock_t end = clock();


	double us = static_cast<double>(end - start)
		* 1000000.0 / CLOCKS_PER_SEC;

	std::cout << "Time to process a range of "
			  << _input.size()
			  << " elements with "
			  << type
			  << " : "
			  << us
			  << " us"
			  << std::endl;
}


//print
template <template <typename, typename > class Container>
void PmergeMe<Container>::printInput(){
	std::cout << "Before: ";

	for (size_t i = 0; i < _input.size(); i++)
		std::cout << _input[i] << " ";

	std::cout << std::endl;
}


template <template <typename, typename > class Container>
void PmergeMe<Container>::printRest(){
	std::cout << "_rest: " ;
	for (size_t i = 0; i < _rest.size(); i++)
	{
		std::cout << _rest[i] << " ";
	}
	std::cout << std::endl;
}

template <template <typename, typename > class Container>
void PmergeMe<Container>::printGroups(){
	std::cout << "_groups: ";
	for (size_t i = 0; i < _groups.size(); i++)
	{
		std::cout << "Groups[" << i << "]: ";
		for (size_t j = 0; j < _groups[i].size(); j++)
		{
			std::cout << _groups[i][j] << " ";

		}
	}
	std::cout << std::endl;
}

template <template <typename, typename > class Container>
void PmergeMe<Container>::printWhatever(Groups &g){
	std::cout << "g: ";
	for (size_t i = 0; i < g.size(); i++)
	{
		std::cout << "g[" << i << "]: ";
		for (size_t j = 0; j < g[i].size(); j++)
		{
			std::cout << g[i][j] << " ";

		}
	}
	std::cout << std::endl;
}

template <template <typename, typename > class Container>
void PmergeMe<Container>::printMain(){
	std::cout << "Main: ";

	for (size_t i = 0; i < _main.size(); i++)
	{
		for (size_t j = 0; j < _main[i].size(); j++)
		std::cout << _main[i][j] << " ";
	}

	std::cout << std::endl;
}

template <template <typename, typename > class Container>
void PmergeMe<Container>::printPending(){
	std::cout << "Pending: ";

	for (size_t i = 0; i < _pending.size(); i++)
	{
		for (size_t j = 0; j < _pending[i].size(); j++)
			std::cout << _pending[i][j] << " ";
	}

	std::cout << std::endl;
}


template <template <typename, typename > class Container>
void PmergeMe<Container>::printSorted()
{
	std::cout << "After: ";

	for (size_t i = 0; i < _groups.size(); i++)
	{
		for (size_t j = 0; j < _groups[i].size(); j++)
			std::cout << _groups[i][j] << " ";
	}

	std::cout << std::endl;
}

template <template <typename, typename > class Container>
PmergeMe<Container>::~PmergeMe(){}
