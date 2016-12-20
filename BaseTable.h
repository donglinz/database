#pragma once
#include "Table.h"
class BaseTable
{
public:
	BaseTable();
	void open(string fileName);
	void markAsDelete(int nLineNumber);
	void runDelete();
	virtual Table getTable();
	Table m_table;
	std::shared_ptr<BaseTable> from;
	~BaseTable();
protected:
	bool isChanged;
};

