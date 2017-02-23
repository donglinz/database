#include "Praser.h"

const std::exception Praser::ex_field_error = std::exception("[ERROR] 字段格式错误!");
const std::regex Praser::re_date = std::regex("[0-9]+-[0-9]+-[0-9]+");
const std::vector<int> Praser::days{ 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
const std::set<char> Praser::legalOperator{' ', '(', ')', '.', '+', '-', '*', '/', '&', '|', '=', '\'', '>', '<', '!', '_'};
Praser::Praser()
{
}

std::string Praser::toLowerString(const std::string & t)
{
	std::string ret;
	for (int i = 0; i < t.length(); ++i) {
		ret += tolower(t[i]);
	}
	return ret;
}

std::string Praser::trim(const std::string & t)
{
	int begin = 0;
	int end = t.length() - 1;
	while (begin < t.length() && t[begin] == ' ') ++begin;
	while (end >= 0 && t[end] == ' ') --end;
	if (begin > end) return "";
	return t.substr(begin, end - begin + 1);
}

std::string Praser::mergeSpaces(const std::string & t)
{
	std::string stander;
	for (int i = 0; i < t.length() ; ++i) {
		if (i != 0 && t[i] == ' ' && t[i - 1] == ' ') continue;
		stander += t[i];
	}
	return stander;
}

int Praser::findFirstOf(const std::string & a, const std::string & b, int bpos)
{
	for (int i = bpos; i < (int)a.length() - (int)b.length() + 1; ++i) {
		bool ok = true;
		for (int j = 0; j < b.length(); j++) {
			if (tolower(a[i + j]) != tolower(b[j])) {
				ok = false;
				break;
			}
		}
		if (ok == true) return i;
	}

	//没找到
	return -1;
}

std::vector<std::string> Praser::resolveField(std::string fields) throw(std::exception)
{
	std::vector<std::string> ret = split(fields, ',');
	for (int i = 0; i < ret.size(); ++i) {
		ret[i] = trim(ret[i]);
		if (ret[i] == "") throw ex_field_error;
		for (int j = 0; j < ret[i].size(); j++) {
			if (!isalnum(ret[i][j]) && !legalOperator.count(ret[i][j])) {
				std::cerr << "无法识别的字符 " << ret[i][j] << std::endl;
				throw ex_field_error;
			}
		}
	}
	return ret;
}

std::vector<std::string> Praser::split(const std::string & a, char ch)
{
	std::vector<std::string> ret;
	std::string t;
	for (int i = 0; i < a.length(); ++i) {
		if (a[i] == ch) {
			ret.push_back(trim(t));
			t = "";
			continue;
		}
		t += a[i];
	}
	if (t != "") ret.push_back(t);
	return ret;
}

std::vector<std::string> Praser::split(const std::string & a, std::set<char> se)
{
	std::vector<std::string> ret;
	std::string t;
	for (int i = 0; i < a.length(); ++i) {
		if (se.count(a[i])) {
			if(trim(t) != "")
				ret.push_back(trim(t));
			t = "";
			continue;
		}
		t += a[i];
	}
	if (t != "") ret.push_back(t);
	return ret;
}

Praser::~Praser()
{
}

bool Praser::isLegalDate(std::string date)
{
	date = trim(date);
	if (!std::regex_match(date, re_date)) return false;
	for (int i = 0; i < date.length(); ++i)
		if (date[i] == '-') date[i] = ' ';
	std::stringstream ss(date);
	int year, day, month;
	ss >> year;
	if (year < 0 || year > 9999) return false;
	ss >> month;
	if (month < 0 || month > 12) return false;
	ss >> day;
	if (day < 0 || day > days[month - 1]) return false;
	return true;
}

std::string Praser::noQuotationMarks(const std::string & t)
{
	std::string ret;
	for (char ch : t) if (ch != '\'') ret += ch;
	return ret;
}

int Praser::convertStringToInt(std::string arg)
{
	int ret = 0;
	if (arg == "") return 0;
	int i = 0;
	bool nag = false;
	if (arg[0] == '-') {
		++i;
		nag = true;
	}
	for (i = 0; i < arg.length(); ++i) ret = ret * 10 + arg[i] - '0';
	return nag ? -ret : ret;
}

double Praser::convertStringToDouble(std::string arg)
{
	double ret1 = 0;
	double ret2 = 0;
	int beg = 0;
	bool nag = false;
	if (arg[0] == '-') {
		nag = true;
		++beg;
	}

	int dotPos = -1;
	for (int i = 0; i < arg.length(); ++i) {
		if (arg[i] == '.') {
			dotPos = i;
			break;
		}
	}

	if (dotPos < 0) {
		return convertStringToInt(arg);
	}

	for (int i = (int)arg.length() - 1; i > dotPos; --i) {
		ret1 += arg[i] - '0';
		ret1 /= 10;
	}

	for (int i = beg; i < dotPos; ++i) {
		ret2 = ret2 * 10 + arg[i] - '0';
	}
	return nag ? -(ret1 + ret2) : (ret1 + ret2);
}

int Praser::convertDateToInt(std::string arg)
{
	int ret = 0;
	for (int i = 0; i < arg.length(); ++i) {
		ret = ret * 10 + arg[i] - '0';
	}
	return ret;
}

std::string Praser::convertNumberToString(double arg, bool removeBackDot)
{
	std::stringstream ss;
	ss << arg;
	std::string ret;
	ss >> ret;
	if (removeBackDot) {
		if (ret.length() >= 2 && ret.substr(ret.length() - 2, 2) == ".0") {
			ret.resize(ret.length() - 2);
		}
	}
	return ret;
}

