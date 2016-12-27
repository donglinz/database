#pragma once
#include <string>
#include <cstring>
#include <cctype>
#include <vector>
#include <regex>
#include <sstream>
#include <set>
#include <iostream>
class Praser
{
public:
	Praser();
	~Praser();
	static std::string toLowerString(const std::string & t);
	static std::string trim(const std::string & t);
	static std::string mergeSpaces(const std::string & t);
	static int findFirstOf(const std::string & a, const std::string & b, int bpos = 0);
	static std::vector<std::string> resolveField(std::string fields);
	static std::vector<std::string> split(const std::string & a, char ch);
	
	static bool isLegalDate(std::string date);
	static std::string noQuotationMarks(const std::string & t);

	static int convertStringToInt(std::string arg);
	static double convertStringToDouble(std::string arg);
	static int convertDateToInt(std::string arg);

	static std::string convertNumberToString(double arg, bool removeBackDot = false);
private:
	const static std::exception ex_field_error;
	const static std::regex re_date;
	const static std::vector<int> days;
	const static std::set<char> legalOperator;
};



