#include "BaseTable.h"



BaseTable::BaseTable()
{
	isChanged = false;
}

void BaseTable::open(string fileName)
{
}

void BaseTable::markAsDelete(int nLineNumber)
{
}

void BaseTable::runDelete()
{
}

Table BaseTable::getTable()
{
	return m_table;
}

BaseTable::~BaseTable()
{
}
