#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <vector>
#include <deque>
#include <iostream>
#include <memory>
#include <climits>
#include <algorithm>
#include <ctime>

template < template <typename, typename> class Container >
class PmergeMe
{
private:

	typedef Container<unsigned int, std::allocator<unsigned int> > Vec;
	typedef Container<Vec, std::allocator<Vec> > Groups;

	bool _asRest;
	Vec _rest;

	Vec _input;
	Vec _sorted;

	Groups _stash;
	Groups _groups;
	Groups _main;
	Groups _pending;

public:

	PmergeMe();
	PmergeMe(const PmergeMe& copy);
	PmergeMe& operator=(const PmergeMe& other);


	void initContainer(int ac, char **av);
	void initGroups();
	void initMain(Groups& g);
	void initPending(Groups& g);
	void init(int ac, char **av);

	void recursiveUp(Groups& g, size_t pairsize);
	void recursiveDown(Groups& g, size_t pairSize);
	Vec merge(Vec& left, Vec& right);
	Groups splitGroups(Groups& g, size_t pairSize);

	size_t binarySearchGroup(const Groups& groups, const Vec& block, size_t limit);
	std::vector<size_t> generateJacobsthalOrder(size_t size);
	size_t findPosition(unsigned int value);
	void insertPending(Groups &g);

	void run();

	void    printInput();
	void    printRest();
	void    printGroups();
	void    printPending();
	void    printMain();
	void    printSorted();
	void    printClock(clock_t start, clock_t end, std::string type);
	void	printWhatever(Groups &g);

	~PmergeMe();
};

#include "PmergeMe.tpp"
#endif