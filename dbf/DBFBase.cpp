#include "DBFBase.h"

fieldDefinition TypeString()
{
	fieldDefinition fd;
	memset(&fd, 0, sizeof fd);
	fd.cFieldType = 'C';
	fd.uLength = 10;
	return fd;
}

fieldDefinition TypeDate()
{
	fieldDefinition fd;
	memset(&fd, 0, sizeof fd);
	fd.cFieldType = 'D';
	fd.uLength = 8;
	return fd;
}

fieldDefinition TypeDouble()
{
	fieldDefinition fd;
	memset(&fd, 0, sizeof fd);
	fd.cFieldType = 'F';
	fd.uLength = 8;
	return fd;
}

fieldDefinition TypeInteger()
{
	fieldDefinition fd;
	memset(&fd, 0, sizeof fd);
	fd.cFieldType = 'I';
	fd.uLength = 4;
	return fd;
}

fieldDefinition TypeBoolean()
{
	fieldDefinition fd;
	memset(&fd, 0, sizeof fd);
	fd.cFieldType = 'L';
	fd.uLength = 1;
	return fd;
}

fieldDefinition getType(char ch)
{
	if (ch == 'C')	return TypeString();
	else if (ch == 'D') return TypeDate();
	else if (ch == 'F') return TypeDouble();
	else if (ch == 'I' || ch == 'N') return TypeInteger();
	else if (ch == 'L') return TypeBoolean();
	return TypeString();
}
