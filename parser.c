#include "utils.h"
#define START 1
#define EXTERN_LEN 6
#define NUM_OF_INSTRUCTION 16

static OpCode opCodesMatch[NUM_OF_INSTRUCTION] = {
	{"mov", MOV, TWO_OPERANDS},
    {"cmp", CMP, TWO_OPERANDS},
    {"add", ADD, TWO_OPERANDS},
    {"sub", SUB, TWO_OPERANDS},
    {"lea", LEA, TWO_OPERANDS},
    {"clr", CLR, ONE_OPERAND},
    {"not", NOT, ONE_OPERAND},
    {"inc", INC, ONE_OPERAND},
    {"dec", DEC, ONE_OPERAND},
    {"jmp", JMP, ONE_OPERAND},
    {"bne", BNE, ONE_OPERAND},
    {"red", RED, ONE_OPERAND},
    {"prn", PRN, ONE_OPERAND},
    {"jsr", JSR, ONE_OPERAND},
    {"rts", RTS, NO_OPERANDS},
    {"stop", STOP, NO_OPERANDS}
}

void updateValuesInSymbTable(SymbTable *symbTable, SymbNode *symbNode)
{
	if (symbTable->head == NULL)
	{
		symbTable->counter = START;
		symbTable->head = symbNode;
		symbTable->tail = symbNode;
	}
	else 
	{
		if(isSymbExist(symbTable, symbNode->symbValue))
		{
			symbTable->tail.next = symbNode;
			symbTable->counter++;
		}
		else
		{
			printError(SYMBOL_ALREADY_EXIST_ERROR);
		}
	}
}

parseLineSec(ParsedLineNode* line, InstructImg *instructImg, DataImg *dataImg, SymbTable *symbTable)
{
	if(line->symbType == DATA | line->symbType == EXTERN)
	{
		return;
	}
	else if (line->symbType == ENTRY)
	{
		//do line 6
		return;
	}
	//do line 7
	instructImg->ic = instructImg->ic + L;
	return;
}

parseLine(ParsedLineNode* line, InstructImg *instructImg, DataImg *dataImg, SymbTable *symbTable)
{
	void (parseDType[2])(ParsedLineNode*, DataImg*) = {parseDataType, parseStringType};

	if(isFirstFieldSymb(line))
	{
		line->symbFlag = 1;
		validateSymb(line);
	}
	else
	{
		line->symbFlag = 0;
	}
	if(isItDataInsturction(line))
	{
		if (line->lineType == EXTERN)
		{
			return handleExternCase(line, dataImg, symbNode);
		}
		else if (line->symbFlag)
		{
			SymbNode *symbNode
			createSymbNode(line, dataImg, symbNode, DATA);
			updateValuesInSymbTable(symbTable, symbNode);
		}
		parseDType[line->dataType](line, dataImg);
		return;
	}
	else 
	{
		if (line->symbFlag)
		{
			SymbNode *symbNode
			createSymbNode(line,dataImg, instructImg, symbNode, CODE)
			updateValuesInSymbTable(symbTable, symbNode);
		}
		getInstrucName(line);
		getOperandsStruct(line);
		calculateL(line);
		buildBinaryCode(line);
		instructImg->ic = instructImg->ic + L
	}	
}

int isFirstFieldSymb(ParsedLineNode* line, SymbTable * symbTable)
{
	char* label;
	if (strchr(line->line ,":") == NULL)
		return 0;
	else
		return 1;
}

void validateSymb(ParsedLineNode* line, SymbTable * symbTable)
{
	char* label;
	label = strchr(line->line ,":");
	if ((strlen(line)-strlen(label)) > LABEL_MAX_LEN)
	{
		 printError(LABEL_EXCEEDED_MAX_LEN_ERROR, line->fileName);
		 line->error = LABEL_EXCEEDED_MAX_LEN_ERROR;
	}
	else
	{
		getSymbValue(line, label);
		if (isSymbInSymbTable(label, symbTable))
		{
			printError(DUP_LABEL_NAME_ERROR);
			line->error = DUP_LABEL_NAME_ERROR;
		}
	}
}

int isItDataInsturction(ParsedLineNode * line)
{
	trimwhitespace(line->line);
	if (line->line[FIRST_INDEX] == '.')
	{
		line->line++;
		if(isDataStorageInsturction(line))
		else
		{
			setStorageEntryOrExtern() 
		}
		return 1;
	}
	else
	{
		line->instructLine = 1;
		return 0;
	}
}	

int isDataStorageInsturction(ParsedLineNode * line) 
{
	if (strncmp(line->line, DATA, 4) == 0)
	{
		line->lineType = DATA;
		line->dataType = DOT_DATA_TYPE;
		return 1;
	}
	else if (strncmp(line->line, STRING, 6) == 0)
	{
		line->lineType = DATA;
		line->dataType = DOT_STRING_TYPE;
		return 1;
	}
	return 0;
}

void setStorageEntryOrExtern() 
{	
	if(strncmp(line->line, "entry", 5) == 0)
	{
		line->lineType = ENTRY;
	}
	else if(strncmp(line->line, "extern", EXTERN_LEN) == 0)
	{
		line->lineType = EXTERN;
	}
}

