#include "Table.h"

Table::Table()
{
}



void Table::open(string fileName, int allowWrite)
{
	m_fileName = fileName;
	if (m_dbfFile.open(fileName, allowWrite) != 0) {
		std::cerr << "[ERROR] In Class Table 打开 DBF 文件失败" << std::endl;
	}
	m_fileHeader = m_dbfFile.getMyFileHeader();
	m_fieldDefinition = m_dbfFile.getMyFieldDefinitions();
	m_Records.resize(m_dbfFile.GetNumRecords());
	deleteMark.resize(m_dbfFile.GetNumRecords());
	loadRec(m_dbfFile);
}

void Table::prettyPrint()
{
	std::vector<int> maxlen;
	maxlen.resize(numFields());
	for (int nField = 0; nField < numFields(); ++nField) {
		for (int nRec = 0; nRec < numRecord(); ++nRec) {
			maxlen[nField] = std::max(maxlen[nField], (int)m_Records[nRec].get()[nField].length());
		}
	}
	int sumlen = 1 + numFields();
	for (int len : maxlen) sumlen += len;
	for (int i = 0; i < sumlen; i++) std::cout << '-';
	std::cout << std::endl;

	for (int nField = 0; nField < numFields(); ++nField) {
		std::cout << "|" << std::setfill(' ') << std::setw(maxlen[nField]) << m_fieldDefinition[nField].cFieldName;
	}
	std::cout << "|" << std::endl;

	for (int nRec = 0; nRec < numRecord(); ++nRec) {
		for (int nField = 0; nField < numFields(); ++nField) {
			std::cout << "|" << std::setfill(' ') << std::setw(maxlen[nField]) << m_Records[nRec].get()[nField];
		}
		std::cout << "|" << std::endl;
	}
	for (int i = 0; i < sumlen; i++) std::cout << '-';
	std::cout << std::endl;
}

void Table::close()
{
	m_dbfFile.close();
}

void Table::writeBackToDbf()
{

}

int Table::isMatch(int nRec, string expr, bool & ans)
{
	expr = Praser::toLowerString(Praser::trim(expr));
	int numBraket = 0;
	TokenMap vars;
	calculator calu;
	calu.compile(expr.c_str());
	for (int cntField = 0; cntField < m_fieldDefinition.size(); ++cntField) {
		fieldDefinition & field = m_fieldDefinition[cntField];
		//vars[field.cFieldName] = 10;

		// N I B D作为值类型处理 其余当作字符串处理(只能比较相等或不相等)
		if (field.cFieldType == 'N' || field.cFieldType == 'I') {
			vars[field.cFieldName] = convertStringToInt(m_Records[nRec].get()[cntField]);
		}
		else if (field.cFieldType == 'B') {
			vars[field.cFieldName] = convertStringToDouble(m_Records[nRec].get()[cntField]);
		}
		else if (field.cFieldType == 'D') {
			vars[field.cFieldName] = convertDateToInt(m_Records[nRec].get()[cntField]);
		}
		else vars[field.cFieldName] = m_Records[nRec].get()[cntField];
	}
	if (calu.eval(vars) == "0") ans = false;
	else ans = true;
	return 0;
}

int Table::numRecord()
{
	return m_Records.size();
}

int Table::numFields()
{
	return m_fieldDefinition.size();
}

void Table::deleteRecord(int nRec)
{
	m_fileHeader.uRecordsInFile--;
	m_fieldDefinition.erase(m_fieldDefinition.begin() + nRec);
	m_Records.erase(m_Records.begin() + nRec);
}

int Table::fieldPos(const string & field)
{
	for (int cnt = 0; cnt < m_fieldDefinition.size(); ++cnt) {
		if (field.length() == strlen(m_fieldDefinition[cnt].cFieldName) &&
			Praser::findFirstOf(field, string(m_fieldDefinition[cnt].cFieldName)) == 0) return cnt;
	}
	return -1;
}

int Table::convertStringToInt(string arg)
{
	int ret = 0;
	if (arg == "") return 0;
	int i = 0;
	bool nag = false;
	if (arg[0] == '-') {
		++i;
		nag = true;
	}
	for (i = 0; i < arg.length(); ++i) ret = ret * 10 + arg[i] - '0';
	return nag?-ret:ret;
}

double Table::convertStringToDouble(string arg)
{
	double ret1 = 0;
	double ret2 = 0;
	int beg = 0;
	bool nag = false;
	if (arg[0] == '-') {
		nag = true;
		++beg;
	}

	int dotPos;
	for (int i = 0; i < arg.length(); ++i) {
		if (arg[i] == '.') {
			dotPos = i;
			break;
		}
	}
	
	for (int i = (int)arg.length() - 1; i > dotPos; --i) {
		ret1 += arg[i] - '0';
		ret1 /= 10;
	}

	for (int i = beg; i < dotPos; ++i) {
		ret2 = ret2 * 10 + arg[i] - '0';
	}
	return nag ? -(ret1 + ret2) : (ret1 + ret2);
}

int Table::convertDateToInt(string arg)
{
	int ret = 0;
	for (int i = 0; i < arg.length(); ++i) {
		ret = ret * 10 + arg[i] - '0';
	}
	return ret;
}

// 字符串类型，布尔类型两遍要加上''!!!
void Table::loadRec(DBF & dbfFile)
{
	for (int cnt = 0; cnt < m_fileHeader.uRecordsInFile; ++cnt) {
		if (dbfFile.loadRec(cnt) != 0) {
			std::cerr << "[ERROR] In Class Table 从DBF文件加载记录失败!" << std::endl;
		}

		m_Records[cnt].reset(new string[dbfFile.GetNumFields()]);

		
		for (int i = 0; i < dbfFile.GetNumFields(); ++i) {
			m_Records[cnt].get()[i] = Praser::trim(dbfFile.readField(i));
			char cType = m_fieldDefinition[i].cFieldType;
			if (cType == 'L' || cType == 'C') {
				m_Records[cnt].get()[i] = "'" + m_Records[cnt].get()[i] + "'";
			}
		}
	}
	dbfFile.close();
}



Table::~Table()
{
}
