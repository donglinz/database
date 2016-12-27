#ifndef DBF_H
#define DBF_H

// Copyright (C) 2012 Ron Ostafichuk
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files
// (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify,
// merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
// OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
// LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR
// IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.


#include <cstdio>
#include <iostream>
#include <sstream>
#include <errno.h>
#include <cmath>
#include <ctime>
#include <string>
#include <algorithm>
#include <vector>
#include "DBFBase.h"
#include "../Praser.h"

#include <io.h>


using std::string;
using std::min;
using std::stringstream;

// 支持的数据类型int double string(nvarchar) bool(char) date
class DBF
{
public:
    DBF();
    ~DBF();
    int open(string sFileName,bool bAllowWrite=false); // open an existing dbf file
    void close();

    int markAsDeleted(int nRecord); // mark this record as deleted
    int create(string sFileName,int nNumFields); // create a new dbf file with space for nNumFields
    int assignField(fieldDefinition myFieldDef,int nField); // used to assign the field info ONLY if num records in file = 0 !!!
    int appendRecord(string *sValues, int nNumValues); // used to append records to the end of the dbf file
	void dumpAsCSV(); // output fields and records as csv to std output
	string readField(int nField); // read the request field as a string always from the loaded record!

	int GetNumRecords();
	int GetNumFields();
	fileHeader getMyFileHeader();
	std::vector<fieldDefinition> getMyFieldDefinitions();
	int getFieldIndex(string sFieldName);
	int loadRec(int nRecord); // load the record into memory


private:
	string GetFieldName(int nField);

	string convertInt(int number);
	
	string convertNumber(uint8 *n, int nSize);

	int ConvertStringToInt(string sInteger, int nSize, char *cRecord);

	int ConvertStringToFloat(string sFloat, int nSize, char *cRecord);

	int ConvertStringToDate(std::string sDate, char* cRecord);


    FILE * m_pFileHandle;
    string m_sFileName;

    bool m_bStructSizesOK; // this must be true for engine to work!
    bool m_bAllowWrite;
    fileHeader m_FileHeader;
    std::vector<fieldDefinition> m_FieldDefinitions; // allow a max of 255 fields
    int m_nNumFields; // number of fields in use
	std::vector<char> m_pRecord;

	int resizeDBFFile();
	int updateFileHeader();
	
	bool isRecordDeleted(); // check if loaded record is deleted
	
	double readFieldAsDouble(int nField); // read the request field as a double to get higher performance for 'B' type fields only!
	
};

#endif // DBF_H
