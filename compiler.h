#include "dataStruct.h"

#define OPCODE_LEN_BITS 4
#define REG_METHOD_LEN_BITS 4
#define ARE_LEN_BITS 3
#define IMM_VAL_LEN_BITS 12
#define REG_VAL_BITS 3
#define REG_PRIOR_BITS 6
#define NUM_OF_DATA_GUIDANCE 2

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
 SymbTable *symbTable, ParsedFile *parsedFile);

/*
    Run the first iteration algorithem (Lines 3-15 in the mmn page 33)
    @param char* fileName - The original filename we're working with
    @param InstructImg* instructImg - The instruction image
    @param DataImg* dataImg - The data image
    @param SymbTable* symbTable - The symbol table
    @param ParsedFile* pf - Linked list of the rows in the file
*/
void firstLineAlgo(ParsedLineNode* line, InstructImg *instructImg, DataImg *dataImg,
 SymbTable *symbTable);

/*
   	manage the steps that needs to do for hadling extern type line.

	@param ParsedLineNode * line - The object of the current line. 
	@param DataImg* dataImg - The data image
    @param InstructImg* instructImg - The instruction image
    @param SymbNode* symbNode - The symbol node to be added to the symbol table
    @param SymbTable* symbTable - The symbol table
*/
void handleExternCase(ParsedLineNode* line,DataImg* dataImg,InstructImg* instructImg,
 SymbNode* symbNode, SymbTable* symbTable);

/*
   	manage the operands parse that need to do in the CODE type line.

	@param ParsedLineNode * line - The object of the current line. 
    @param SymbTable* symbTable - The symbol table
*/
void getOperandsStruct(ParsedLineNode* line, SymbTable *symbTable);

/*
   	Calculate the number of additional binary rows for one code line. 

	@param ParsedLineNode * line - The object of the current line. 
*/
void calculateL(ParsedLineNode* line);

/*
   	Calculate the binary code of the first row of the instruction. 

	@param ParsedLineNode * line - The object of the current line. 
	@param InstructImg* instructImg - The instruction image
*/
void buildBinaryCodeFirstLn(ParsedLineNode* line, InstructImg* instructImg);

/*
   	Add the relevant BITs of the addressing method of the operand to the first row
   	of the binary code. 

	@param int opMet - The addressing method of the operand. 
	@param struct instructNode *in - The first instruction row of the current line. 
	@param int place - Represent the pleace which need to insert the instrction to. 
	@return int - the next place to insert the additional binaary code. 
*/
int addRegMethodToFirstLn(int opMet, struct instructNode *in, int place);

/*
   	Calculate the binary code of the additional rows of the instruction. 

	@param ParsedLineNode * line - The object of the current line. 
	@param InstructImg* instructImg - The instruction image
*/
void buildBinaryCodeNextLn(ParsedLineNode *line, InstructImg* instructImg);

/*
   	Add the relevant BITs of the addressing method of the operand to the additional row
   	of the binary code. 

	@param int opMet - The addressing method of the operand. 
	@param int place - Represent the pleace which need to insert the instrction to. 
	@param struct instructNode *in - Th instruction row.
	@param ParsedLineNode *line - The object of the current line.
	@param InstructImg* instructImg - The instruction image.
	@return int - the next place to insert the additional binaary code. 
*/
void addRegMethodToLn(int opMethod, int place, struct instructNode *in, 
	ParsedLineNode *line, InstructImg* instructImg);

/*
   	Add BITs of an addressing method of an operand to a binary code.

	@param char* p - The opernd.
	@param int isNegative - A flag representing if the symbol is negative or not. 0 = False, 1 = True
	@param int len - num of BITs
	@param int place - Represent the pleace which need to insert the instrction to. 
	@param struct instructNode *in - The instruction row.
*/
void addRegToMem(char *p, int isNegative, int len, int place, struct instructNode *in);

/*
    Second iteration of the Assembler, parse the rest of the file and complit the convertion 
    of the binary machine code.
    return an indication whether we failed or succeeded to parse the file and convert it
    to a binary machine code.
    @param InstructImg* instructImg - The instruction image
    @param DataImg* dataImg - The data image
    @param SymbTable* symbTable - The symbol table
    @param char* fileName - The original filename we're working with
    @param ParsedFile* pf - Linked list of the rows in the file
    @return int - The Error Num we found, if there is no error so it will be 0
*/
int secIteration(InstructImg *instructImg, DataImg *dataImg, SymbTable *symbTable,
 char *fileName, ParsedFile *parsedFile);

/*
    Run the second iteration algorithem (Lines 4-8 in the mmn page 33)

    @param ParsedLineNode *line - The object of the current line.
    @param InstructImg* instructImg - The instruction image
    @param DataImg* dataImg - The data image
    @param SymbTable* symbTable - The symbol table
*/
void secLineAlgo(ParsedLineNode* line, InstructImg *instructImg, 
	DataImg *dataImg, SymbTable *symbTable);

/*
    Calculate the binary code of the additional rows of the instructions that still was not calculated. 

    @param ParsedLineNode *line - The object of the current line.
    @param InstructImg* instructImg - The instruction image
    @param SymbTable* symbTable - The symbol table
*/
void updateBinaryMachineCode(ParsedLineNode* line, InstructImg *instructImg, 
	SymbTable *symbTable);

/*
    Calculate the binary code of the label address and figure what the relevant ARE code is
    and add it to the instruction row.

    @param ParsedLineNode *line - The object of the current line.
    @param InstructImg* instructImg - The instruction image
    @param SymbTable* symbTable - The symbol table
*/
void addDirectLabelToMem(ParsedLineNode* line, InstructImg *instructImg, 
	SymbTable *symbTable);
