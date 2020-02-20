#include "utils.h"

void createOutputFiles(InstructImg *instructImg, char *inFileName, int dc)
{
	int i, keywordInOct;
	char *memAdd;
	char outFileName[MAX_LINE_LENGTH];
	FILE *fp;
	memAdd = (char*)malloc(sizeof(char)); 
	appendExtensionToFilename(outFileName, inFileName, OBJ_EXTENSION);
	fp = fopen(outFileName, "w");
	fprintf(fp, "%d %d\n", instructImg->ic - dc, dc); /* Subtract DC from IC, which was added previously in the 2nd iteration */

	/* Write every row in Octal */
	for (i = 0; i < instructImg->ic; i++) 
	{
		keywordInOct = binaryToOctal(instructImg->instructions[i].instruction);
		numToMemAdd(memAdd, FIRST_ADDRES+i, DEC_MEM_ADD_SIZE+1);
		fprintf(fp, "%s %d\n", memAdd ,keywordInOct);
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
	for (i = 0; i < symbTable->exTable.counter; i++)
	{
		if (!hasExData) 
		{
			fpEx = fopen(exOutFileName, "w");
			hasExData = 1;
		}
		numToMemAdd(memAdd, symbTable->exTable.externalLabel[i].memAddr, DEC_MEM_ADD_SIZE+1);
		fprintf(fpEx, "%s %s\n", symbTable->exTable.externalLabel[i].SymbName,memAdd);
	}
	/* Close files */
	if (hasEnData) 
	{
		fclose(fpEn);
	}
	if (hasExData) 
	{
		fclose(fpEx);
	}
}

void freeMem(SymbTable *symbTable, InstructImg* instructImg, 
	DataImg* dataImg, ParsedFile* pf) 
{
	int i;
	SymbNode *symbNode;
	DataImgNode *dataNode;
	ParsedLineNode *pLine;

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

	for (i = 0; i < instructImg->ic; i++) 
	{
		free(instructImg->instructions[i].instruction);
	}
}

int binaryToOctal(char* binaryNumStr)
{
    int octalnum = 0, decimalnum = 0, i = 0, num;

	num = atoi(binaryNumStr);

    /* This while loop converts binary number "num" to the
     * decimal number "decimalnum"
     */
    while(num != 0)
    {
        decimalnum = decimalnum + (num%10) * pow(2,i);
        i++;
        num = num / 10;
    }

    i = 1;

    /* This loop converts the decimal number "decimalnum" to the octal
     * number "octalnum"
     */
    while (decimalnum != 0)
    {
        octalnum = octalnum + (decimalnum % 8) * i;
        decimalnum = decimalnum / 8;
        i = i * 10;
    }

    /* Returning the octal number that we got from binary number */
    return octalnum;
}

void numToMemAdd(char* dstNum, int srcNum, int len)
{
	int i = 0;
	if (srcNum<=MAX_NUM_WITH_3_DIGIT)
	{
		*dstNum = '0';
		i++;
		dstNum++;
	}
	itoa(srcNum, dstNum);
	*(dstNum+len) = '\0';
}

/*
 void parsedLineNodeCpy(ParsedLineNode *dst, ParsedLineNode *src)
{
	strcpy(dst->fileName,src->fileName);
	strcpy(dst->line,src->line);
	dst->error = src->error;
	dst->symbFlag = src->symbFlag;
	strcpy(dst->symbValue, src->symbValue);
	dst->symbType = src->symbType;
	strcpy(dst->instructName,src->instructName);
	dst->parseLine *next = NULL;
}
*/


void appendExtensionToFilename(char* dst, char* src, char* extension)
{
	strcpy(dst, src);
	strcat(dst, extension);
}

char* trimwhitespace(char *ln)
{
	char *end;
	printf("trimwhitespace before:%s\n", ln);
	while(isspace(*ln)) ln++; /* Trim leading space */
	if(*ln == '\0')
	{
		return ln;
	}
	/* Trim trailing space */
	end = ln + strlen(ln) - 1;
	while(end > ln && isspace(*end)) end--;

	end[1] = '\0'; /* Write new null terminator character */
	printf("trimwhitespace after:%s\n", ln);
	return ln;
}

void convertDecStrToBinaryStr(char* binaryDst, char* decSrc, int negative, int len)
{
	int num;
	if (isValidNumber(decSrc))
	{
		num = atoi(decSrc);
		if (negative)  //Handle negative numbers - For two's compliment we need to flip the number and add +1. We'll add before flipping since it makes it easier (And the results are the same!) 
			num -= 1;
		convertNumToBinaryStr(binaryDst, num, negative, len);
		printf("convertDecStrToBinaryStr - %d\n", num);
	}
}

void convertNumToBinaryStr(char* binaryDst, int num, int negative, int len)
{
	int mask;
	mask = 1 << (len-1);
	printf("convertNumToBinaryStr: num:%d\n", num);
	while (mask) 
	{
		*binaryDst = getBinaryChar(mask, num, negative);
		binaryDst++;
		mask >>= 1;
	}
	*(binaryDst) = '\0';
}


char getBinaryChar(int mask, int value, int isNegative) 
{
	if (isNegative)
	{ /* Flip the bit of the results */
		if ((mask & value) == 0)
		{
			return '1';
		}
		else
		{
			return '0';
		}
	}
	else
	{
		if ((mask & value) == 0) 
		{
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

void itoa(int n, char s[])
{
	int i, sign;

	if((sign =n) < 0) /* record sign */
		n = -n;
	i=0;
	do {	/* generate digits in reverse order */
		s[i++] = n % 10 + '0'; /* get next digit */
	} while ((n /= 10) > 0);	/* delete it */
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
	char* savedWord[NUM_OF_SAVED_WORD] = {ENTRY, EXTERN, DATA, STRING, R1, R2, R3, R4, R5, R6, R7, 
		"mov", "cmp", "add", "sub", "lea", "clr", "not", "inc", "dec", "jmp", "bne",
		"red", "prn", "jsr", "rts", "stop"};
	printf("isSavedWord - %s\n", "here");
	for (i = 0; i < NUM_OF_SAVED_WORD; i++)
	{
		if (strcmp(savedWord[i], sy) == 0)
		{
			return 1;
		}
	}
	return 0;
}
