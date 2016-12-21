#pragma once
#include "Praser.h"
#include "dbf\dbf.h"
#include "cparse\shunting-yard.h"
#include <memory>
#include <iomanip>
#include <algorithm>
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
private:
	DBF m_dbfFile;
	void loadRec(DBF & dbfFile);
	int fieldPos(const string& field);

	struct Date {
		int year, month, day;
		Date(int year, int month, int day) : year(year), month(month), day(day) {};
		bool operator < (const Date& ths) const {
			if (year < ths.year ||
				(year == ths.year && month < ths.month) ||
				(year == ths.year && month == ths.month && day < ths.day)) return true;
			return false;
		}
	};
	int convertStringToInt(string arg);
	double convertStringToDouble(string arg);
	int convertDateToInt(string arg);
	string m_fileName;
	fileHeader m_fileHeader;
	std::vector<fieldDefinition> m_fieldDefinition;
	std::vector<std::shared_ptr<string> > m_Records;
	std::vector<bool> deleteMark;

	const string ex_where_field_error = "[ERROR] WHERE ²ÎÊý´íÎó!";
};

