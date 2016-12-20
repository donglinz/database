#pragma once
#include "Praser.h"
#include "dbf\dbf.h"
#include <memory>
class Table
{
public:
	Table();
	~Table();
	string m_fileName;
	fileHeader m_fileHeader;
	std::vector<fieldDefinition> m_fieldDefinition;
	std::vector<std::shared_ptr<string> > m_Records;
	std::vector<bool> deleteMark;
	string m_sFileName;
	void open(string fileName, int allowWrite = false);
	void prettyPrint();
	void close();
	void writeBackToDbf();
private:
	DBF dbfFile;
	void loadRec(DBF & dbfFile);
	
};

