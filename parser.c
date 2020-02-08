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


void parseSecLine(ParsedLineNode* line, InstructImg *instructImg, DataImg *dataImg, SymbTable *symbTable)
{
	if(line->symbType == DATA_TYPE | line->symbType == EXTERN_TYPE)
	{
		return;
	}
	else if (line->symbType == ENTRY_TYPE)
	{
		SymbNode *symbNode;
		if(getSymbFeature(symbTable, line->typeHandle->et.labelName, symbNode) == NULL)
		{
			printError(LABEL_NOT_IN_SYMB_TABLE);
			line->error = LABEL_NOT_IN_SYMB_TABLE;
		}
		else
		{
			symbNode->symbType = ENTRY_TYPE;
		}
		return;
	}
	else 
	{
		updateBinaryMachineCode(line, instructImg, symbTable);
	}
	instructImg->ic = instructImg->ic + L;
	return;
}



void addDirectLabelToMem(ParsedLineNode* line, InstructImg *instructImg, 
	SymbTable *symbTable)
{
	SymbNode *symbNode;
	
	if(getSymbFeature(symbTable, line->symbValue, symbNode) == NULL)
	{
		printError(LABEL_NOT_IN_SYMB_TABLE);
		line->error = LABEL_NOT_IN_SYMB_TABLE;
	}
	else
	{
		InstructionNode *in;
		char *dst; 
		convertNumToBinaryStr(symbNode->symbAddr ,dst, false, IMM_VAL_LEN_BITS)
		memcpy(in->instruction, dst, IMM_VAL_LEN_BITS);
		instructImg->instructions[instructImg->ic++]=in; 
	}
	if (symbNode->symbType == EXTERN_TYPE)
	{
		memcpy(in->instruction+IMM_VAL_LEN_BITS, E, ARE_LEN_BITS);
	}
	else
	{
		memcpy(in->instruction+IMM_VAL_LEN_BITS, R, ARE_LEN_BITS);
	}
}



void setStorageEntryOrExtern(ParsedLineNode * line) 
{	
	if(strncmp(line->line, ENTRY, ENTRY_LEN) == 0)
	{
		line->lineType = ENTRY_TYPE;
		setSymbFromExternEntry(symbNode, line->line, ENTRY_LEN)
	}
	else if(strncmp(line->line, EXTERN, EXTERN_LEN) == 0)
	{
		line->lineType = EXTERN_TYPE;
		setSymbFromExternEntry(symbNode, line->line, EXTERN_LEN)
	}
}

void getInstrucName(ParsedLineNode* line)
{
	int i = 0;
	char instruc[MAX_LENTH_OF_INSTRUCT+1];
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
	struct externNode exNode; 
	for (int i = 0; i < symbTable->counter; i++)
	{
		if(strcmp(operand, tmp->symbName))
		{
			tmp = tmp->next;
		}
		else
		{
			exNode.memAddr = tmp->symbAddr;
			strcpy(exNode.SymbName, operand);
			symbTable->exTable[symbTable->exTable->counter++] = exNode;
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




void setSymbFromExternEntry(ParsedLineNode* line, int etLen)
{
	line->line = line->line + etLen
	trimwhitespace(line->line);
	memcpy(line->typeHandle->et.labelName, line->line);
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

		if ((negative = isNegative(dec)) | isPositive(dec))
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
	char* binary;

	trimwhitespace(line->line);
	len = strlen(line->line);
	lenOfLine(len);
	for (int i = 0; i < len; i++)
	{
		c = line->line[i];
	 	if (isalpha(c))
	 	{
	 		convertNumToBinaryStr(c , binary, false, MAX_WORD_LENGTH)
	 		addNumToDataImg(binary, dataImg);
	 	}
	}
}

/*
	Check if the line is Data line.
	@param ParsedLineNode * line - The object of the current line. 
	@return int - A flag representing if the line is Data line or not. 0 = False, 1 = True
*/
int isDataStorage(ParsedLineNode * line) 
{
	int isData, isStr;
	if (((isData=strncmp(line->line, DATA, DATA_LEN)) == 0) | 
		((isStr= strncmp(line->line, STRING, STRING_LEN)) == 0))
	{
		line->lineType = DATA_TYPE;
		line->typeHandle->dataType = (isData == 0 ? DOT_DATA_TYPE : DOT_STRING_TYPE);
		return 1;
	}
	return 0;
}

/*
	Check if the line is guidance line.
	if there is '.' in the beginning of the line, it indicates that it's a guidance line.
	@param ParsedLineNode * line - The object of the current line. 
	@return int - A flag representing if the line is guidance line or not. 0 = False, 1 = True
*/
int isGuidanceType(ParsedLineNode* line)
{
	trimwhitespace(line->ln);
	if (*(line->ln) == '.')
	{
		line->ln++;
		if(isDataStorage(line))
		else
		{
			setStorageEntryOrExtern(line);
		}
		return 1;
	}
	else
	{
		line->lineType = CODE_TYPE;
		return 0;
	}
}	

/*
	Set the symbol value in the line object to be 
	the symbol which defined at the beginning of the line,
	and move the string pointer forward as the length of the symbol value.
	@param ParsedLineNode* line - The object of the current line. 
	@param char* label - The substring of line->ln which start at the end of the label.
*/
void setSymbValue(ParsedLineNode* line, char* label)
{
	int lenLabel;
	lenLabel = strlen(line->ln)- strlen(label);
	memcpy(line->symbValue,line->ln, lenLabel);
	line->ln = line->ln + lenLabel;
}

/*
	Check if the symbol which defined at the beginning of the line is valid,
	if is not valid - will update it as an error. 
	if there is ':' in the line, it indicates that it's a definition.
	@param ParsedLineNode* line - The object of the current line. 
	@param SymbTable* symbTable - The symbol table
*/
void validateSymb(ParsedLineNode* line, SymbTable * symbTable)
{
	char* label;
	label = strchr(line->ln ,":");
	if ((strlen(line)-strlen(label)) > LABEL_MAX_LEN) /*max label size is 31*/
	{
		line->error = LABEL_EXCEEDED_MAX_LEN_ERROR;
	} 
	else if(!isalpha(*(line->ln))) /*first char of label is alphabetic*/
	{
		line->error = LABEL_START_ERROR;
	}
	else
	{
		setSymbValue(line, label);
		strcpy(label, line->symbValue);
		while(isalnum(*label)) label++; /* all the chars of the label are alphabetic or numeric */
		if (label != '\0')
		{
			line->error = ILLIGALE_CHAR_ERROR;
		}
		else if (symbTableContains(line->symbValue, symbTable)) /* each definition of label can appear one time */
		{
			line->error = DUP_LABEL_NAME_ERROR;
		}
		else if (isSavedWord(line->symbValue)) /* label name can't be equal to a saved word in c language */
		{
			line->error = LABEL_EQUAL_TO_SAVED_WORD_ERROR;
		}
	}
}

/*
	Check if there is a symbol definition at the beginning of the line.
	if there is ':' in the line, it indicates that it's a definition.
	@param char* ln - The string of the current line. 
	@return int - A flag representing if we found a symbol or not. 0 = False, 1 = True
*/
int isFirstFieldSymb(char* ln)
{
	return (strchr(ln ,":") != NULL)
}
