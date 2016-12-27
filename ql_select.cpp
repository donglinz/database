#include "ql_select.h"



ql_select::ql_select(string q_line) : m_sSelect(q_line)
{
}

void ql_select::run()
{
	Table ret;

	run_select(m_sSelect, ret);
	ret.prettyPrint();
}

ql_select::~ql_select()
{
	
}

int ql_select::run_select(string q_line, Table& ret)
{
	q_line = Praser::trim(q_line);

	if (q_line.substr(0, 6) != select) {
		std::cerr << "[ERROR] 嵌套查询中SELECT 语句不完整" << std::endl;
	}

	// 字段
	std::shared_ptr<std::vector<string> >m_fields(new std::vector<string>());
	std::shared_ptr<std::vector<string> >m_from(new std::vector<string>());
	std::shared_ptr<std::vector<string> >m_where(new std::vector<string>());
	std::shared_ptr<std::vector<string> >m_group_by(new std::vector<string>());
	std::shared_ptr<std::vector<string> >m_order_by(new std::vector<string>());
	std::shared_ptr<std::vector<string> >m_having(new std::vector<string>());

	bool m_distinct = false;

	FieldsPosition positions[8];
	positions[0].name = select;
	positions[1].name = from;
	positions[2].name = where;
	positions[3].name = group_by;
	positions[4].name = order_by;
	positions[5].name = having;
	int fpos;
	int p_cnt = 0;
	bool hasq = false;
	for (int i = 0; i < q_line.length(); ++i) {
		if (q_line[i] == '(') {
			hasq = true;
			++p_cnt;
		}
		if (q_line[i] == ')') {
			--p_cnt;
			if (p_cnt == 0) {
				fpos = i + 1;
				break;
			}
		}
	}
	
	positions[0].pos = Praser::findFirstOf(q_line, select);
	positions[1].pos = Praser::findFirstOf(q_line, from);
	positions[2].pos = Praser::findFirstOf(q_line, where, (hasq ? fpos : 0));
	positions[3].pos = Praser::findFirstOf(q_line, group_by, (hasq ? fpos : 0));
	positions[4].pos = Praser::findFirstOf(q_line, order_by, (hasq ? fpos : 0));
	positions[5].pos = Praser::findFirstOf(q_line, having, (hasq ? fpos : 0));
	
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
			std::cerr << e.what() << " " << (*positions[cnt].posPtr)[0] << "附近有语法错误!" << std::endl;
			return 1;
		}
	}
	if ((*m_fields)[0].substr(0, distinct.length()) == distinct) {
		isdistinct = true;
		(*m_fields)[0] = (*m_fields)[0].substr(distinct.length() + 1, 99999);
	}

	if (m_from->size() > 1) {
		string ne_sql;
		for (int i = 0; i < m_from->size(); ++i) {
			if (i != 0) ne_sql += ",";
			ne_sql += (*m_from)[i];
		}

		ne_sql = Praser::trim(ne_sql);
		ne_sql = ne_sql.substr(1, ne_sql.length() - 2);
		// 递归嵌套查询
		if(run_select(ne_sql, ret) != 0) return 1;
	}
	else {
		// 没有递归嵌套查询
		FILE *tfp;
		if ((tfp = fopen(((*m_from)[0] + ".dbf").c_str(), "rb")) == NULL) {
			std::cerr << (*m_from)[0] << " " << ex_do_not_have_table.what() << std::endl;
			return 1;
		}
		ret.open((*m_from)[0] + ".dbf");
	}
	ret.close();

	if(get_select(ret, m_fields, m_where, m_group_by, m_order_by, m_having) != 0) return 1;
	
	return 0;
}


