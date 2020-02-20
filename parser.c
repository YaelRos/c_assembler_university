#include "parser.h"


#define isImmediate(operand) (operand[0] == '#' ? 1: 0)
#define isIndirectReg(operand) (((operand[0] == '*') && (operand[1] == 'r')) ? 1: 0) 
#define isDirectReg(operand) ((operand[0] == 'r') ? 1: 0)
/*
	get the line length and return the smallest value between the line length and the given max length of line.
*/
#define lenOfLine(len) (len > MAX_LINE_LENGTH ? MAX_LINE_LENGTH : len)

OpCode opCodesMatch[NUM_OF_INSTRUCTION] = {
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
};

void setSymbFromExternEntry(ParsedLineNode* line, int etLen)
{
	line->ln = line->ln + etLen;
	line->ln = trimwhitespace(line->ln);
	strcpy(line->typeHandle.et.labelName, line->ln);
}

void setStorageEntryOrExtern(ParsedLineNode* line) 
{	
	if(strncmp(line->ln, ENTRY, ENTRY_LEN) == 0)
	{
		line->lineType = ENTRY_TYPE;
		setSymbFromExternEntry(line, ENTRY_LEN);
	}
	else if(strncmp(line->ln, EXTERN, EXTERN_LEN) == 0)
	{
		line->lineType = EXTERNAL_TYPE;
		setSymbFromExternEntry(line, EXTERNAL_TYPE);
	}
}


void updateExTable(int mamadd, char* operand, SymbTable *symbTable)
{
	int i;
	SymbNode *tmp;
	struct externNode exNode; 

	tmp = symbTable->head;
	for (i = 0; i < symbTable->counter; i++)
	{
		if(strcmp(operand, tmp->symbName))
		{
			tmp = tmp->next;
		}
		else
		{
			/* if a label came from extern source than add the instruction line address to the exTable */
			if(tmp->symbType == EXTERNAL_TYPE)
			{
				exNode.memAddr = mamadd;
				strcpy(exNode.SymbName, operand);
				symbTable->exTable.externalLabel[symbTable->exTable.counter++] = exNode;
			}
		}
	}
}

int getAddressindMethod(ParsedLineNode* line, char* operand, SymbTable *symbTable)
{
	printf("getAddressindMethod - isImmediate: %c\n", operand[0]);
	if(isImmediate(operand))
	{
		return IMMEDIATE;
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
		return DIRECT;
	}
	
}


void validateStringType(ParsedLineNode* line, char * string)
{
	/*check for  ""
	MISS_PARAM_ERROR check for an empty data
	EXTRA_TEXT_ERROR
	*/
}

/*

void validateDataType(ParsedLineNode* line, char* data)
{	
	char* validData;
	char comma[2] = ",";
	int commaFlag = 0, positivitySignFlag = 0;

	validData = (char*)malloc(sizeof(char));
	strcpy(validData, data);

    if (*validData == '\0') 
    {
        line->error = EMPTY_DATA_ERROR;
        return;
    } 
    else if (*validData == ',') 
    {
        line->error = ILL_COMMA_ERROR;
        return;
    } 
    else if (!isdigit(*validData) && !(*validData == '-'  || *validData == '+' ))
    {
    	line->error = ILL_DATA_TYPE_ERROR;
    	return;
    }
    else
    {
    	validData++;
    } 

    while (*validData != '\0' && *validData != '\n') 
    {
    	if (*validData == ',') 
    	{
            if (commaFlag || (!isspace(*(validData-1)) && !isdigit(*(validData-1)))) { 
             // Check if consecutive commas, or if neighbours aren't a digit or a space
                pr->errorType = ILLEGAL_COMMA_IN_DATA_DECLARATION;
                return;
            }
            commaFlag = 1;
            positivitySignFlag = 0;
        } 
        else if ((*rawData == '-'  || *rawData == '+' )) 
        {
            if (isdigit(*(rawData-1)) || !isdigit(*(rawData+1)) || positivitySignFlag) {  After a sign we must see a number, and we can't have multiple signs for a number
                pr->errorType = ILLEGAL_POSITIVITY_SIGN_IN_DATA_DECLARATION;
                return;
            }
            positivitySignFlag = 1;
        } 
        else if (isdigit(*rawData)) 
        {
            commaFlag = 0;
        } 
        else if (isspace(*rawData))
        {
            if (isdigit(*(rawData-1)) && isdigit(*(rawData+1))) {
                pr->errorType = ILLEGAL_DATA_DECLARATION_EXTRANEOUS_SPACE;
                return;
            }
        } 
        else 
        {
            pr->errorType = ILLEGAL_DATA_DECLARATION_CHARACTER;
            return;
        }
        validData++;
    }

    if (commaFlag || *rawData == ',') 
    {
        pr->errorType = ILLEGAL_DATA_DECLARATION_EXTRANEOUS_COMMA;
        return;
    }

	MULTI_CONSEC_COMMAS_ERROR
	 After a sign we must see a number, and we can't have multiple signs for a number
	MISS_PARAM_ERROR
	MISS_COMMA_ERROR
	EXTRA_TEXT_ERROR
	SET_MEM_NOT_INT_ERROR
}
*/

