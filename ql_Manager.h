#pragma once
#include <exception>
#include "ql_select.h"
#include "ql_create.h"
#include "ql_insert.h"
#include "ql_update.h"
#include "ql_delete.h"

class ql_Manager
{
public:
	ql_Manager();
	void run(string q_line);
	void getStanderString(string & q_line);
	~ql_Manager();
private:
	const std::exception ex_sql_empty = std::exception("[ERROR] sqlÓï¾äÎª¿Õ!");
};

