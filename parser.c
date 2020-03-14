#include "parser.h"


#define isImmediate(operand) (operand[0] == '#' ? 1: 0)
#define isIndirectReg(operand) (((operand[0] == '*') && (operand[1] == 'r')) ? 1: 0) 
#define isDirectReg(operand) ((operand[0] == 'r') ? 1: 0)
/*
	get the line length and return the smallest value between the line length and the given max length of line.
*/
#define lenOfLine(len) (len > MAX_LINE_LENGTH ? MAX_LINE_LENGTH : len)

/* All the valid registers */
static char *VALID_REGISTERS[REGISTERS_NUM] = 
{
    "r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7"
};

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

int operandIsRegister(char *operand) 
{
    int i;
    for (i = 0; i < REGISTERS_NUM; i++) 
    {
        if (strcmp(operand, VALID_REGISTERS[i]) == 0) 
        {
            return 1;
        }
    }
    return 0;
}

int operandIsNumber(char *operand) 
{
	char* tmp = operand;
	tmp++; /* ignore # */
    if (*tmp == '+' || *tmp == '-') {
        tmp++;
    }

    while (*tmp != '\0') {
        if (!isdigit(*tmp)) {
            return 0;
        }
        tmp++;
    } 
    return 1;
}


void setSymbFromExternEntry(ParsedLineNode* line, int etLen)
{
	line->ln = line->ln + etLen;
	line->ln = trimwhitespace(line->ln);
	strcpy(line->typeHandle.et.labelName, line->ln);
}

int setStorageEntryOrExtern(ParsedLineNode* line) 
{	
	if(strncmp(line->ln, ENTRY, ENTRY_LEN) == 0)
	{
		line->lineType = ENTRY_TYPE;
		setSymbFromExternEntry(line, ENTRY_LEN);
		return ENTRY_TYPE; 
	}
	else if(strncmp(line->ln, EXTERN, EXTERN_LEN) == 0)
	{
		line->lineType = EXTERNAL_TYPE;
		setSymbFromExternEntry(line, EXTERN_LEN);
		return EXTERNAL_TYPE; 
	}
	line->error = ILLIGALE_LINE_TYPE;
	return DATA_TYPE;
}



int getAddressindMethod(ParsedLineNode* line, char* operand, SymbTable *symbTable)
{
	char* tmp;

	if(isImmediate(operand))
	{
		if(!operandIsNumber(operand))
			line->error = IMM_OPERAND_NOT_A_NUMBER;
		return IMMEDIATE;
	}
	else if (isIndirectReg(operand))
	{
		tmp = operand;
		tmp++;
		if(!operandIsRegister(tmp))
			line->error = ILL_REG_NAME;
		return INDIRECT_REG;
	}
	else if (isDirectReg(operand))
	{
		if(!operandIsRegister(operand))
			line->error = ILL_REG_NAME;
		return DIRECT_REG;
	}
	else
	{
		return DIRECT;
	}
	
}

void validateStringType(ParsedLineNode* line, char* string) 
{
    int hasClosingQuotes=0, isEmptyString = 1;

    if (*string != '"') /* String declaration must start with a quote */
    { 
        line->error = STRING_STARTS_QUOTES_IS_MISS;
        return;
    }
    string++; 

    while (*string != '\0' && *string != '\n') 
    {
        if (hasClosingQuotes && !isspace(*string)) /* If we already detected closing quotes, we're not supposed to see additional text*/
        { 
            line->error = EXTRA_TEXT_ERROR;
            return;
        }
        else if (*string == '"') 
        {
            hasClosingQuotes = 1;
        } 
        else 
        {
            isEmptyString = 0;
        }
        string++;
    }

    if (!hasClosingQuotes) 
    {
        line->error = STRING_QUOTES_IS_MISS;
        return;
    }

    else if (isEmptyString) 
    {
        line->error = EMPTY_STRING_ERROR;
        return;
    }
}


