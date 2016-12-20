#include "Table.h"

Table::Table()
{
}



void Table::open(string fileName, int allowWrite)
{
	m_fileName = fileName;
	if (dbfFile.open(fileName, allowWrite) != 0) {
		std::cerr << "[ERROR] In Class Table 打开 DBF 文件失败" << std::endl;
	}
	m_fileHeader = dbfFile.getMyFileHeader();
	m_fieldDefinition.resize(dbfFile.GetNumFields());
	m_Records.resize(dbfFile.GetNumRecords());
	deleteMark.resize(dbfFile.GetNumRecords());
	loadRec(dbfFile);
}

void Table::prettyPrint()
{
}

void Table::close()
{
	dbfFile.close();
}

void Table::writeBackToDbf()
{
}

void Table::loadRec(DBF & dbfFile)
{
	for (int cnt = 0; cnt < m_fileHeader.uRecordsInFile; ++cnt) {
		if (dbfFile.loadRec(cnt) != 0) {
			std::cerr << "[ERROR] In Class Table 从DBF文件加载记录失败!" << std::endl;
		}

		m_Records[cnt].reset(new string[dbfFile.GetNumFields()]);

		
		for (int i = 0; i < dbfFile.GetNumFields(); ++i) {
			m_Records[cnt].get()[i] = Praser::trim(dbfFile.readField(i));
		}

	}
	dbfFile.close();
}

Table::~Table()
{
}
