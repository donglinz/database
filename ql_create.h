#pragma once

#include <sstream>

#include "Table.h"
#include "Praser.h"


class ql_create
{
public:
	ql_create(string q);
	~ql_create();
	string q_line;
	void run();

private:
	string fileName;
	const string type_int = "int";
	const string type_double = "double";
	const string type_date = "date";
	const string type_char = "char";
	const string type_bool = "bool";
	const string type_varchar = "varchar";
};

