#include "PmergeMe.hpp"

// int main(int ac, char **av)
// {
// 	if (ac < 2)
// 	{
// 		std::cerr << "Error" << std::endl;
// 		return 1;
// 	}
//
// 	try
// 	{
// 		PmergeMe<std::vector> vectorSort;
//
// 		vectorSort.init(ac, av);
// 		vectorSort.run("std::vector");
//
//
// 		PmergeMe<std::deque> dequeSort;
//
// 		dequeSort.init(ac, av);
// 		dequeSort.run("std::deque");
// 	}
// 	catch (std::exception& e)
// 	{
// 		std::cerr << e.what() << std::endl;
// 		return 1;
// 	}
//
// 	return 0;
// }

int main(int ac, char **av){
	PmergeMe<std::vector> vectorSort;
	vectorSort.init(ac, av);
	vectorSort.run("std::vector");

}