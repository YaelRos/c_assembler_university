#include "parser.h"
#include "utils.h" 
#include "dataStruct.h"

#define isImmediate(operand) (operand[0] == '#' ? 1: 0)
#define isIndirectReg(operand) (((operand[0] == '*') && (operand[1] == 'r')) ? 1: 0) 
#define isDirectReg(operand) ((operand[0] == 'r') ? 1: 0)
/*
	get the line length and return the smallest value between the line length and the given max length of line.
*/
#define lenOfLine(len) (len = (len > MAX_LINE_LENGTH ? MAX_LINE_LENGTH : len))


void setSymbFromExternEntry(ParsedLineNode* line, int etLen)
{
	line->ln = line->ln + etLen;
	trimwhitespace(line->ln);
	strcpy(line->typeHandle.et.labelName, line->ln);
}

/*
void setStorageEntryOrExtern(ParsedLineNode * line) 
{	
	if(strncmp(line->ln, ENTRY, ENTRY_LEN) == 0)
	{
		line->lineType = ENTRY_TYPE;
		setSymbFromExternEntry(symbNode, line->ln, ENTRY_LEN);
	}
	else if(strncmp(line->ln, EXTERN, EXTERN_LEN) == 0)
	{
		line->lineType = EXTERN_TYPE;
		setSymbFromExternEntry(symbNode, line->ln, EXTERNAL_TYPE);
	}
}
*/

int isDirect(char* operand, SymbTable *symbTable)
{
	SymbNode *tmp;
	struct externNode exNode; 
	int i;
	if ((tmp = (SymbNode*)malloc(sizeof(SymbNode))) == NULL)
	{
		printMemEllocateError();
		exit(1);
	}
	tmp = symbTable->head;
	for (i = 0; i < symbTable->counter; i++)
	{
		if(strcmp(operand, tmp->symbName))
		{
			tmp = tmp->next;
		}
		else
		{
			exNode.memAddr = tmp->symbAddr;
			strcpy(exNode.SymbName, operand);
			symbTable->exTable.externalLabel[symbTable->exTable.counter++] = exNode;
			return 1;
		}
	}
	return 0;
}

int getAddressindMethod(ParsedLineNode* line, char* operand, SymbTable *symbTable)
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
		line->error = NON_EXSIT_ADDRESSING_METHOD;
		return NO_ADD_METHOD;
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
	char* operand = NULL;
	am = getOperand(line, operand, symbTable);
	if (*line->ln != '\0' || *line->ln != '\n')
	{
		line->error = EXTRA_TEXT_ERROR;
		return;
	}
	strcpy(line->typeHandle.instruct.opDst, operand);
	line->typeHandle.instruct.opDstMethod = am;
}

int getOperand(ParsedLineNode* line, char* operand, SymbTable *symbTable)
{
	int am;
	int i = 0;
	operand = (char*)malloc(LABEL_MAX_LEN* sizeof(char));
	trimwhitespace(line->ln);
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

	line->ln = line->ln+i-1;
	am = getAddressindMethod(line, operand, symbTable);
	return am;
}

void getFirstOperand(ParsedLineNode* line, SymbTable *symbTable)
{
	int am;
	char* operand = NULL;
	
	am = getOperand(line, operand, symbTable);
	strcpy(line->typeHandle.instruct.opSrc, operand);
	line->typeHandle.instruct.opSrcMethod = am;
	trimwhitespace(line->ln);
	if (*line->ln != ',') 	/* remove , between first and second operand */
	{
		line->error = MISS_COMMA_ERROR;
		return;
	}
	line->ln++;
	getSecOperand(line, symbTable);
}

void getInstrucName(ParsedLineNode* line)
{
	int i = 0;
	char instruc[MAX_LENTH_OF_INSTRUCT+1];
	trimwhitespace(line->ln);
	while(!isspace(line->ln[i]))
	{
		instruc[i] = line->ln[i];
		i++;
	}
	instruc[i] = '\0';

	i = 0;
	while (i<NUM_OF_INSTRUCTION && strcmp(instruc, opCodesMatch[i].opCodeName))
	{
		i++;
	}
	line->ln = (line->ln)+i;
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
	{
		printMemEllocateError();
		exit(1);
	}
	strcpy(dec, line->ln);
	/* validateDataType(dec); */
	strtok(dec, comma);
	while (dec != NULL)
	{
		trimwhitespace(dec);

		if ((negative = isNegative(dec)) || isPositive(dec))
		{
			dec++;
		}
		convertDecStrToBinaryStr(binary, dec, negative, MAX_WORD_LENGTH+1);
		addNumToDataImg(binary, dataImg);
		dec = strtok(NULL, comma);
	}
}


void parseStringType(ParsedLineNode* line, DataImg *dataImg)
{
	int len, i;
	char c;
	char* binary = NULL;
	/* validateStringType(line->ln); */
	trimwhitespace(line->ln);
	len = strlen(line->ln);
	lenOfLine(len);

	for (i = 0; i < len; i++)
	{
		c = line->ln[i];
		convertNumToBinaryStr(binary, c - '0', 0, MAX_WORD_LENGTH);
		addNumToDataImg(binary, dataImg);
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
		return 1;
	}
	return 0;
}


int isGuidanceType(ParsedLineNode* line)
{
	trimwhitespace(line->ln);
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
	char* label;
	label = strchr(line->ln ,':');
	if ((strlen(line->ln)-strlen(label)) > LABEL_MAX_LEN) /*max label size is 31*/
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


int isFirstFieldSymb(char* ln)
{
	return (strchr(ln ,':') != NULL);
}
