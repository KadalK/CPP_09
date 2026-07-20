#include "BitcoinExchange.hpp"
#include <fstream>
#include <algorithm>
#include <cctype>
#include <ctime>

BitcoinExchange::BitcoinExchange(){}

BitcoinExchange::BitcoinExchange(const BitcoinExchange& copy) : _content(copy._content){}

BitcoinExchange::BitcoinExchange(std::map<std::string, double> content) : _content(content) {}

BitcoinExchange& BitcoinExchange::operator=(const BitcoinExchange& other){
	if (this != &other)
		this->_content = other._content;
	return (*this);
}

void BitcoinExchange::loadData(std::string file)
{
	std::ifstream fs(file.c_str());

	if (!fs.is_open())
	{
		std::cerr << "Error: could not open file" << std::endl;
		return;
	}

	std::string line;
	while (std::getline(fs, line))
	{
		size_t pos = line.find(',');

		if (pos == std::string::npos)
			continue;

		std::string date = line.substr(0, pos);
		std::string value = line.substr(pos + 1);

		if (date == "date")
			continue;

		double money = std::strtod(value.c_str(), NULL);
		_content.insert(std::make_pair(date, money));
	}
}

static std::string trim(std::string str){
	size_t start = str.find_first_not_of(" \t");
	size_t end = str.find_last_not_of(" \t");

	if (start == std::string::npos)
		return "";

	return str.substr(start, end - start + 1);
}

static bool validDate(std::string str){
	if (str.length() != 10)
		return false;
	if (str[4] != '-' || str[7] != '-')
		return false;
	for (size_t i = 0; i < str.length(); i++)
	{
		if (i == 4 || i == 7)
			continue;

		if (!std::isdigit(str[i]))
			return false;
	}
	int year = std::atoi(str.substr(0,4).c_str());
	int month = std::atoi(str.substr(5,2).c_str());
	int day = std::atoi(str.substr(8,2).c_str());

	struct tm date = {};
	// y - 1900 a cause du format convention de tm
	date.tm_year = year - 1900;
	date.tm_mon = month - 1;
	date.tm_mday = day;

	std::mktime(&date);

	if (date.tm_year != year - 1900 || date.tm_mon != month - 1 || date.tm_mday != day)
		return false;

	return true;
}

void	BitcoinExchange::extractInput(std::string file){
	std::ifstream fs(file.c_str());

	if (!fs.is_open())
	{
		std::cerr << "Error: could not open file" << std::endl;
		return;
	}

	std::string line;

	while (std::getline(fs, line))
	{
		size_t pos = line.find('|');

		if (pos == std::string::npos)
		{
			std::cout << "Error: bad input => " << line << std::endl;
			continue;
		}

		std::string inputDate = trim(line.substr(0, pos));
		std::string inputValue = trim(line.substr(pos + 1));

		if (inputDate == "date")
			continue;

		if (!validDate(inputDate))
		{
			std::cerr << "Error: date is invalid." << std::endl;
			continue;
		}

		char *end;
		double value = std::strtod(inputValue.c_str(), &end);

		if (inputValue.empty() || *end != '\0')
		{
			std::cerr << "Error: bad input => " << line << std::endl;
			continue;
		}

		if (value < 0)
		{
			std::cerr << "Error: not a positive number." << std::endl;
			continue;
		}
		if (value > 1000)
		{
			std::cerr << "Error: too large a number." << std::endl;
			continue;
		}

		std::map<std::string, double>::iterator it;

		it = _content.lower_bound(inputDate);

		if (it == _content.end() || it->first != inputDate)
		{
			if (it == _content.begin())
			{
				std::cerr << "Error: date too early." << std::endl;
				continue;
			}
			--it;
		}

		double result = value * it->second;

		std::cout << inputDate << " => " << value << " = " << result << std::endl;
	}
}

BitcoinExchange::~BitcoinExchange(){}


