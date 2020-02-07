#include "error.h"
#include "util.h"
#include "dataStruct.h"

int createOutputFiles()
{
	
}

void freeMem()
{
	
}


int isEmptyLine ()
int isCommentLine()

void rmvSpace(char* str) 
{
	while(str[FIRST_INDEX] == ' ' or str[FIRST_INDEX] == '\t')
	{
		str++;
	}
}

// Note: This function returns a pointer to a substring of the original string.
// If the given string was allocated dynamically, the caller must not overwrite
// that pointer with the returned value, since the original pointer must be
// deallocated using the same allocator with which it was allocated.  The return
// value must NOT be deallocated using free() etc.
char *trimwhitespace(char *str)
{
  char *end;

  // Trim leading space
  while(isspace((unsigned char)*str)) str++;

  if(*str == 0)  // All spaces?
    return str;

  // Trim trailing space
  end = str + strlen(str) - 1;
  while(end > str && isspace((unsigned char)*end)) end--;

  // Write new null terminator character
  end[1] = '\0';

  return str;
}

void handleExternCase(ParsedLineNode* line,DataImg* dataImg,InstructImg* instructImg, SymbNode* symbNode, SymbTable* symbTable)
{
	SymbNode *symbNode
	if (line->symbFlag)
	{
		printWarning(LABEL_BEFORE_EXTERN);
	}
	initSymbNode(line->typeHandle->et.labelName, dataImg, instructImg, symbNode, EXTERN);
	updateValuesInSymbTable(symbTable, symbNode);
}

void parsedLineNodeCpy(ParsedLineNode *dst, ParsedLineNode *src)
{
	strcpy(dst->fileName,src->fileName);
	dst->lineNum = src->lineNum;
	strcpy(dst->line,src->line);
	dst->error = src->error;
	dst->symbFlag = src->symbFlag;
	strcpy(dst->symbValue, src->symbValue);
	dst->symbType = src->symbType;
	strcpy(dst->instructName,src->instructName);
	dst->parseLine *next = NULL;
}

int isSymbExist(SymbTable *symbTable, char* symbValue)
{
	int i;
	SymbNode *symbNode;
	if((symbNode = (SymbNodes*)malloc(sizeof(SymbNodes))) == NULL)
	{
		//to do
	}
	symbNode = ymbTable->head;
	for (int i = 0; i < symbTable->counter; ++i)
	{
		if ((symbNode->symbValue) == symbValue)
			return 1;
		symbNode = symbNode->next;
	}
	return 0;
}

void printWarning(char * str)
{

}

void convertDecStrToBinaryStr(char* decSrc,char* binaryDst, bool negative, int len)
{
    int mask, num;
    num = atoi(decSrc);
	if (isValidNumber(num))
	{
		if (isNegative) { /* Handle negative numbers - For two's compliment we need to flip the number and add +1. We'll add before flipping since it makes it easier (And the results are the same!) */
	        num += 1;
	        num *= -1;
	    }
	    convertNumToBinaryStr(num, binaryDst, negative, len)
	}
}

void convertNumToBinaryStr(int num ,char* binary, bool negative, int len)
{
	int mask, num;
    mask = 1 << (len-1);
    while (mask) {
        *binaryDst = getBinaryChar(mask, num, negative);
        binaryDst++;
        mask >>= 1;
    }
    *(binaryDst) = '\0';
}

char getBinaryChar(int mask, int value, bool isNegative) {
    if (isNegative) { /* Flip the bit of the results */
        if ((mask & value) == 0) {
            return '1';
        } else {
            return '0';
        }
    } else {
        if ((mask & value) == 0) {
            return '0';
        }
    }
    return '1';
}


int isValidNumber(char * c) 
{
	int i, len;
  	len = strlen(c);
  	len = (len > MAX_LINE_LENGTH ? MAX_LINE_LENGTH : len);
	for (i = 0; i < len; i++) 
        if (isdigit(c[i]) == 0) 
        {
            return 0; 
        }
    return 1; 
}

