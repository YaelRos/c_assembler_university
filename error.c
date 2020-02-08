#include "errors.h"

void printMisinFileError()
{
	printf("%s%s", ERROR, MISSING_FILE_PARAMETER);
}

void printFetchFileError(char* inFileName);
{
	printf("%s%s%s", ERROR, FETCH_FILE, inFileName);
}

void printMemEllocateError()
{
	printf("%s%s", ERROR, OUT_OF_MEM);
}