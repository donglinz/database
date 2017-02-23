#pragma once

#include <memory>

#include "dbf\dbf.h"
#include "Praser.h"

class ql_insert
{
public:
	ql_insert(string q);
	~ql_insert();
	void run();
	string q_line;

	string fileName;

private:
	const string into = "into";
	const string values = "values";
};

