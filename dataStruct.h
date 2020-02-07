
typedef struct dataNode {
	struct dataNode *next;
	char binaryData[MAX_WORD_LENGTH+1];
} DataImgNode;

typedef struct dataImg {
	DataImgNode head*;
	DataImgNode tail*;
	int dc;
} DataImg;


typedef struct instructImg {
	InstructionNode *instructions[MEM_SIZE];
	int ic;
} InstructImg;

typedef struct instructNode {
	char instruction[MAX_WORD_LENGTH+1];
} InstructionNode ;

typedef struct symbNode {
	char* symbName;
	int symbAddr;
	LineTypes symbType;
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
	union {
		DotDataTypes dataType;
		InstructionStruct instruct;
		struct EterenEntryType et;
	} typeHandle;
	struct parseLine *next
} ParsedLineNode;

struct EterenEntryType
{
	char labelName[LABEL_MAX_LEN];
} ;

typedef struct parsedFile 
{
	ParsedLineNode *head;
	ParsedLineNode *tail;
	int count;
} ParsedFile; 

typedef struct instructionStruct
{
	Instruction opCode;
	char opSrc[LABEL_MAX_LEN];
	AddressingMethod opSrcMethod;
	char opDst[LABEL_MAX_LEN];
	AddressingMethod opDestMethod;
	int addLine;
} InstructionStruct;

typedef enum LINES_TYPE 
{
	DATA_TYPE,
	CODE_TYPE,
	ENTRY_TYPE,
	EXTERNAL_TYPE,
} LineTypes;

typedef enum ADDRESSING_METHOD 
{
	IMMEDIATE,
	DIRECT,
	INDIRECT_REG,
	DIRECT_REG,
	NO_ADD_METHOD
} AddressingMethod;


typedef enum DOT_DATA_TYPE 
{
	DOT_DATA_TYPE,
	DOT_STRING_TYPE
} DotDataTypes;

typedef struct opCode
{
	char* opCodeName;
	Instruction opCodeNum;
	InstructionGroup instrucGroup;
} OpCode;

typedef enum INSTRUCTION_GROUPS 
{
	TWO_OPERANDS = 1,
	ONE_OPERAND,
	NO_OPERANDS
} InstructionGroup

typedef enum INSTRUCTIONS 
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
} Instruction;
