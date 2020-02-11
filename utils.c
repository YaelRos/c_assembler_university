#include "util.h"
#include "dataStruct.h"

/*
	get the line length and return the smallest value between the line length and the given max length of line.
*/
#define lenOfLine(len) (len = (len > MAX_LINE_LENGTH ? MAX_LINE_LENGTH : len))


void createOutputFiles(InstructImg *instructImg, char *inFileName, int dc)
{
	int i, keywordInOct;
	char *memAdd;
	char outputFileName[MAX_LINE_LENGTH];
	FILE *fp;
	memAdd = (char*)malloc(sizeof(char)) 
	appendExtensionToFilename(outFileName, inFileName, OBJ_EXTENSION);
	fp = fopen(outFileName, "w");
	fprintf(fp, "%d %d\n", instructImg->ic - dc, dc); /* Subtract DC from IC, which was added previously in the 2nd iteration */

	/* Write every row in Octal */
	for (i = 0; i < instructImg->ic; i++) 
	{
		keywordInOct = binaryToOctal(instructImg->instructions[i]);
		numToMemAdd(memAdd, FIRST_ADDRES+i, DEC_MEM_ADD_SIZE+1);
		fpintf(fp, "%d %d\n", memAdd ,keywordInOct);
	}    

	fclose(fp);
	createEntryExternFiles(symbTable, inFileName);
}

void createEntryExternFiles(SymbolTable *symbTable, char *inFileName) 
{
	char enOutFileName[MAX_LINE_LENGTH];
	char exOutFileName[MAX_LINE_LENGTH];
	FILE *fpEx, *fpEn;
	int hasEnData = 0, hasExData = 0, i;
	SymbNode *symbNode = symbTable->head;

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
	for (i = 0; i < symbTable->exTable->counter; i++)
	{
		if (!hasExData) 
		{
			fpEx = fopen(exOutFileName, "w");
			hasExData = 1;
		}
		fprintf(fpEx, "%s %s\n", symbTable->exTable->externalLabel[i].SymbName,
		numToMemAdd(symbTable->exTable->externalLabel[i].memAddr));
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

void freeMem(SymbolTable *symbTable, InstructImg* instructImg, 
	DataImg* dataImg, ParsedFile* pf) 
{
	int i;
	SymbNode *symbNode1, *symbNode2;
	dataNode *dataNode1, *dataNode2;
	parsedLine *pLine1, *pLine2;

	symbNode1 = symbTable->head;
	while (symbNode1 != NULL) 
	{
		symbNode2 = symbNode1;
		symbNode1 = symbNode1->next;
		free(symbNode2);
	}

	dataNode1 = dataTable->head;
	while (dataNode1 != NULL) 
	{
		dataNode2 = dataNode1;
		dataNode1 = dataNode1->next;
		free(dataNode2);
	}

	pLine1 = pf->head;
	while (pf != NULL) 
	{
		pLine2 = pLine1;
		pLine1 = pLine1->next;
		free(pLine2);
	}

	for (i = 0; i < instructImg->ic; i++) 
	{
		free(instructImg->instructions[i]);
	}
}

int binaryToOctal(char* binaryNumStr)
{
    int octalnum = 0, decimalnum = 0, i = 0, num;

	binarynum = atoi(binaryNumStr);

    /* This while loop converts binary number "binarynum" to the
     * decimal number "decimalnum"
     */
    while(binarynum != 0)
    {
        decimalnum = decimalnum + (binarynum%10) * pow(2,i);
        i++;
        binarynum = binarynum / 10;
    }

    //i is re-initialized
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

    //Returning the octal number that we got from binary number
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
	itoa(dstNum, srcNum,len-i-1);
	*(dstNum+len) = '\0';
}




// void parsedLineNodeCpy(ParsedLineNode *dst, ParsedLineNode *src)
// {
// 	strcpy(dst->fileName,src->fileName);
// 	strcpy(dst->line,src->line);
// 	dst->error = src->error;
// 	dst->symbFlag = src->symbFlag;
// 	strcpy(dst->symbValue, src->symbValue);
// 	dst->symbType = src->symbType;
// 	strcpy(dst->instructName,src->instructName);
// 	dst->parseLine *next = NULL;
// }


/*
	Add the file extension to the file name.
	@param char* dst - The file name with the extension name.
	@param char* src - The file name 
	@param char* extension - The extension type in a string format.
*/
void appendExtensionToFilename(char* dst, char* src, char* extension)
{
	strcpy(dst, src);
	strcat(dst, extension);
}

void trimwhitespace(char *ln)
{
	char *end;
	
	while(isspace(*ln)) ln++; // Trim leading space

	if(*ln == 0)
	{
		return;
	}
	// Trim trailing space
	end = ln + strlen(ln) - 1;
	while(end > ln && isspace(*end)) end--;

	end[1] = '\0'; // Write new null terminator character
}

void convertDecStrToBinaryStr(char* binaryDst, char* decSrc, bool negative, int len)
{
	int mask, num;
	num = atoi(decSrc);
	if (isValidNumber(num))
	{
		if (isNegative) /* Handle negative numbers - For two's compliment we need to flip the number and add +1. We'll add before flipping since it makes it easier (And the results are the same!) */
		{ 
			num += 1;
			num *= -1;
		}
		convertNumToBinaryStr(num, binaryDst, negative, len)
	}
}

void convertNumToBinaryStr(char* binary, int num, bool negative, int len)
{
	int mask, num;
	mask = 1 << (len-1);
	while (mask) 
	{
		*binaryDst = getBinaryChar(mask, num, negative);
		binaryDst++;
		mask >>= 1;
	}
	*(binaryDst) = '\0';
}


char getBinaryChar(int mask, int value, bool isNegative) 
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

int isSavedWord(char* sy) 
{
	int i;
	char* savedWord[] = {ENTRY, EXTERN, DATA, STRING, R1, R2, R3, R4, R5, R6, R7, 
		"mov", "cmp", "add", "sub", "lea", "clr", "not", "inc", "dec", "jmp", "bne",
		"red", "prn", "jsr", "rts", "stop"}
	for (int i = 0; i < count; i++)
	{
		if (strcmp(savedWord[i], sy) == 0)
		{
			return 1;
		}
	}
	return 0;
}