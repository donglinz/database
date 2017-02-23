#include "ql_insert.h"



ql_insert::ql_insert(string q) : q_line(q)
{

}


ql_insert::~ql_insert()
{
}

void ql_insert::run()
{
	q_line = Praser::trim(q_line);
	q_line = Praser::toLowerString(q_line);
	std::vector<string> v_fields = Praser::split(q_line, { ' ', ',' });
	fileName = v_fields[2] + ".dbf";
	DBF dbfFile;
	dbfFile.open(fileName, true);

	std::vector<string> vRec;
	vRec.resize(dbfFile.GetNumFields());


	std::vector<string> fields;
	std::vector<string> v_values;
	
	for (string & t : v_fields) {
		if (t[0] == '(') {
			t = t.substr(1, t.length() - 1);
		}
		if (t.back() == ')') {
			t.pop_back();
		}
		if (t[0] == '\'') {
			t = t.substr(1, t.length() - 1);
		}
		if (t.back() == '\'') {
			t.pop_back();
		}
	}
	
	bool b_into = false;
	bool b_values = false;

	for (string & t : v_fields) {
		t = Praser::trim(t);

		if (t == fileName) {
			b_into = true;
			continue;
		}
		if (t == values) {
			b_values = true;
			continue;
		}
		if (b_into) {
			fields.push_back(t);
		}
		if (b_values) {
			v_values.push_back(t);
		}
	}

	if (fields.size() == 0) {
		for (int i = 0; i < v_values.size(); ++i) {
			vRec[i] = v_values[i];
		}
	}
	else {
		if (fields.size() != v_values.size()) {
			std::cerr << "[ERROR] insert  ×Ö¶Î´íÎó!" << std::endl;
		}

		for (int i = 0; i < v_fields.size(); ++i) {
			vRec[dbfFile.getFieldIndex(v_fields[i])] = v_values[i];
		}
	}
	dbfFile.appendRecord(&vRec[0], dbfFile.GetNumFields());
	dbfFile.close();
}
