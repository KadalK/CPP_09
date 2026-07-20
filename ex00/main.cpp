#include "BitcoinExchange.hpp"

int main(int ac, char ** av)
{
	if (ac != 2)
	{
		std::cerr << "Error: could not open file." << std::endl;
		return -1;
	}
	BitcoinExchange btc;
	if (btc.loadData("data.csv") == -1)
		return -1;
	btc.extractInput(av[1]);

	return (0);
}