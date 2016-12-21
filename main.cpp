//#include "dbf\dbf.h"
#include "ql_Manager.h"
using namespace std;
#define DEBUG_SQL
#include <iostream>
#include "cparse\shunting-yard.h"
#ifdef DEBUG_DBF

int main(int argc, char *argv[])
{
/*
	TokenMap vars;
	calculator calu;
	calu.compile("(d != 'abcdef')");
	vars["a"] = 2.33;
	vars["b"] = 2.34;
	vars["c"] = "1234";
	vars["d"] = "4567";
	std::cout << calu.eval(vars).str() << std::endl;
	packToken pt;
	*/
    if( false )
    {
        string sFileReadTest = "TestCreate.dbf";
        //sFileReadTest = argv[1]; // do a read test on the given file

        DBF mydbf;
        std::cerr << "Read Test of " << sFileReadTest << std::endl;
        int nRet = mydbf.open(sFileReadTest);
        if( nRet )
        {
            std::cerr << "Unable to Open File " << sFileReadTest << std::endl;
        } else
        {
            int nRecs = mydbf.GetNumRecords();
            int nFields = mydbf.GetNumFields();
            std::cout << "Open() found " << nRecs << " records, and " << nFields << " fields." << std::endl;
            mydbf.dumpAsCSV();
            std::cout << "Done Read Test " << std::endl;
            mydbf.close();
        }
    } else
    {
        // no param, means to run a test to create, read and delete a record in the dbf
		
        // now create a db
        std::cout << "Create file TestCreate.dbf from code" << std::endl;
        DBF newdbf;


        int nRet = newdbf.create("TestCreate.dbf",6);
        if( nRet == 0 )
        {
            // create the 5 fields
            fieldDefinition fd = TypeInteger();
            strncpy(fd.cFieldName,"ID",10);
            newdbf.assignField(fd,0);

			fd = TypeString();
            strncpy(fd.cFieldName,"FirstName",10);
			fd.uLength = 20;
            newdbf.assignField(fd,1);

			fd = TypeDouble();
            strncpy(fd.cFieldName,"Weight",10);
            newdbf.assignField(fd,2);

			fd = TypeInteger();
            strncpy(fd.cFieldName,"Age",10);
            newdbf.assignField(fd,3);

			fd = TypeBoolean();
			strncpy(fd.cFieldName, "Married", 10);
			newdbf.assignField(fd, 4);

			fd = TypeDate();
			strncpy(fd.cFieldName, "Date", 10);
			newdbf.assignField(fd, 5);

			

            // now create some records to test it!
            string s1[6]= {"1" ,"Ric G","210.123456789123456","43","T", "2016-10-10"};
            newdbf.appendRecord(&s1[0],6);

            string s2[6]={"1000" ,"Paul F","196.2","33","T", "2016-10-10" };
            newdbf.appendRecord(s2,6);

            string s3[6]={"20000" ,"Dean K","186.1","23","F", "2016-10-10" };
            newdbf.appendRecord(s3,6);

            string s4[6]={"300000" ,"Gary\" Q","175.123456789","13","F", "2016-10-10" };
            newdbf.appendRecord(s4,6);

            string s5[6]={"2000000" ,"Dan'e \"D, with comma and over sized field that will be truncated","65.2","6","F", "2016-10-10" };
            newdbf.appendRecord(s5,6);

            // now add a huge pile of random records to see if it crashes
            //int nID = 2000001;
            //for( int i=0; i < 1000; i++)
            //{
            //    stringstream ssName;
            //    ssName << "FirstName" << nID;

            //    float fWeight = (float) nID / 40000.0;
            //    int nAge = i % 120;


            //    stringstream ssID;
            //    ssID << nID;

            //    stringstream ssWeight;
            //    ssWeight << fWeight;

            //    stringstream ssAge;
            //    ssAge << nAge;

            //    string sMarried = "F";
            //    if( nID % 2 == 0 )
            //        sMarried = "T";

            //    string sRec[5]={ssID.str(),ssName.str(),ssWeight.str(),ssAge.str(),sMarried};
            //    newdbf.appendRecord(sRec,5);

            //    nID++;
            //}



            newdbf.close();

            std::cout << "Done Creating the DBF" << std::endl;

            //std::cout << "Open the DBF for reading and writing" << std::endl;
            //// now read the dbf back to test that it worked!
            //DBF readTest;
            //readTest.open("TestCreate.dbf",true);
            //int nRecs = readTest.GetNumRecords();
            //int nFields = readTest.GetNumFields();
            //std::cout << "Open() found " << nRecs << " records, and " << nFields << " fields." << std::endl;
            //readTest.dumpAsCSV();
            //std::cout << "Done Reading a freshly created DBF! " << std::endl;

            //std::cout << "Test Delete Record 1 in DBF! " << std::endl;
            //std::cout << "Test Delete Record 3 in DBF! " << std::endl;
            //readTest.markAsDeleted(1);
            //readTest.markAsDeleted(3);
            //readTest.dumpAsCSV();

            //std::cout << "Done Test Delete Record DBF! " << std::endl;

            //readTest.close();
        }
    }
	system("pause");
    return 0;
}

#endif // DEBUG

#ifdef DEBUG_SQL
int main() {
	ql_Manager qm;
	string sql = "";
	while (true) {
		string tmp;
		getline(cin, tmp);
		sql += " " + tmp;
		int ret = sql.find(';');
		if (ret != string::npos) {
			qm.run(sql.substr(0, ret + 1));
			sql = sql.substr(ret + 1, 10000);
		}
	}
}
#endif // DEBUG_SQL

/*
select ID, Firstname from
test where date='2016-10-10';
*/