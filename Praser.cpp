#include "Praser.h"

const std::exception Praser::ex_field_error = std::exception("[ERROR] �ֶθ�ʽ����");

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

int Praser::findFirstOf(const std::string & a, const std::string & b)
{
	for (int i = 0; i < (int)a.length() - (int)b.length() + 1; ++i) {
		bool ok = true;
		for (int j = 0; j < b.length(); j++) {
			if (a[i + j] != b[j] && a[i + j] != tolower(b[j])) {
				ok = false;
				break;
			}
		}
		if (ok == true) return i;
	}

	//û�ҵ�
	return -1;
}

std::vector<std::string> Praser::resolveField(std::string fields) throw(std::exception)
{
	std::vector<std::string> ret = split(fields, ',');
	for (int i = 0; i < ret.size(); ++i) {
		ret[i] = trim(ret[i]);
		if (ret[i] == "") throw ex_field_error;
		for (int j = 0; j < ret[i].size(); j++) {
			if (!isalnum(ret[i][j]) && ret[i][j] != '(' && ret[i][j] != ')') throw ex_field_error;
		}
	}
	return ret;
}

std::vector<std::string> Praser::split(const std::string & a, char ch)
{
	std::vector<std::string> ret;
	std::string t;
	for (int i = 0; i < a.length(); ++i) {
		if (a[i] == ',') {
			ret.push_back(trim(t));
			t = "";
			continue;
		}
		t += a[i];
	}
	return ret;
}

Praser::~Praser()
{
}