#include "PmergeMe.hpp"

#include <iostream>

int main(int ac, char **av)
{
	if (ac < 2)
	{
		std::cout << "Usage: ./PmergeMe numbers..." << std::endl;
		return 1;
	}

	PmergeMe pmerge;

	pmerge.init(ac, av);
	pmerge.run();

	return 0;
}