int ql_select::get_select(
	Table & m_table, 
	std::shared_ptr<std::vector<string> > m_fields,
	std::shared_ptr<std::vector<string> > m_where,
	std::shared_ptr<std::vector<string> > m_group_by,
	std::shared_ptr<std::vector<string> > m_order_by,
	std::shared_ptr<std::vector<string> > m_having
)
{
	if (m_where->size() != 0) {
		if (runWhere(m_table, (*m_where)[0]) != 0) {
			std::cerr << "[ERROR] WHERE 参数有误!" << std::endl;
		}
	}
	
	if (m_table.numRecord() == 0) return 0;
	if ((*m_fields)[0].find('(') != string::npos || m_group_by->size() != 0) {
		runGroupBy(m_table, m_group_by->size() == 0?"":(*m_group_by)[0], *m_fields);
		if(m_order_by->size() != 0) runOrderBy(m_table, *m_order_by);
		if (m_having->size() != 0) runHaving(m_table, (*m_having)[0]);
		return 0;
	}

	if (m_order_by->size() != 0)
		runOrderBy(m_table, *m_order_by);
	if(m_fields->size() != 0) 
		runFrom(m_table, *m_fields);
	return 0;
}

int ql_select::runWhere(Table & m_table, string s_condition)
{
	for (int nRec = m_table.numRecord() - 1; nRec >= 0 ; --nRec) {
		bool match;
		m_table.isMatch(nRec, s_condition, match);
		if (match == false) {
			m_table.deleteRecord(nRec);
		}
	}
	

	return 0;
}

int ql_select::runGroupBy(Table & m_table, string s_condition, std::vector<string> v_fields)
{

	std::vector<string> vec;
	std::vector<int> isAggregateFunction(v_fields.size(), 0);

	vec.push_back(s_condition);
	m_table.sort(vec);
	int nPos = 0;
	for (string& field : v_fields) {
		if (field.substr(0, count.length()) == count) {
			isAggregateFunction[nPos] = AggregateFunc::Count;
			field = field.substr(count.length(), field.length() - count.length());
			field.pop_back();
		}
		else if (field.substr(0, max.length()) == max) {
			isAggregateFunction[nPos] = AggregateFunc::Max;
			field = field.substr(max.length(), field.length() - max.length());
			field.pop_back();
		}
		else if (field.substr(0, min.length()) == min) {
			isAggregateFunction[nPos] = AggregateFunc::Min;
			field = field.substr(min.length(), field.length() - min.length());
			field.pop_back();
		}
		else if (field.substr(0, ave.length()) == ave) {
			isAggregateFunction[nPos] = AggregateFunc::Ave;
			field = field.substr(ave.length(), field.length() - ave.length());
			field.pop_back();
		}
		else if (field.substr(0, sum.length()) == sum) {
			isAggregateFunction[nPos] = AggregateFunc::Sum;
			field = field.substr(sum.length(), field.length() - sum.length());
			field.pop_back();
		}
		else {
			isAggregateFunction[nPos] = AggregateFunc::None;
		}
		nPos++;
	}
	
	nPos = -1;
	for (int i = 0; i < isAggregateFunction.size(); ++i) {
		if (isAggregateFunction[i] == AggregateFunc::None) {
			nPos = i;
			break;
		}
	}
	
	std::vector<std::shared_ptr<std::vector<string> > > m_Records;
	std::vector<std::pair<int, int> > Range;
	std::vector<fieldDefinition> fds = m_table.getFieldDefinition(v_fields);
	
	for (int nFie = 0; nFie < fds.size(); ++nFie) {
		if (nPos != nFie) {
			fieldDefinition fd = TypeDouble();
			strncpy(fd.cFieldName, fds[nFie].cFieldName, 10);
			fds[nFie] = fd;
		}
	}
	if (nPos != -1) {
		for (int nRec = 0, nFiePos = m_table.fieldPos(v_fields[nPos]); nRec < m_table.numRecord(); ++nRec) {
			string rec;
			string recLast;
			m_table.getRecord(nRec, nFiePos, rec);
			if (nRec) {
				m_table.getRecord(nRec - 1, nFiePos, recLast);
			}
			if (nRec == 0 || rec != recLast) {
				m_Records.push_back(std::shared_ptr<std::vector<string> >());
				m_Records.back().reset(new std::vector<string>(v_fields.size(), ""));
				Range.push_back(std::make_pair(nRec, nRec));
				GET_STRING(m_Records, m_Records.size() - 1, nPos) = rec;
			}
			if (rec == recLast) {
				Range.back().second = std::max(nRec, Range.back().second);
			}
		}
	}
	else {
		m_Records.push_back(std::shared_ptr<std::vector<string> >());
		m_Records.back().reset(new std::vector<string>(v_fields.size(), ""));
		Range.push_back(std::make_pair(0, m_table.numRecord() - 1));
	}
	

	for (int nRec = 0; nRec < m_Records.size(); ++nRec) {
		for (int nFie = 0; nFie < v_fields.size(); ++nFie) {
			if (nFie == nPos) continue;
			double countAns = 0;
			if (isAggregateFunction[nFie] == AggregateFunc::Count) {
				m_table.countNum(m_table.fieldPos(v_fields[nFie]), Range[nRec].first, Range[nRec].second, countAns);
			}
			else if (isAggregateFunction[nFie] == AggregateFunc::Max) {
				m_table.countMax(m_table.fieldPos(v_fields[nFie]), Range[nRec].first, Range[nRec].second, countAns);
			}
			else if (isAggregateFunction[nFie] == AggregateFunc::Min) {
				m_table.countMin(m_table.fieldPos(v_fields[nFie]), Range[nRec].first, Range[nRec].second, countAns);
			}
			else if (isAggregateFunction[nFie] == AggregateFunc::Ave) {
				m_table.countAve(m_table.fieldPos(v_fields[nFie]), Range[nRec].first, Range[nRec].second, countAns);
			}
			else if (isAggregateFunction[nFie] == AggregateFunc::Sum) {
				m_table.countSum(m_table.fieldPos(v_fields[nFie]), Range[nRec].first, Range[nRec].second, countAns);
			}
			GET_STRING(m_Records, nRec, nFie) = Praser::convertNumberToString(countAns, true);
		}
	}
	
	m_table.setRecords(m_Records, fds);
	return 0;
}

