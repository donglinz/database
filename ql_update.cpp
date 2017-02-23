#include "ql_update.h"



ql_update::ql_update(string q) : q_line(q)
{
}


ql_update::~ql_update()
{
}

void ql_update::run()
{
	q_line = Praser::trim(q_line);
	q_line = Praser::toLowerString(q_line);
	std::vector<string> vec = Praser::split(q_line, ' ');
	string s_condition;
	s_condition = vec.back();
	fileName = vec[1];
	Table m_table;
	m_table.open(fileName + ".dbf", true);
	
	string key = Praser::split(vec[3], '=')[0];
	string val = Praser::split(vec[3], '=')[1];
	for (int nRec = 0; nRec < m_table.numRecord(); ++nRec) {
		bool match = false;
		m_table.isMatch(nRec, s_condition, match);
		if (match) {
			m_table.setRecord(nRec, key, val);
		}
	}
	m_table.close();
}
