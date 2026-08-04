#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <vector>
#include <deque>
#include <iostream>

class PmergeMe{
private:
private:
	int _rest;
	bool _asRest;
	unsigned int _groupSize;
	std::vector<unsigned int> _input;
	std::vector<unsigned int> _sorted;
	std::vector<std::vector<unsigned int> > _pending;
	std::vector<std::vector<unsigned int> > _groups;
public:
	PmergeMe();
	PmergeMe(const PmergeMe& copy);
	PmergeMe& operator=(const PmergeMe& other);

	//debug func
	void    DebugPrintContain();
	void	DebugPrintSorted();
	void	DebugPrintPending();

	//init
	void    initContainer(int ac, char **av);
	void    initGroups();
	void    initMain();
	void    initPending();
	// void	run();
	void    init(int ac, char **av);
	void sortGroups();

	void	recursiveSort(std::vector<std::vector <unsigned int> >& v);
	void	mergeGroups(std::vector<std::vector<unsigned int> >& v);
	size_t binarySearch(unsigned int value, size_t limit);
	std::vector<size_t> generateJacobsthalOrder(size_t size);
	size_t findPosition(unsigned int value);
	void	insertPending();
	void run();


	~PmergeMe();
};

#endif