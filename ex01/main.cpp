#include "RPN.hpp"

int main (int ac, char **av){
	(void)ac;
	RPN r;
	r.initStack(av[1]);
	r.calculator();
	r.printResult();
	return (0);
}