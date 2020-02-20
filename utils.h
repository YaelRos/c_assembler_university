#ifndef UTILS_H
#define UTILS_H

#include <ctype.h>
#include <math.h>
#include "dataStruct.h"

#define OCT_KEYWORD_SIZE 5
#define DEC_MEM_ADD_SIZE 4
#define MAX_NUM_WITH_3_DIGIT 999

#define OBJ_EXTENSION ".ob" 
#define EN_EXTENSION ".ent" 
#define EX_EXTENSION ".ext" 
#define ASSEMBLY_EXTENSION ".as" 

#define NUM_OF_SAVED_WORD 27

#define isNegative(num) (*num == '-' ? 1: 0)
#define isPositive(num) (*num == '+' ? 1: 0)
/*
	Check if the line is a comment line.
	@param char* line - The current read line from the file 
	@return int - A flag representing if the line is a comment line or not. 0 = False, 1 = True
*/
#define isCommentLine(ln) (*ln == ';' ? 1: 0)

/*
	Check if the line is empty.
	@param char* line - The current read line from the file 
	@return int - A flag representing if the line is empty or not. 0 = False, 1 = True
*/
#define isEmptyLine(ln) (*ln == '\0' ? 1: 0) 

void numToMemAdd(char* dstNum, int srcNum, int len);
int binaryToOctal(char* binaryNumStr);
void createEntryExternFiles(SymbTable *symbTable, char *inFileName);
void createOutputFiles(InstructImg *instructImg, char *inFileName, int dc);
void freeMem(SymbTable *symbTable, InstructImg* instructImg, DataImg* dataImg, ParsedFile* pf);
/*
	Add the file extension to the file name.
	@param char* dst - The file name with the extension name.
	@param char* src - The file name 
	@param char* extension - The extension type in a string format.
*/
void appendExtensionToFilename(char* dst, char* src, char* extension);

/* 
	Remove white space from the beginning and the end of the line.
	@param char* line - The current read line from the file 
	@return char* - the line after removing scpaces.
*/
char* trimwhitespace(char *ln);

/*
	convert a decimal number which is represented by a string to binary number which 
	is represented by a string.

	@param char* binaryDst - The converted string in binary (an empty value)
	@param int decSrc - The string to convert
	@param int negative - A flag representing if the number id negative or not. 0 = False, 1 = True. 
	@param int len - number of BITs in the binary number, each BIT will represented by one char
*/
void convertDecStrToBinaryStr(char* binaryDst, char* decSrc, int negative, int len);

/*
	convert a decimal number which is represented by a string to binary number which 
	is represented by a string.

	@param char* binary - The converted string in binary
	@param int num - The string to convert
	@param int negative - A flag representing if the number id negative or not. 0 = False, 1 = True. 
	@param int len - number of BITs in the binary number, each BIT will represented by one char
*/
void convertNumToBinaryStr(char* binary, int num, int negative, int len);

/* 
    Get the binary character for a number with a given mask.
    If the negativity number is on, we flip the results
    @param int mask - The mask we're comparing the number with
    @param int value - The original value, in it's absolute value, and +1 if previously negative
    @param int isNegative - Negativity flag
*/
char getBinaryChar(int mask, int value, int isNegative);

/*
	Check if the given string is a valid number.  
*/
int isValidNumber(char * c);

/*
	Check if the string sy is equal to one of the saved wotd in c language. 
	@param char* sy - The string value
	@return int - A flag representing if the string is equal to saved word. 0 = False, 1 = True
*/
int isSavedWord(char* sy);

/* reverse characters in s */
void reverse(char s[]);

/* convert n to characters in s */
void itoa(int n, char s[]);


#endif
