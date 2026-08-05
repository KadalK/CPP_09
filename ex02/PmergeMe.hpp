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

	int _rest;
	bool _asRest;

	Vec _input;
	Vec _sorted;

	Groups _groups;
	Groups _pending;

public:

	PmergeMe();
	PmergeMe(const PmergeMe& copy);
	PmergeMe& operator=(const PmergeMe& other);

	void printInput();
	void printSorted();

	void initContainer(int ac, char **av);
	void initGroups();
	void initMain();
	void initPending();

	void init(int ac, char **av);

	void sortGroups();

	void recursiveSortBigs(Vec& v);
	void recursiveSort(Groups& v);
	void merge(Vec& left, Vec& right, Vec& result);
	void mergeGroups(Groups& v);

	size_t binarySearch(unsigned int value, size_t limit);

	std::vector<size_t> generateJacobsthalOrder(size_t size);

	size_t findPosition(unsigned int value);

	void insertPending();

	void run(std::string type);

	~PmergeMe();
};

#include "PmergeMe.tpp"
#endif