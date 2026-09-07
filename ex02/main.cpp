#include "PmergeMe.hpp"

int main(int ac, char **av)
{
	if (ac < 2)
	{
		std::cerr << "Error" << std::endl;
		return 1;
	}

	try
	{
		std::cout << "------STD::VECTOR------\n" << std::endl;

		clock_t start = clock();

		PmergeMe<std::vector> vectorSort;
		vectorSort.init(ac, av);
		vectorSort.run();

		clock_t end = clock();
		vectorSort.printClock(start, end, "std::vector");
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return 1;
	}


	try
	{

		std::cout << "\n\n------STD::DEQUE------\n" << std::endl;
		clock_t start = clock();

		PmergeMe<std::deque> dequeSort;
		dequeSort.init(ac, av);
		dequeSort.run();

		clock_t end = clock();
		dequeSort.printClock(start, end, "std::deque");
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return 1;
	}

	return 0;
}


// int main(int ac, char **av){
// 	PmergeMe<std::vector> vectorSort;
// 	vectorSort.init(ac, av);
// 	vectorSort.run("std::vector");
// }