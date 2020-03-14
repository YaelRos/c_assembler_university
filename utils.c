#include "utils.h"


static const ConvertBinToOct binaryToOctalMap[BIN_TO_OCT_THREE] = 
{ 
    {"000", '0'},
    {"001", '1'},
    {"010", '2'},
    {"011", '3'},
    {"100", '4'},
    {"101", '5'},
    {"110", '6'},
    {"111", '7'}    
};

static const char* savedWord[NUM_OF_SAVED_WORD] = 
{ 
	ENTRY, EXTERN, DATA, STRING, "r1", "r2", "r3", "r4", "r5", "r6", "r7",
	 "mov", "cmp", "add", "sub", "lea", "clr", "not", "inc",
	  "dec", "jmp", "bne", "red", "prn", "jsr", "rts", "stop"
};

void createOutputFiles(InstructImg *instructImg, char *inFileName, DataImg* dataImg)
{
	int i;
	char *memAdd = NULL, *keywordInOct = NULL;
	char outFileName[MAX_LINE_LENGTH];
	FILE *fp;
	DataImgNode *tmp;

	appendExtensionToFilename(outFileName, inFileName, OBJ_EXTENSION);
	fp = fopen(outFileName, "w");
	fprintf(fp, "%d %d\n", instructImg->ic-FIRST_ADDRES, dataImg->dc); /* Subtract DC from IC, which was added previously in the 2nd iteration */

	/* Write every row in Octal */
	for (i = 0; i < instructImg->ic -FIRST_ADDRES; i++) 
	{
		keywordInOct = binaryToOctal(keywordInOct, instructImg->instructions[i].instruction);
		memAdd = numToMemAdd(memAdd, FIRST_ADDRES+i, DEC_MEM_ADD_SIZE+1);
		fprintf(fp, "%s %s\n", memAdd ,keywordInOct);
	} 

	tmp=dataImg->head;
	while (tmp) 
	{
		keywordInOct = binaryToOctal(keywordInOct, tmp->binaryData);
		memAdd = numToMemAdd(memAdd, FIRST_ADDRES+i, DEC_MEM_ADD_SIZE+1);
		fprintf(fp, "%s %s\n", memAdd ,keywordInOct);
		tmp= tmp->next;
		i++;	
	}    
  

	fclose(fp);
}

void createEntryExternFiles(SymbTable *symbTable, char *inFileName) 
{
	char enOutFileName[MAX_LINE_LENGTH];
	char exOutFileName[MAX_LINE_LENGTH];
	FILE *fpEx, *fpEn;
	int hasEnData = 0, hasExData = 0, i;
	SymbNode *symbNode = symbTable->head;
	char *memAdd = NULL;
	ExternNode *tmpNode;

	appendExtensionToFilename(enOutFileName, inFileName, EN_EXTENSION);
	appendExtensionToFilename(exOutFileName, inFileName, EX_EXTENSION);

	for (i = 0; i < symbTable->counter; symbNode = symbNode->next, i++)  
	{   
		if (symbNode->symbType == ENTRY_TYPE) 
		{
			if (!hasEnData) 
			{
				fpEn = fopen(enOutFileName, "w");
				hasEnData = 1;
			}
			fprintf(fpEn, "%s %d\n", symbNode->symbName, symbNode->symbAddr);
		}
	}

	tmpNode = symbTable->exTable->head;
	while (tmpNode)
	{
		if (!hasExData) 
		{
			fpEx = fopen(exOutFileName, "w");
			hasExData = 1;
		}
		memAdd = numToMemAdd(memAdd, tmpNode->memAddr, DEC_MEM_ADD_SIZE+1);
		fprintf(fpEx, "%s %s\n", tmpNode->SymbName,memAdd);
		tmpNode= tmpNode->next;
	}
	/* Close files */
	if (hasEnData) 
		fclose(fpEn);
	if (hasExData) 
		fclose(fpEx);
}

void freeMem(SymbTable* symbTable, InstructImg* instructImg, 
	DataImg* dataImg, ParsedFile* pf) 
{
	SymbNode* symbNode;
	DataImgNode* dataNode;
	lineDFS* pLine;
	ExternNode* exNode;

	while (symbTable->exTable->head != NULL) 
	{
		exNode = symbTable->exTable->head;
		symbTable->exTable->head = symbTable->exTable->head->next;
		free(exNode);
	}

	while (symbTable->head != NULL) 
	{
		symbNode = symbTable->head;
		symbTable->head = symbTable->head->next;
		free(symbNode);
	}
	while (dataImg->head != NULL) 
	{
		dataNode = dataImg->head;
		dataImg->head = dataImg->head->next;
		free(dataNode);
	}
	while (pf->head != NULL) 
	{
		pLine = pf->head;
		pf->head = pf->head->next;
		free(pLine);
	}
}

