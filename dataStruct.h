#ifndef DATA_STRUCT_H
#define DATA_STRUCT_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "error.h"

#define LABEL_MAX_LEN 31
#define FIRST_ADDRES 100
#define MAX_WORD_LENGTH 15
#define NUM_OF_INSTRUCTION 16
#define MAX_LINE_LENGTH 80
#define MAX_LENTH_OF_INSTRUCT 4
#define MEM_SIZE 4096
#define ENTRY_LEN 5
#define EXTERN_LEN 6
#define STR_LEN 6
#define DATA_LEN 4
#define FILE_NAME_MAX_LEM 4
#define OPCODE_LEN_BITS 4
#define REG_METHOD_LEN_BITS 4
#define ARE_LEN_BITS 3
#define IMM_VAL_LEN_BITS 12
#define REG_VAL_BITS 3
#define REG_PRIOR_BITS 6
#define NUM_OF_DATA_GUIDANCE 2
#define MAX_LINES_IN_FILE 50
#define BIN_TO_OCT_BITS 3
#define REGISTERS_NUM 8

#define NO_METHOD_BITS "0000"
#define IMMEDIATE_BITS "0001"
#define DIRECT_BITS "0010"
#define INDIRECT_REG_BITS "0100"
#define DIRECT_REG_BITS "1000"

#define A "100"
#define R "010"
#define E "001"

#define ENTRY "entry"
#define EXTERN "extern"
#define DATA "data"
#define STRING "string"


enum LINES_TYPE 
{
	DATA_TYPE,
	CODE_TYPE,
	ENTRY_TYPE,
	EXTERNAL_TYPE
};

typedef struct dataNode {
	struct dataNode *next;
	char binaryData[MAX_WORD_LENGTH+1];
} DataImgNode;

typedef struct dataImg {
	DataImgNode* head;
	int dc;
} DataImg;

struct instructNode {
	char instruction[MAX_WORD_LENGTH+1];
};

typedef struct {
	struct instructNode instructions[MEM_SIZE];
	int ic;
} InstructImg;



typedef struct externNode {
	char SymbName[LABEL_MAX_LEN];
	int memAddr;
	struct externNode *next;
} ExternNode ;

typedef struct {
	ExternNode *head;
	int counter;
} ExternTable;

typedef struct symbNode {
	struct symbNode *next;
	int symbAddr;
	int symbType;
	char symbName[LABEL_MAX_LEN];
} SymbNode;

typedef struct {
	SymbNode *head;
	ExternTable *exTable;
	int counter;
} SymbTable;

struct EterenEntryType
{
	char labelName[LABEL_MAX_LEN];
};

enum ADDRESSING_METHOD 
{
	IMMEDIATE,
	DIRECT,
	INDIRECT_REG,
	DIRECT_REG,
	NO_ADD_METHOD
};


enum DOT_DATA_TYPE 
{
	DOT_DATA_TYPE,
	DOT_STRING_TYPE
};

typedef struct 
{
	char* opCodeName;
	int opCodeNum;
	int instrucGroup;
} OpCode;

typedef struct 
{
	char bin[BIN_TO_OCT_BITS];
	char oct;
} ConvertBinToOct;

enum INSTRUCTION_GROUPS 
{
	TWO_OPERANDS = 1,
	ONE_OPERAND,
	NO_OPERANDS
};

enum INSTRUCTIONS 
{
	MOV,
	CMP,
	ADD,
	SUB,
	LEA,
	CLR,
	NOT,
	INC,
	DEC,
	JMP,
	BNE,
	RED,
	PRN,
	JSR,
	RTS,
	STOP
};

typedef struct
{
	int opCode;
	char opSrc[LABEL_MAX_LEN];
	int opSrcMethod;
	char opDst[LABEL_MAX_LEN];
	int opDstMethod;
	int addLine;
} InstructionStruct;

