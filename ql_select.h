#pragma once

#include <algorithm>

#include "Table.h"
#include "Praser.h"

class ql_select
{
public:
	ql_select(string q_line);
	void run();
	~ql_select();
	
private:
	const string m_sSelect;
	const string select = "select";
	const string where = "where";
	const string from = "from";
	const string distinct = "dictinct";
	const string group_by = "group by";
	const string order_by = "order by";
	const string having = "having";

	const string count = "count(";
	const string max = "max(";
	const string min = "min(";
	const string ave = "ave(";
	const string sum = "sum(";

	enum AggregateFunc {
		None,
		Count,
		Max,
		Min,
		Ave,
		Sum
	};

	std::exception ex_do_not_have_table = std::exception("±í²»´æÔÚ!");
	bool isdistinct = false;

	struct FieldsPosition {
		int pos;
		string name;
		std::shared_ptr<std::vector<string> > posPtr;
		bool operator < (const FieldsPosition& rhs) const {
			return pos < rhs.pos;
		}
	};

	//std::shared_ptr<BaseTable> selectTable;
	int run_select(string q_line, Table& ret);

	int get_select(
		Table & m_table, 
		std::shared_ptr<std::vector<string> > m_fields, 
		std::shared_ptr<std::vector<string> > m_where,
		std::shared_ptr<std::vector<string> > m_group_by,
		std::shared_ptr<std::vector<string> > m_order_by, 
		std::shared_ptr<std::vector<string> > m_having
	);

	int runWhere(Table& m_table, string s_condition);
	int runGroupBy(Table& m_table, string s_condition, std::vector<string> v_fields);
	int runOrderBy(Table& m_table, std::vector<string> v_condition);
	int runFrom(Table& m_table, std::vector<string> v_fields);
	int runHaving(Table& m_table, std::string s_condition);
};