char* binaryToOctal(char* octal, char* binaryNumStr)
{
	int i = 0,j = 0;
    octal = (char*)malloc(sizeof(char));      

    while (j < OCT_KEYWORD_SIZE) 
    { 
        /* one by one extract from left, substring of size 3 and add its octal code */
        while (strncmp(binaryNumStr, binaryToOctalMap[i].bin, BIN_TO_OCT_BITS))
        {
        	i++;
        }
        octal[j] = binaryToOctalMap[i].oct;
        j++;
        binaryNumStr += 3; 
        i = 0;       
    } 
    octal[j] = '\0';
    return octal;     
}

char* numToMemAdd(char* dstNum, int srcNum, int len)
{
	dstNum= (char*)malloc(sizeof(char));
	if (srcNum<=MAX_NUM_WITH_3_DIGIT)
	{
		*dstNum = '0';
		dstNum++;
	}
	itoa(srcNum, dstNum);
	dstNum--;
	*(dstNum+len) = '\0';
	return dstNum;
}


void appendExtensionToFilename(char* dst, char* src, char* extension)
{
	strcpy(dst, src);
	strcat(dst, extension);
}

char* trimwhitespace(char *ln)
{
	char *end;
	while(isspace(*ln)) ln++; /* Trim leading space */
	if(*ln == '\0')
	{
		return ln;
	}
	/* Trim trailing space */
	end = ln + strlen(ln) - 1;
	while(end > ln && isspace(*end)) end--;

	end[1] = '\0'; /* Write new null terminator character */
	return ln;
}

char* convertDecStrToBinaryStr(char* binaryDst, char* decSrc, int negative, int len)
{
	int num;

	if (isValidNumber(decSrc))
	{
		num = atoi(decSrc);
		if (negative)  /*Handle negative numbers - For two's compliment we need to flip the number and add +1. We'll add before flipping since it makes it easier */
			num -= 1;
		binaryDst = convertNumToBinaryStr(binaryDst, num, negative, len);
		return binaryDst;
	}
	return NULL;
}

char* convertNumToBinaryStr(char* binaryDst, int num, int negative, int len)
{
	int mask, i =0;
	binaryDst = (char *) malloc(sizeof(char));
	mask = 1 << (len-1);
	while (mask) 
	{
		*binaryDst = getBinaryChar(mask, num, negative);
		binaryDst++;
		mask >>= 1;
		i++;
	}
	*(binaryDst) = '\0';
	binaryDst-=i;
	return binaryDst;
}


char getBinaryChar(int mask, int value, int isNegative) 
{
	if (isNegative)
	{ /* Flip the bit of the results */
		if ((mask & value) == 0)
			return '1';
		else
			return '0';
	}
	else
		if ((mask & value) == 0) 
			return '0';
	return '1';
}

int isValidNumber(char * c) 
{
	int i, len;
	len = strlen(c);
	len = (len > MAX_LINE_LENGTH ? MAX_LINE_LENGTH : len);
	for (i = 0; i < len; i++) 
		if (isdigit(c[i]) == 0) 
			return 0; 
	return 1; 
}

void itoa(int n, char s[])
{
	int i, sign;

	if((sign =n) < 0) /* record sign */
		n = -n;
	i=0;
	do {	/* generate digits in reverse order */
		s[i++] = n % DEC + '0'; /* get next digit */
	} while ((n /= DEC) > 0);	/* delete it */
	if (sign < 0)
		s[i++] = '-';
	s[i] = '\0';
	reverse(s);
}

void reverse(char s[])
{
	int c, i, j;
	for (i = 0, j = strlen(s)-1; i<j; i++, j--)
	{
		c = s[i];
		s[i] = s[j];
		s[j] = c;
	}
}

int isSavedWord(char* sy) 
{
	int i;
	for (i = 0; i < NUM_OF_SAVED_WORD; i++)
	{
		if (strcmp(savedWord[i], sy) == 0)
		{
			return 1;
		}
	}
	return 0;
}