void getSecOperand(ParsedLineNode *line, SymbTable *symbTable)
{
	int am;
	char* operand;
	if((operand = (char*)malloc(sizeof(char)))== NULL)
		printMemEllocateError();
	am = getOperand(line, operand, symbTable);
	line->ln = trimwhitespace(line->ln);
	if (*(line->ln) != '\0' && *(line->ln) != '\n')
	{
		line->error = EXTRA_TEXT_ERROR;
		return;
	}
	strcpy(line->typeHandle.instruct.opDst, operand);
	line->typeHandle.instruct.opDstMethod = am;
	printf("getSecOperand - opDstMethod:%d\n", line->typeHandle.instruct.opDstMethod);
}

int getOperand(ParsedLineNode* line, char* operand, SymbTable *symbTable)
{
	int am;
	int i = 0;
	line->ln = trimwhitespace(line->ln);
	if (*line->ln == '\0' || *line->ln == '\n')
	{
		line->error = NO_OPERANDS_ERROR;
		return NO_ADD_METHOD;
	}
	else if (*line->ln == ',')
	{
		line->error = MISS_PARAM_ERROR;
		return NO_ADD_METHOD;
	}
	while(!isspace(line->ln[i]) && line->ln[i] != ',' && line->ln[i] != '\0' && line->ln[i] != '\n')
	{
		operand[i] = line->ln[i];
		i++;
	}
	operand[i] = '\0';
	line->ln = line->ln+i;
	printf("getOperand - line->ln: %s\n", line->ln);
	am = getAddressindMethod(line, operand, symbTable);
	return am;
}

void getFirstOperand(ParsedLineNode* line, SymbTable *symbTable)
{
	int am;
	char* operand;
	if((operand = (char*) malloc (sizeof(char))) == NULL)
	{
		printMemEllocateError();
	}
	am = getOperand(line, operand, symbTable);
	strcpy(line->typeHandle.instruct.opSrc, operand);
	printf("getFirstOperand - opsrc: %s\n", line->typeHandle.instruct.opSrc);

	line->typeHandle.instruct.opSrcMethod = am;
	line->ln = trimwhitespace(line->ln);
	if (*line->ln != ',') 	/* remove , between first and second operand */
	{
		line->error = MISS_COMMA_ERROR;
		return;
	}
	line->ln++;
	printf("getFirstOperand- line->ln: %s\n", line->ln);
	getSecOperand(line, symbTable);
}

void getInstrucName(ParsedLineNode* line)
{
	int i = 0;
	char instruc[MAX_LENTH_OF_INSTRUCT+1];
	printf("getInstrucName - line:%s\n", line->ln);
	while(!isspace(line->ln[i]) && line->ln[i])
	{
		instruc[i] = line->ln[i];
		i++;
	}
	instruc[i] = '\0';
	printf("getInstrucName - instruc:%s\n", instruc);
	line->ln = (line->ln)+i+1;
	printf("getInstrucName - line: %s\n", line->ln);
	i = 0;
	while (i<NUM_OF_INSTRUCTION && strcmp(instruc, opCodesMatch[i].opCodeName))
	{
		i++;
	}
	line->typeHandle.instruct.opCode = opCodesMatch[i].opCodeNum;
	if (i == NUM_OF_INSTRUCTION)
	{
		line->error= INSTRUCTION_DOES_NOT_EXIST_ERROR;
	}
	else
	{
		line->typeHandle.instruct.opCode = opCodesMatch[i].opCodeNum;
	}
}

