#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <vector>
#include <deque>
#include <iostream>

struct Pair{
	unsigned int small;
	unsigned int big;
};

class PmergeMe{
private:
	int _rest;
	bool _asRest;
	std::vector<unsigned int> _input;
	std::vector<Pair> _pairs;
	std::vector<unsigned int> _pending;
	std::vector<unsigned int> _sorted;
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
	void    initPairs();
	void    initSorted();
	void    initPending();
	void    init(int ac, char **av);

	void    sortBigPairs();

	~PmergeMe();
};

#endif