#pragma once

#include <memory>
#include <iomanip>
#include <algorithm>

#include "Praser.h"
#include "dbf\dbf.h"
#include "cparse\shunting-yard.h"

#define GET_STRING(m_Records, r, c) (*m_Records[r])[c] 

class Table
{
public:
	Table();
	~Table();
	

	void open(string fileName, int allowWrite = false);
	void prettyPrint();
	void close();
	void writeBackToDbf();
	int isMatch(int nRec, string expr, bool & ans);
	int numRecord();
	int numFields();
	void deleteRecord(int nRec);
	int sort(std::vector<string> v_coindition);
	int fieldPos(const string& field);

	// 用于支持聚合函数
	int countMax(int nField, int r1, int r2, double & ans);
	int countMin(int nField, int r1, int r2, double & ans);
	int countSum(int nField, int r1, int r2, double & ans);
	int countAve(int nField, int r1, int r2, double & ans);
	int countNum(int nField, int r1, int r2, double & ans);

	int getRecord(int r, int c, string & ans);

	char getFieldType(int nField);

	void setRecords(std::vector<std::shared_ptr<std::vector<string> > > vec, 
		std::vector<fieldDefinition> fds);
	std::vector<fieldDefinition> getFieldDefinition(std::vector<string> v_fields);

private:
	DBF m_dbfFile;
	void loadRec(DBF & dbfFile);
	

	int convertStringToInt(string arg);
	double convertStringToDouble(string arg);
	int convertDateToInt(string arg);
	


	string m_fileName;
	fileHeader m_fileHeader;
	std::vector<fieldDefinition> m_fieldDefinition;
	std::vector<std::shared_ptr<std::vector<string> > > m_Records;
	std::vector<bool> deleteMark;

	const string ex_where_field_error = "[ERROR] WHERE 参数错误!";
};

