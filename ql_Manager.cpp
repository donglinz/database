#include "ql_Manager.h"



ql_Manager::ql_Manager()
{
}

void ql_Manager::run(string q_line)
{
	try {
		getStanderString(q_line);
	}
	catch (std::exception e) {
		std::cerr << e.what() << std::endl;
	}

	if (Praser::toLowerString(q_line.substr(0, 6)) == "select") {
		ql_select qsl(q_line);
		qsl.run();
	}
	else if (Praser::toLowerString(q_line.substr(0, 6)) == "create") {
	}
	else if (Praser::toLowerString(q_line.substr(0, 6)) == "insert") {
	}
	else if (Praser::toLowerString(q_line.substr(0, 6)) == "update") {
	}
	else if (Praser::toLowerString(q_line.substr(0, 6)) == "delete") {
	}
	else {
		std::cerr << q_line.substr(0, 6) << "¸½½üÓÐÓï·¨´íÎó" << std::endl;
	}


}

void ql_Manager::getStanderString(string & q_line) throw (std::exception)
{
	if (q_line == "" || q_line == ";") {
		throw ex_sql_empty;
	}
	
	q_line = Praser::trim(q_line);
	if (q_line == "") throw ex_sql_empty;

	q_line = Praser::mergeSpaces(q_line);
	if (q_line == "") throw ex_sql_empty;

	if (*q_line.rbegin() == ';') q_line.pop_back();
	if (q_line == "") throw ex_sql_empty;

}


ql_Manager::~ql_Manager()
{
}
