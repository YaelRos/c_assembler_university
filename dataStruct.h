#define DATA "data"
#define STRING "string"

typedef struct dataNode {
	struct dataNode *next;
	int binaryData[MAX_WORD_LENGTH];
} DataImgNode;

typedef struct dataImg {
	DataImgNode head*;
	DataImgNode tail*;
	int dc;
} DataImg;


typedef struct instructImg {
	InstructImgNode *head;
	int ic;
} InstructImg;

typedef struct instructNode {
	
	INSTRUCTIONS instruct;
	struct instructNode *next;

} InstructImgNode;

typedef struct symbNode {
	char* symbName;
	int symbAddr;
	SymbType symbType;
	struct symbNode *next;
} SymbNode;

typedef struct symbTable {
	SymbNode *head;
	SymbNode *tail;
	int counter;
} SymbTable;

typedef struct parsedLine {
	char* fileName;
	int lineNum;
	char* line;
	int error;
	int symbFlag;
	char* symbValue;
	LineTypes lineType;
	DotDataTypes dataType;
	int instructName;
	struct parseLine *next
} ParsedLineNode;

typedef struct parsedFile {
	ParsedLineNode *head;
	ParsedLineNode *tail;
	int count;
} ParsedFile; 

typedef enum LINES_TYPE 
{
	DATA_TYPE,
	CODE_TYPE,
	ENTRY_TYPE,
	EXTERNAL_TYPE,
} LineTypes;

typedef enum DOT_DATA_TYPE 
{
	DOT_DATA_TYPE,
	DOT_STRING_TYPE
} DotDataTypes;

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
