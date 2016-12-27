#include "ql_create.h"



ql_create::ql_create(string q) : q_line(q)
{
}


ql_create::~ql_create()
{
}

void ql_create::run()
{
	q_line = Praser::toLowerString(q_line);
	fileName = Praser::split(q_line, ' ')[2];
	int p1, p2;
	for (int i = 0; i < q_line.length(); ++i) {
		if (q_line[i] == '(') {
			p1 = i;
		}
		if (q_line[i] == ')') {
			p2 = i;
		}
	}

	q_line = q_line.substr(p1 + 1, p2 - p1 - 1);
	std::vector<string> fields = Praser::split(q_line, ',');
	for (string & tmp : fields) {
		tmp = Praser::trim(tmp);
	}

	DBF newdbf;
	int cnt = 0;
	newdbf.create(fileName + ".dbf", fields.size());
	for (string field : fields) {
		string name = Praser::split(field, ' ')[0];
		string type = Praser::split(field, ' ')[1];
		fieldDefinition fd;
		if (Praser::findFirstOf(type, type_int) == 0) {
			fd = TypeInteger();
		}
		else if (Praser::findFirstOf(type, type_double) == 0) {
			fd = TypeDouble();
		}
		else if (Praser::findFirstOf(type, type_date) == 0) {
			fd = TypeDate();
		}
		else if (Praser::findFirstOf(type, type_char) == 0) {
			fd = TypeString();
			fd.uLength = 1;
		}
		else if (Praser::findFirstOf(type, type_bool) == 0) {
			fd = TypeString();
			fd.uLength = 1;
		}
		else if (Praser::findFirstOf(type, type_varchar) == 0) {
			fd = TypeString();
			for (int i = 0; i < type.length(); ++i) {
				if (type[i] == '(') {
					p1 = i;
				}
				if (type[i] == ')') {
					p2 = i;
				}
			}
			std::stringstream ss(type.substr(p1 + 1, p2 - p1 - 1));
			ss >> fd.uLength;
		}
		strncpy(fd.cFieldName, name.c_str(), 10);
		newdbf.assignField(fd, cnt++);
	}
	newdbf.close();
}
