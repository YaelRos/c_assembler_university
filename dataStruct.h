#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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


#define A "100"
#define R "010"
#define E "001"

#define R1 "r1"
#define R2 "r2"
#define R3 "r3"
#define R4 "r4"
#define R5 "r5"
#define R6 "r6"
#define R7 "r7"
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
	DataImgNode* tail;
	int dc;
} DataImg;

struct instructNode {
	char instruction[MAX_WORD_LENGTH+1];
};

typedef struct instructImg {
	struct instructNode instructions[MEM_SIZE];
	int ic;
} InstructImg;

struct externNode {
	char SymbName[LABEL_MAX_LEN];
	int memAddr;
};

typedef struct externTable {
	struct externNode *externalLabel;
	int counter;
} ExternTable;

typedef struct symbNode {
	char* symbName;
	int symbAddr;
	int symbType;
	struct symbNode *next;
} SymbNode;

typedef struct symbTable {
	SymbNode *head;
	SymbNode *tail;
	ExternTable exTable;
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


typedef enum DOT_DATA_TYPE 
{
	DOT_DATA_TYPE,
	DOT_STRING_TYPE
} DotDataTypes;

typedef struct opCode
{
	char* opCodeName;
	int opCodeNum;
	int instrucGroup;
} OpCode;

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

typedef struct instructionStruct
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
		DotDataTypes dataType;
		InstructionStruct instruct;
		struct EterenEntryType et;
	} typeHandle;
	struct parsedLine *next;
} ParsedLineNode;

typedef struct parsedFile 
{
	struct parsedLine* head;
	struct parsedLine* tail;
	char * filename;
	int error;
	int count;
} ParsedFile; 

void initDataImg(DataImg* dataImg);

void initParsedFile(ParsedFile *pf);

void initSymbTable(SymbTable *symbTable);

void initExTable(ExternTable *exTable);

void initPardedLineNode(ParsedLineNode* line, char* ln);

/*
	Get the symbol node from the symbol table that has the same name like the labelName.

    @param SymbTable* symbTable - The symbol table
	@param char* labelName - The symbol value to b compered.
	@param SymbNode *tmp - The symbol node.
	@return SymbNode* - the symbol node from the symbol table that has the same name like the labelName,
	if there's not such a node, return NULL.
*/
SymbNode* getSymbFeature(SymbTable *symbTable, char* labelName, SymbNode *tmp);

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
    @param int count - number of lines that already read.
*/
void addLineToParsedFile(ParsedLineNode *line, ParsedFile *parsedFile, int count);

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
    @return int - A flag representing if the symbol already exists in our symbol table. 0 = False, 1 = True
*/
int addValuesInSymbTable(SymbTable *symbTable, SymbNode *symbNode);

/*
	initialize nre symbol node
	@param char* name - The name of the symbol
	@param DataImg* dataImg - The data image
    @param InstructImg* instructImg - The instruction image
    @param SymbNode* symbNode - An empty symbol node 
    @param int feature - The feature of the symbol
*/
void initSymbNode(char* name, DataImg *dataImg, 
	InstructImg *instructImg,SymbNode* symbNode, int feature);

/*
	Check if the symbol table contains the symbol value sy
	@param char* sy - The symbol value
    @param SymbTable* symbTable - The symbol table
	@return int - A flag representing if the symbol value is in the symbol table. 0 = False, 1 = True
*/
int symbTableContains(char* sy, SymbTable *symbTable);

/*
	Set the symbol value in the line object to be 
	the symbol which defined at the beginning of the line,
	and move the string pointer forward as the length of the symbol value.
	@param ParsedLineNode* line - The object of the current line. 
	@param char* label - The substring of line->ln which start at the end of the label.
*/
void setSymbValue(ParsedLineNode* line, char* label);

