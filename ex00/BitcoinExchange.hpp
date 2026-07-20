#ifndef BITCOINEXCHANGE_HPP
#define BITCOINEXCHANGE_HPP

#include <map>
#include <iostream>
#include <exception>

class BitcoinExchange{
private:
	std::map<std::string, double> _content;
public:
	BitcoinExchange();
	BitcoinExchange(const BitcoinExchange& copy);
	BitcoinExchange(std::map<std::string, double> content);
	BitcoinExchange& operator=(const BitcoinExchange& other);

	void	extractInput(std::string file);
	void	loadData(std::string file);

	~BitcoinExchange();
};




#endif
