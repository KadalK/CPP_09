#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <vector>
#include <deque>
#include <iostream>
#include <memory>

template < template <typename, typename> class Container >
class PmergeMe
{
private:

	typedef Container<unsigned int, std::allocator<unsigned int> > Vec;
	typedef Container<Vec, std::allocator<Vec> > Groups;

	Vec _rest;
	bool _asRest;

	Vec _input;
	Vec _sorted;
	Vec _stash;

	Groups _groups;
	Groups _main;
	Groups _pending;

public:

	PmergeMe();
	PmergeMe(const PmergeMe& copy);
	PmergeMe& operator=(const PmergeMe& other);

	void printGroups();
	void printRest();
	void printPending();
	// void printSorted();

	void initContainer(int ac, char **av);
	void initGroups();
	void initMain(Groups& g);
	void initPending(Groups& g);
	void init(int ac, char **av);

	// void recursiveDownBigs(Vec& v);
	void recursiveUp(Groups& g);
	void recursiveDown(Groups& g);
	Vec merge(Vec& left, Vec& right);
	Groups splitGroups(Groups& g);

	size_t binarySearch(unsigned int value, size_t limit);
	size_t binarySearchGroup(const Groups& groups, const Vec& block, size_t limit);
	std::vector<size_t> generateJacobsthalOrder(size_t size);
	size_t findPosition(unsigned int value);
	void insertPending();

	void run(std::string type);

	void printInput();
	void printSorted();

	~PmergeMe();
};

#include "PmergeMe.tpp"
#endif