#ifndef BITCOINEXCHANGE_HPP
#define BITCOINEXCHANGE_HPP

#include <map>
#include <iostream>

class BitcoinExchange{
private:
	std::map<std::string, double> _content;
public:
	BitcoinExchange();
	BitcoinExchange(const BitcoinExchange& copy);
	BitcoinExchange& operator=(const BitcoinExchange& other);

	int	loadData(std::string file);
	void	extractInput(std::string file);

	~BitcoinExchange();
};




#endif