typedef struct parsedLine {
	char* ln;
	int error;
	int symbFlag;
	char* symbValue;
	int lineType;
	union {
		int dataType;
		InstructionStruct instruct;
		struct EterenEntryType et;
	} typeHandle;
} ParsedLineNode;

typedef struct lineDataForSecIter
{
	ParsedLineNode line;
	struct lineDataForSecIter *next;
} lineDFS; 


typedef struct 
{
	lineDFS *head;
	char * filename;
	int error;
	int count;
} ParsedFile; 

/* Initiate Data Image */
void initDataImg(DataImg* dataImg);

/* Initiate the Parsed File Object */
void initParsedFile(ParsedFile *pf);

/* Initiate the Symbol Table for specific File */
void initSymbTable(SymbTable *symbTable);

/* Initiate a table that contains the operand that using an extrenal label in order to create the .ext file */
ExternTable* initExTable(ExternTable *exTable);

/* Initate the object of specific line in the file */
ParsedLineNode* initPardedLineNode(ParsedLineNode* line);

/*
	Get the symbol node from the symbol table that has the same name like the labelName.

    @param SymbTable* symbTable - The symbol table
	@param char* labelName - The symbol value to b compered.
	@return SymbNode* - the symbol node from the symbol table that has the same name like the labelName,
	if there's not such a node, return NULL.
*/
SymbNode* getSymbFeature(SymbTable *symbTable, char* labelName);

/*
	Add 100+IC to all the address of symbols from type DATA.

    @param SymbTable* symbTable - The symbol table
    @param int ic - IC 
*/
void updateValuesInSymbTable(SymbTable *symbTable, int ic);

/*
	Add a line object to a parsed file. 

	@param ParsedLineNode* line - The object of the current line. 
    @param ParsedFile* parsedFile - The object of the current file. 
*/
void addLineToParsedFile(ParsedLineNode *line, ParsedFile *parsedFile);

/*
	Add a new number to the Data image and increasing the data image counter by 1.

    @param char* binaryStr - The number represented by binary string
    @param DataImg* dataImg - The data image
*/
void addNumToDataImg(char* binaryStr, DataImg *dataImg);

/*
	Add a new node to the symbol table.
    If the record already exists in our symbol table, return 1.
    @param SymbNode* symbNode - An empty symbol node 
    @param SymbTable* symbTable - The symbol table
    @return SymbNode* - A flag representing if the symbol already exists in our symbol table. NULL = False
*/
SymbTable* addValuesInSymbTable(SymbTable *symbTable, SymbNode *symbNode);

/*
	initialize nre symbol node
	@param char* name - The name of the symbol
	@param DataImg* dataImg - The data image
    @param InstructImg* instructImg - The instruction image
    @param SymbTable* symbTable - The symbol table
    @param int feature - The feature of the symbol
    @return int - If the initiate action with done with no error. 0 = False, 1 = True

*/
int initSymbNode(char* name, DataImg *dataImg, 
	InstructImg *instructImg, SymbTable* symbTable, int feature);

/*
	Check if the symbol table contains the symbol value sy
	@param char* sy - The symbol value
    @param SymbTable* symbTable - The symbol table
	@return int - A flag representing if the symbol value is in the symbol table. 0 = False, 1 = True
*/
int symbTableContains(char* sy, SymbTable *symbTable);

/*
	Set the symbol value to entry type.
	@param SymbTable* st - The symbol table. 
	@param char* labelName - The label name of the entry.
	@return int - A flag representing if the symbol value that asked to be update exist. 0 = False, 1 = True
*/
int setSymbEntryType(SymbTable *st, char* labelName);

/*
	For each operand that use an extrenal label update the extTable with the operand value
	and the the address of the opernad.
	@param int memadd - the address line of the operand. 
	@param char* operand - The label name of the opernad.
	@param SymbTable* symbTable - The symbol table.
*/
void updateExTable(int memadd, char* operand, SymbTable* symbTable);


#endif

