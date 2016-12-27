#pragma once
typedef unsigned char uint8;
typedef short int uint16;
typedef int uint32;

#define DBF_DELETED_RECORD_FLAG '*' // found by reading with hex edito
#include <cstring>

struct fileHeader
{
	uint8 u8FileType;			/*0*/
	uint8 u8LastUpdateYear;		/*1*/
	uint8 u8LastUpdateMonth;	/*2*/
	uint8 u8LastUpdateDay;		/*3*/
	uint32 uRecordsInFile;		/*4-7*/
	uint16 uPositionOfFirstRecord;	/*8-9*/
	uint16 uRecordLength;			/*10-11*/	// the length of each record, includes the delete flag (byte) at start of record
	uint8 Reserved16[4 * 4];			/*12-27*/	// 16 bytes reserved, must always be set to zeros
	uint8 uTableFlags;				/*28*/
	uint8 uCodePage;				/*29*/
	uint8 Reserved2[2];				/*30-31*/		// 2 bytes reserved, must put zeros in all reserved fields
};

// after the file header, we can have n field definition records, terminated by the byte 0x0D
// must build as a union to make sure bytes are aligned correctly
struct fieldDefinition
{
	char cFieldName[11];
	char cFieldType;
	uint32 uFieldOffset; // location of field from start of field record, first ' ' or '*' is position 0
	uint8 uLength; // Length of Field in bytes

				   // 以下字段0x30和0x03类型没有区别，暂时用不到
	uint8 uNumberOfDecimalPlaces;
	uint8 FieldFlags;
	uint8 uNextAutoIncrementValue[4]; // 32 bit int
	uint8 uAutoIncrementStep;
	uint8 Reserved8[8]; // should always be zero
};
// terminated by the byte 0x0D then 263 bytes of 0x00
// then the records start

// C
fieldDefinition TypeString();

// D
fieldDefinition TypeDate();

// F
fieldDefinition TypeDouble();

// N
fieldDefinition TypeInteger();

// L
fieldDefinition TypeBoolean();

fieldDefinition getType(char ch);