int ql_select::runOrderBy(Table & m_table, std::vector<string> v_condition)
{
	m_table.sort(v_condition);
	return 0;
}

int ql_select::runFrom(Table & m_table, std::vector<string> v_fields)
{
	if (v_fields[0] == "*") return 0;

	std::vector<fieldDefinition> fds = m_table.getFieldDefinition(v_fields);
	std::vector<std::shared_ptr<std::vector<string> > > m_Records;
	m_Records.resize(m_table.numRecord());
	for (std::shared_ptr<std::vector<string> > & pt : m_Records) {
		pt.reset(new std::vector<string>(v_fields.size(), ""));
	}
	for (int nFie = 0; nFie < v_fields.size() ; ++nFie) {
		int fieldPos = m_table.fieldPos(v_fields[nFie]);
		for (int nRec = 0; nRec < m_table.numRecord(); ++nRec) {
			string ans;
			m_table.getRecord(nRec, fieldPos, ans);
			GET_STRING(m_Records, nRec, nFie) = ans;
		}
	}
	m_table.setRecords(m_Records, fds);
	return 0;
}

int ql_select::runHaving(Table & m_table, std::string s_condition)
{
	for (int i = 0; i < s_condition.length(); ++i) {
		bool erase = false;
		if (s_condition.substr(i, count.length()) == count) {
			s_condition.erase(i, count.length());
			erase = true;
		} else if (s_condition.substr(i, max.length()) == max) {
			s_condition.erase(i, max.length());
			erase = true;
		} else if (s_condition.substr(i, min.length()) == min) {
			s_condition.erase(i, min.length());
			erase = true;
		} else if (s_condition.substr(i, ave.length()) == ave) {
			s_condition.erase(i, ave.length());
			erase = true;
		} else if (s_condition.substr(i, sum.length()) == sum) {
			s_condition.erase(i, sum.length());
			erase = true;
		}
		if (erase) {
			for (int j = i + 1; j < s_condition.length(); ++j) {
				if (s_condition[j] == ')') {
					s_condition.erase(j, 1);
					break;
				}
			}
		}
	}

	runWhere(m_table, s_condition);

	return 0;
}