void getInstrucName(ParsedLineNode* line)
{
	int i = 0;
	char instruc[MAX_LENTH_OF_INSTRUCT];
	trimwhitespace(line->line);
	while(!isspace(line->line[i]))
	{
		instruc[i] = line->line[i];
		i++;
	}
	instruc[i] = '\0';
	i = 0;
	while (i<NUM_OF_INSTRUCTION && strcmp(instruc, opCodesMatch[i].opCodeName))
	{
		i++;
	}
	line->line = (line->line)+i;
	if (i == NUM_OF_INSTRUCTION)
	{
		printError(INSTRUCTION_DOES_NOT_EXIST_ERROR);
		line->error= INSTRUCTION_DOES_NOT_EXIST_ERROR;
	}
	else
	{
		line->typeHandle = opCodesMatch[i].opCodeNum;
	}

}

AddressingMethod getOperand(char* line, char* operand, SymbTable *symbTable)
{
	AddressingMethod am;
	int i = 0;
	operand = (char*)malloc(LABEL_MAX_LEN* sizeof(char));
	trimwhitespace(line->line);
	while(!isspace(line->line[i]))
	{
		operand[i] = line->line[i];
		i++;
	}
	operand[i] = '\0';

	line = line+i;
	am = getAddressindMethod(operand, symbTable);
	return am;
}


char * getFirstOperand(ParsedLineNode* line, SymbTable *symbTable)
{
	AddressingMethod am;
	char* operand;
	am = getOperand(line->line,operand, symbTable);
	strcpy(line->typeHandle.opSrc, operand);
	line->typeHandle.opSrcMethod = am;
	getSecOperand(line, symbTable);
}


char * getSecOperand(ParsedLineNode *line, SymbTable *symbTable)
{
	AddressingMethod am;
	char* operand;
	am = getOperand(line->line, operand, symbTable);
	strcpy(line->typeHandle.opDst, operand)
	line->typeHandle.opDstMethod = am
}

bool isDirect(operand, SymbTable *symbTable)
{
	SymbNode *tmp;
	tmp = (SymbNodes*)malloc(sizeof(SymbNodes))
	tmp = symbTable->head;
	for (int i = 0; i < symbTable->counter; i++)
	{
		if(strcmp(operand, tmp->symbName))
		{
			tmp = tmp->next;
		}
		else
		{
			return true;
		}
	}
	return false;
}

AddressingMethod getAddressindMethod(char* operand, SymbTable *symbTable)
{
	if(isImmediate(operand))
	{
		return IMMEDIATE;
	}
	else if (isDirect(operand, symbTable))
	{
		return 	DIRECT;
	}
	else if (isIndirectReg(operand))
	{
		return INDIRECT_REG;
	}
	else if (isDirectReg(operand))
	{
		return DIRECT_REG;
	}
	else
	{
		printError(NON_EXSIT_ADDRESSING_METHOD);
	}
}

getOperandsStruct(ParsedLineNode* line, SymbTable *symbTable) 
{
	line->typeHandle.opSrcMethod = NULL;
	line->typeHandle.opDstMethod = NULL;

	switch(opCodesMatch[line->typeHandle.opCode].instrucGroup)
	{
		case TWO_OPERANDS: 
			getFirstOperand(line, symbTable);
			break;
		case ONE_OPERAND: 
			getSecOperand(line);
			break;
		case NO_OPERANDS:
			break;
	}
}

calculateL(line)
{
	//do line 13
}

buildBinaryCode(line)
{
	//do line 14
}

void getSymbValue(ParsedLineNode* line, char* label)
{
	int lenLabel;
	lenLabel = strlen(line->line)- strlen(label)
	strncpy(line->symbValue,line, lenLabel)
	line = line + lenLabel;
}

void getSymbFromExtern(symbNode, char* line)
{
	line = line + EXTERN_LEN
	trimwhitespace(line);
	strcpy(symbNode->symbName, line);
}

void parseDataType(ParsedLineNode* line, DataImg *dataImg)
{
	char *dec;
	char *binary;
	int negative = 0;
	char comma[2] = ",";
	strcpy(dec, line->line);
	strtok(dec, comma);
	while (dec != NULL)
	{
		trimwhitespace(dec);
		if (dec[0] == '-')
		{
			negative = 1;
			dec++;
		}
		else if (dec[0] == '+') 
		{
			dec++;
		}
		convertDecStrToBinaryStr(dec, binary, negative, MAX_WORD_LENGTH)
		addNumToDataImg(binary, dataImg);
		dec = strtok(NULL, comma)
	}
}

void parseStringType(ParsedLineNode* line, DataImg *dataImg)
{
	int len;
	char c;
	char* ascii;

	trimwhitespace(line->line);
	len = strlen(line->line);
	lenOfLine(len);
	for (int i = 0; i < len; i++)
	{
		c = line->line[i];
	 	if (isalpha(c))
	 	{
	 		convertCharToAscii(c, ascii)
	 		addNumToDataImg(binary, dataImg);
	 	}
	}
}

