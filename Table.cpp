#include "Table.h"

Table::Table()
{
}



void Table::open(string fileName, int allowWrite)
{
	m_fileName = fileName;
	if (m_dbfFile.open(fileName, allowWrite) != 0) {
		std::cerr << "[ERROR] In Class Table 打开 DBF 文件失败" << std::endl;
		return;
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
	for (int nField = 0; nField < numFields(); ++nField) 
		maxlen.push_back(strlen(m_fieldDefinition[nField].cFieldName));
	for (int nField = 0; nField < numFields(); ++nField) {
		for (int nRec = 0; nRec < numRecord(); ++nRec) {
			maxlen[nField] = std::max(maxlen[nField], (int)(*m_Records[nRec])[nField].length());
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
			std::cout << "|" << std::setfill(' ') << std::setw(maxlen[nField]) << (*m_Records[nRec])[nField];
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
		string tmo = Praser::toLowerString(field.cFieldName);
		string tmpcheck = (*m_Records[nRec])[cntField];
		// N I B D作为值类型处理 其余当作字符串处理(只能比较相等或不相等)
		if (field.cFieldType == 'N' || field.cFieldType == 'I') {
			vars[Praser::toLowerString(field.cFieldName)] = convertStringToInt((*m_Records[nRec])[cntField]);
		}
		else if (field.cFieldType == 'F') {
			vars[Praser::toLowerString(field.cFieldName)] = convertStringToDouble((*m_Records[nRec])[cntField]);
		}
		else if (field.cFieldType == 'D') {
			string date = (*m_Records[nRec])[cntField];
			date = date.substr(0, 4) + "-" + date.substr(4, 2) + "-" + date.substr(6, 2);
			vars[Praser::toLowerString(field.cFieldName)] = date;
		}
		else vars[Praser::toLowerString(field.cFieldName)] = Praser::toLowerString((*m_Records[nRec])[cntField]);
	}
	string res = calu.eval(vars).str();
	res = calu.eval(vars).str();
	if (calu.eval(vars).str() == "0") ans = false;
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
	deleteMark.erase(deleteMark.begin() + nRec);
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
	return Praser::convertStringToInt(arg);
}

double Table::convertStringToDouble(string arg)
{
	return Praser::convertStringToDouble(arg);
}

int Table::convertDateToInt(string arg)
{
	return Praser::convertDateToInt(arg);
}

std::vector<fieldDefinition> Table::getFieldDefinition(std::vector<string> v_fields)
{
	std::vector<fieldDefinition> ret;
	for (string & field : v_fields) {
		if (field == "*") ret.push_back(TypeInteger());
		else ret.push_back(m_fieldDefinition[fieldPos(field)]);
	}
	return ret;
}


int Table::getRecord(int r, int c, string & ans)
{
	if (r < 0 || r > m_Records.size() || c < 0 || c > (*m_Records[r]).size()) {
		ans = "[ERROR] out of range!";
		return -1;
	}
	ans = (*m_Records[r])[c];
	return 0;
}

char Table::getFieldType(int nField)
{
	return m_fieldDefinition[nField].cFieldType;
}


// 这个地方对fileheader没有完全修改，应该也没有需要完全修改
void Table::setRecords(std::vector<std::shared_ptr<std::vector<string> > > vec,
	std::vector<fieldDefinition> fds
)
{
	m_Records = vec;
	m_fieldDefinition.clear();
	deleteMark.clear();
	int pos = 0;
	m_fieldDefinition = fds;

	m_fileHeader.uRecordsInFile = vec.size();
	m_fileHeader.uRecordLength = 1;
	for (fieldDefinition fd : m_fieldDefinition) {
		m_fileHeader.uRecordLength += fd.uLength;
	}
	deleteMark.resize(m_Records.size());
}


void Table::loadRec(DBF & dbfFile)
{
	for (int cnt = 0; cnt < m_fileHeader.uRecordsInFile; ++cnt) {
		if (dbfFile.loadRec(cnt) != 0) {
			std::cerr << "[ERROR] In Class Table 从DBF文件加载记录失败!" << std::endl;
		}

		m_Records[cnt].reset(new std::vector<string>(dbfFile.GetNumFields(), ""));

		
		for (int i = 0; i < dbfFile.GetNumFields(); ++i) {
			string tmp = Praser::trim(dbfFile.readField(i));
			(*m_Records[cnt])[i] = tmp;
		}
	}
	dbfFile.close();
}

int Table::sort(std::vector<string> v_coindition) {
	if (v_coindition[0] == "") return 0;
	struct sortArg{
		sortArg(int id = 0, bool d = false) : idField(id), desc(d) {}
		int idField;
		bool desc;
	};
	const string desc = "desc";
	std::vector<sortArg> arg;
	
	for (string con : v_coindition) {
		sortArg sa;
		if (con.length() > desc.length() && con.substr(con.length() - desc.length(), desc.length()) == desc) {
			sa.desc = true;
			con.resize(con.length() - 4);
			con = Praser::trim(con);
		}
		sa.idField = fieldPos(con);
		arg.push_back(sa);
	}

	auto cmp = [&](const std::shared_ptr<std::vector<string> >& a, 
		const std::shared_ptr<std::vector<string> > & b)->bool {
		for (sortArg sa : arg) {
			if ((*a)[sa.idField] == (*b)[sa.idField]) continue;
			char cType = m_fieldDefinition[sa.idField].cFieldType;
			if (cType == 'I' || cType == 'N') {
				return sa.desc?
					convertStringToInt((*a)[sa.idField]) > convertStringToInt((*b)[sa.idField]):
					convertStringToInt((*a)[sa.idField]) < convertStringToInt((*b)[sa.idField]);
			}
			else if (cType == 'F') {
				return sa.desc?
					convertStringToDouble((*a)[sa.idField]) > convertStringToDouble((*b)[sa.idField]):
					convertStringToDouble((*a)[sa.idField]) < convertStringToDouble((*b)[sa.idField]);
			}
			else {
				return sa.desc? 
					(*a)[sa.idField] > (*b)[sa.idField]:
					(*a)[sa.idField] < (*b)[sa.idField];
			}
		}
		return false;
	};
	std::sort(m_Records.begin(), m_Records.end(), cmp);
	return 0;
}

int Table::countMax(int nField, int r1, int r2, double & ans)
{
	bool fir = true;
	string tmp;
	for (int r = r1; r <= r2; ++r) {
		if (fir) {
			getRecord(r, nField, tmp);
			ans = convertStringToDouble(tmp);
			fir = false;
		}
		else {
			getRecord(r, nField, tmp);
			ans = std::max(ans, convertStringToDouble(tmp));
		}
	}
	return 0;
}

int Table::countMin(int nField, int r1, int r2, double & ans)
{
	bool fir = true;
	string tmp;
	for (int r = r1; r <= r2; ++r) {
		if (fir) {
			getRecord(r, nField, tmp);
			ans = convertStringToDouble(tmp);
			fir = false;
		}
		else {
			getRecord(r, nField, tmp);
			ans = std::min(ans, convertStringToDouble(tmp));
		}
	}
	return 0;
}

int Table::countSum(int nField, int r1, int r2, double & ans)
{
	ans = 0;
	string tmp;
	for (int r = r1; r <= r2; ++r) {
		getRecord(r, nField, tmp);
		ans += convertStringToDouble(tmp);
	}
	return 0;
}

int Table::countAve(int nField, int r1, int r2, double & ans)
{
	countSum(nField, r1, r2, ans);
	ans /= (r2 - r1 + 1);
	return 0;
}

int Table::countNum(int nField, int r1, int r2, double & ans)
{
	ans = 0;
	string tmp;
	for (int r = r1; r <= r2; ++r) {
		getRecord(r, nField, tmp);
		if (tmp != "") ans++;
	}
	return 0;
}

Table::~Table()
{
	m_Records.clear();
	deleteMark.clear();
}
