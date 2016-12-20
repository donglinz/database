#include "ql_select.h"



ql_select::ql_select(string q_line) : m_sSelect(q_line)
{
}

void ql_select::run()
{
	Table ret = run_select(m_sSelect);
	ret.prettyPrint();
}

ql_select::~ql_select()
{
}

Table ql_select::run_select(string q_line)
{
	q_line = Praser::trim(q_line);

	if (q_line.substr(0, 6) != select) {
		std::cerr << "[ERROR] 嵌套查询中SELECT 语句不完整" << std::endl;
	}

	// 字段
	std::vector<string> *m_fields = new std::vector<string>;
	std::vector<string> *m_from = new std::vector<string>;
	std::vector<string> *m_where = new std::vector<string>;
	std::vector<string> *m_group_by = new std::vector<string>;
	std::vector<string> *m_order_by = new std::vector<string>;
	std::vector<string> *m_having = new std::vector<string>;

	bool m_distinct = false;

	FieldsPosition positions[8];
	positions[0].name = select;
	positions[1].name = from;
	positions[2].name = where;
	positions[3].name = group_by;
	positions[4].name = order_by;
	positions[5].name = having;

	positions[0].pos = Praser::findFirstOf(q_line, select);
	positions[1].pos = Praser::findFirstOf(q_line, from);
	positions[2].pos = Praser::findFirstOf(q_line, where);
	positions[3].pos = Praser::findFirstOf(q_line, group_by);
	positions[4].pos = Praser::findFirstOf(q_line, order_by);
	positions[5].pos = Praser::findFirstOf(q_line, having);
	
	positions[0].posPtr = m_fields;
	positions[1].posPtr = m_from;
	positions[2].posPtr = m_where;
	positions[3].posPtr = m_group_by;
	positions[4].posPtr = m_order_by;
	positions[5].posPtr = m_having;

	std::sort(positions, positions + 6);

	for (int cnt = 0; cnt < 6; ++cnt) {
		if (positions[cnt].pos == -1) continue;
		if (cnt == 5) {
			(*positions[cnt].posPtr).push_back(q_line.substr(positions[cnt].pos + positions[cnt].name.length(),
				1000));
		}
		else {
			(*positions[cnt].posPtr).push_back(q_line.substr(positions[cnt].pos + positions[cnt].name.length(),
				positions[cnt + 1].pos - (positions[cnt].pos + positions[cnt].name.length())));
		}
		try {
			(*positions[cnt].posPtr) = Praser::resolveField((*positions[cnt].posPtr)[0]);
		}
		catch (std::exception e) {
			for (int i = 0; i <= 5; ++i) delete positions[i].posPtr;
			std::cerr << e.what() << " " << (*positions[cnt].posPtr)[0] << "附近有语法错误!" << std::endl;
			return Table();
		}
	}
	
	Table ret;

	if (m_from->size() > 1) {
		string ne_sql;
		for (int i = 0; i < m_from->size(); ++i) ne_sql += (*m_from)[i];
		if (ne_sql[0] != '(' || ne_sql.back() != ')') {
			std::cerr << "[ERROR] ";
			std::cerr << ne_sql;
			std::cerr << " 附近有语法错误" << std::endl;
			for (int i = 0; i <= 5; ++i) delete positions[i].posPtr;
			return Table();
		}
		
		ret = run_select(ne_sql);
	}
	else {
		FILE *tfp;
		if ((tfp = fopen((*m_from)[0].c_str(), "rb")) == NULL) {
			std::cerr << (*m_from)[0] << " " << ex_do_not_have_table.what() << std::endl;
			for (int i = 0; i <= 5; ++i) delete positions[i].posPtr;
			return Table();
		}
		ret.open((*m_from)[0]);
	}
	 
	 
	 return get_select(ret, m_fields, m_where, m_group_by, m_order_by, m_having);
	 
}

Table ql_select::get_select(
	Table m_table, 
	std::vector<string>* m_fields,
	std::vector<string>* m_where,
	std::vector<string>* m_group_by,
	std::vector<string>* m_order_by,
	std::vector<string>* m_having
)
{
	
	// 一定要返回一个close以后的table
	return Table();
}
