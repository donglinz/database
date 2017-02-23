#pragma once

#include "dbf\dbf.h"
#include "Praser.h"
#include "Table.h"
class ql_update
{
public:
	ql_update(string q);
	~ql_update();
	void run();
	string q_line;
	string fileName;
private:
	const string where = "where";
};