void validateDataType(ParsedLineNode* line, char* data)
{
    int commaFlag = 0, positivitySignFlag = 0;	
    char* tmp;

    tmp = data;
    tmp=trimwhitespace(tmp);
    if (*tmp == '\0') 
    {
        line->error = EMPTY_DATA_ERROR;
        return;
    } 
    else if (*tmp == ',') 
    {
        line->error = ILL_COMMA_ERROR;
        return;
    }
    else if (!isdigit(*tmp) && !(*tmp == '-'  || *tmp == '+' )) 
    {
        line->error = ILL_CHAR_ERROR;
        return;
    }

    tmp++;

    while (*tmp != '\0' && *tmp != '\n') 
    {
        if (*tmp == ',') 
        {
            if (commaFlag || (!isspace(*(tmp-1)) && !isdigit(*(tmp-1)))) /* Check if consecutive commas, or if neighbours aren't a digit or a space*/
            { 
                line->error = MULTI_CONSEC_COMMAS_ERROR;
                return;
            }
            commaFlag = 1;
            positivitySignFlag = 0;
        } 
        else if ((*tmp == '-'  || *tmp == '+' )) 
        {
            if (isdigit(*(tmp-1)) || !isdigit(*(tmp+1)) || positivitySignFlag) /* After a sign we must see a number, and we can't have multiple signs for a number*/
            { 
                line->error = ILL_POSITIVITY_SIGN_ERROR;
                return;
            }
            positivitySignFlag = 1;
        } 
        else if (isdigit(*tmp)) 
        {
            commaFlag = 0;
        } 
        else if (isspace(*tmp)) 
        {
            if (isdigit(*(tmp-1)) && isdigit(*(tmp+1))) 
            {
                line->error = MISS_COMMA_ERROR;
                return;
            }
        } 
        else 
        {
            line->error = ILL_CHAR_ERROR;
            return;
        }
        tmp++;
    }

    if (commaFlag || *tmp == ',') 
    {
        line->error = ILL_CHAR_ERROR;
        return;
    }
}


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
		line->error = EXTRANEOS_TEXT_AFTER_OPERANDS;
		return;
	}
	strcpy(line->typeHandle.instruct.opDst, operand);
	line->typeHandle.instruct.opDstMethod = am;
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
	am = getAddressindMethod(line, operand, symbTable);
	return am;
}

void getFirstOperand(ParsedLineNode* line, SymbTable *symbTable)
{
	int am;
	char* operand;
	if((operand = (char*) malloc (sizeof(char))) == NULL)
		printMemEllocateError();
	am = getOperand(line, operand, symbTable);
	strcpy(line->typeHandle.instruct.opSrc, operand);

	line->typeHandle.instruct.opSrcMethod = am;
	line->ln = trimwhitespace(line->ln);
	if (*line->ln != ',') 	/* remove , between first and second operand */
	{
		line->error = MISS_PARAM_ERROR;
		return;
	}
	line->ln++;
	getSecOperand(line, symbTable);
}

void getInstrucName(ParsedLineNode* line)
{
	int i = 0;
	char instruc[MAX_LENTH_OF_INSTRUCT+1];
	while(!isspace(line->ln[i]) && line->ln[i])
	{
		instruc[i] = line->ln[i];
		i++;
	}
	instruc[i] = '\0';
	line->ln = (line->ln)+i+1;

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
	
	dec = line->ln;
	validateDataType(line, line->ln); 
	strtok(dec, comma);
	while (dec)
	{
		dec = trimwhitespace(dec);

		if ((negative = isNegative(dec)) || isPositive(dec))
		{
			dec++;
		}
		binary = convertDecStrToBinaryStr(binary, dec, negative, MAX_WORD_LENGTH);
		addNumToDataImg(binary, dataImg);
		dec = strtok(NULL, comma);
	}
}


void parseStringType(ParsedLineNode* line, DataImg *dataImg)
{
	int len, firstData;
	char c;
	char* binary = NULL;

	line->ln = trimwhitespace(line->ln);
	validateStringType(line, line->ln);
	line->ln++; /* remove " from the start of the thering */
	len = strlen(line->ln);
	len= lenOfLine(len);
	*(line->ln+len-1) = '\0'; /* remove " from the end of the string */
	firstData = dataImg->dc == 0 ? 1:0;

	c = *(line->ln++);	
	while (c)
	{
		binary = convertNumToBinaryStr(binary, c, 0, MAX_WORD_LENGTH);
		addNumToDataImg(binary, dataImg);
		c= *(line->ln++);
	}
	addNumToDataImg(EMPTY_BMC, dataImg); /* create an empty word in binary format and save it as an instruction. */
}


int isDataStorage(ParsedLineNode * line) 
{
	int isData, isStr;
	if (((isData=strncmp(line->ln, DATA, DATA_LEN)) == 0) || ((isStr= strncmp(line->ln, STRING, STR_LEN)) == 0))
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
			line->lineType =setStorageEntryOrExtern(line);
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
	char* label, tmp[LABEL_MAX_LEN+1]; /* leave extra space for terminating null */
	int len, i=0;
	if((label = (char*) malloc (sizeof(char)*LABEL_MAX_LEN)) == NULL)
		printMemEllocateError();
	label = strchr(line->ln ,':');
	len = (strlen(line->ln)-strlen(label));
	strncpy(tmp, line->ln, len);
	tmp[len] = '\0';

	line->ln = line->ln+len+1; /* 1- for excluding ':' from ln */
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
		line->symbValue = (char*)malloc(len+1);
		strncpy(line->symbValue, tmp, len+1); /* +1 for including '\0' */

		while(isalnum(tmp[i])) i++; /* all the chars of the label are alphabetic or numeric */

		if (tmp[i] != '\0')
		{
			line->error = ILLIGALE_CHAR_ERROR;
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
	return ((tmp = strchr(ln ,':')) != NULL);
}