void parseDataType(ParsedLineNode* line, DataImg *dataImg)
{
	char *dec, *binary = NULL;
	int negative = 0;
	char comma[2] = ",";
	if((dec = (char *) malloc(sizeof(char)))==NULL)
		printMemEllocateError();
	strcpy(dec, line->ln);
	/* validateDataType(dec); */
	strtok(dec, comma);
	printf("parseDataType - dec: %s\n", dec);
	while (dec != NULL)
	{
		dec = trimwhitespace(dec);

		if ((negative = isNegative(dec)) || isPositive(dec))
		{
			dec++;
		}
		if((binary = (char *) malloc(sizeof(char)))==NULL)
		{
			printMemEllocateError();
		}
		printf("parseDataType - dec1: %s\n", dec);
		convertDecStrToBinaryStr(binary, dec, negative, MAX_WORD_LENGTH);
		printf("parseDataType - binary: %s\n", binary);
		addNumToDataImg(binary, dataImg);
		dec = strtok(NULL, comma);
	}
}


void parseStringType(ParsedLineNode* line, DataImg *dataImg)
{
	int len;
	char c;
	char* binary = NULL;
	
	printf("parseStringType - %s\n", "here");
	line->ln = trimwhitespace(line->ln);
	/* validateStringType(line->ln); */
	line->ln++; //remove after validate it here becuase of the "

	len = strlen(line->ln);
	*(line->ln+len-1) = '\0'; //remove after validate it here becuase of the "
	len= lenOfLine(len);
	printf("parseStringType - len:%d\n", len);
	c = *(line->ln);
	while (c)
	{
		if(isalpha(c))
		{
			if((binary = (char *) malloc(sizeof(char)))==NULL)
			{
				printMemEllocateError();
			}
			convertNumToBinaryStr(binary, c, 0, MAX_WORD_LENGTH);
			printf("parseStringType - convert:%s\n", binary);
			addNumToDataImg(binary, dataImg);
		}
		else 
		{
			line->error = NOT_AN_ALPHA_ERROR;
			return;
		}
		c= *(line->ln++);
	}
}


int isDataStorage(ParsedLineNode * line) 
{
	int isData, isStr;
	if (((isData=strncmp(line->ln, DATA, DATA_LEN)) == 0) | 
		((isStr= strncmp(line->ln, STRING, STR_LEN)) == 0))
	{
		line->lineType = DATA_TYPE;
		line->typeHandle.dataType = (isData == 0 ? DOT_DATA_TYPE : DOT_STRING_TYPE);
		line->ln = line->ln+(isData == 0 ? DATA_LEN : STR_LEN);
		return 1;
	}
	return 0;
}


int isGuidanceType(ParsedLineNode* line)
{
	line->ln = trimwhitespace(line->ln);
	if (*(line->ln) == '.')
	{
		line->ln++;
		if(!isDataStorage(line))
			setStorageEntryOrExtern(line);
		return 1;
	}
	else
	{
		line->lineType = CODE_TYPE;
		return 0;
	}
}	


void validateSymb(ParsedLineNode* line, SymbTable * symbTable)
{
	char* label, tmp[LABEL_MAX_LEN];
	int len, i=0;
	if((label = (char*) malloc (sizeof(char)*LABEL_MAX_LEN)) == NULL)
	{
		printMemEllocateError();
	}

	label = strchr(line->ln ,':');
	len = (strlen(line->ln)-strlen(label));
	memcpy(tmp, line->ln, len);
	tmp[len] = '\0';
	printf("validateSymb - tmp:%s\n", tmp);

	line->ln = line->ln+len+1; /* 1- for excluding ':' from ln */
	printf("validateSymb - line->ln:%s\n", line->ln);
	if (len > LABEL_MAX_LEN) /*max label size is 31*/
	{
		line->error = LABEL_EXCEEDED_MAX_LEN_ERROR;
	} 
	else if(!isalpha(tmp[i])) /*first char of label is alphabetic*/
	{
		line->error = LABEL_START_ERROR;
	}
	else
	{
		memcpy(line->symbValue, tmp, len+1); /* +1 for including '\0' */
		printf("validateSymb - line->symbValue:%s\n", line->symbValue);

		while(isalnum(tmp[i])) i++; /* all the chars of the label are alphabetic or numeric */

		if (tmp[i] != '\0')
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


int isFirstFieldSymb(char* ln)
{
	char *tmp;
	tmp = strchr(ln ,':');
	return (tmp != NULL);
}
