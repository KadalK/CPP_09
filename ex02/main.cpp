#include "PmergeMe.hpp"

int main(int ac, char **av){
	if (ac < 2)
		return -1;

	PmergeMe p;

	p.init(ac, av);
	p.DebugPrintContain();
	p.DebugPrintSorted();
	p.DebugPrintPending();
}