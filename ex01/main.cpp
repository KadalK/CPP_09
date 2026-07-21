#include "RPN.hpp"

int main (int ac, char **av){

	if (ac != 2)
		return -1;
	try
	{
		RPN r;
		r.initStack(av[1]);
		// r.calculator();
		r.printResult();
	}catch (std::exception &ex)
	{
		std::cout << ex.what() << std::endl;
	}
	return (0);
}