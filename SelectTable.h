#pragma once
#include "BaseTable.h"
class SelectTable :
	public BaseTable
{
public:
	SelectTable();
	virtual Table getTable();
	std::string whrere;
	string groupby;
	std::vector<string> orderby;
	bool distinct;
	~SelectTable();
};

