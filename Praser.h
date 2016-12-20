#pragma once
#include <string>
#include <cstring>
#include <cctype>
#include <vector>

class Praser
{
public:
	Praser();
	static std::string toLowerString(const std::string & t);
	static std::string trim(const std::string & t);
	static std::string mergeSpaces(const std::string & t);
	static int findFirstOf(const std::string & a, const std::string & b);
	static std::vector<std::string> resolveField(std::string fields);
	static std::vector<std::string> split(const std::string & a, char ch);
	~Praser();

private:
	const static std::exception ex_field_error;
};



