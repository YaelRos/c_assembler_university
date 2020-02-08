#include <stdio.h>
#include "utils.h"
#include "error.h"

/*
    Run the first and second iteration, and return an indication whether we failed or succeeded
    @param char* fileName - The original filename we're working with
    @param InstructImg* instructImg - The instruction image
    @param DataImg* dataImg - The data image
    @param SymbTable* symbTable - The symbol table
    @param ParsedFile* pf - Linked list of the rows in the file
    @return int - The Error Num we found, if there is no error so it will be 0
*/
int ecxcute(char* fileName, InstructImg* instructImg,
	DataImg* dataImg,SymbTable* symbTable,ParsedFile* pf);

/*
    First iteration of the Assembler, parse the as file and convert it (as much it can) 
    to binary machine code.
    return an indication whether we failed or succeeded to parse the file and convert it
    to binary machine code.
    @param char* fileName - The original filename we're working with
    @param InstructImg* instructImg - The instruction image
    @param DataImg* dataImg - The data image
    @param SymbTable* symbTable - The symbol table
    @param ParsedFile* pf - Linked list of the rows in the file
    @return int - The Error Num we found, if there is no error so it will be 0
*/
int firstIteration(char *fileName, InstructImg *instructImg, DataImg *dataImg,
 SymbTable *symbTable, ParsedFile *parsedFile)