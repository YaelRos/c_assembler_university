#ifndef PARSER_H
#define PARSER_H

#include "error.h"
#include "utils.h" 

#define ONE_ADDITIONAL_LINE 1
#define TWO_ADDITIONAL_LINE 2
#define NO_ADDITIONAL_LINE 0
#define EMPTY_BMC "000000000000000"


extern OpCode opCodesMatch[NUM_OF_INSTRUCTION];

void setStorageEntryOrExtern(ParsedLineNode * line);
void updateExTable(int memadd, char* operand, SymbTable *symbTable);
int getAddressindMethod(ParsedLineNode* line, char* operand, SymbTable *symbTable);
void setSymbFromExternEntry(ParsedLineNode* line, int etLen);

/*
	Check if there is a symbol definition at the beginning of the line.
	if there is ':' in the line, it indicates that it's a definition.
	@param char* ln - The string of the current line. 
	@return int - A flag representing if we found a symbol or not. 0 = False, 1 = True
*/
int isFirstFieldSymb(char* ln);

/*
	Check if the symbol which defined at the beginning of the line is valid,
	if is not valid - will update it as an error. 
	if there is ':' in the line, it indicates that it's a definition.
	@param ParsedLineNode* line - The object of the current line. 
	@param SymbTable* symbTable - The symbol table
*/
void validateSymb(ParsedLineNode* line, SymbTable * symbTable);

/*
	Check if the line is guidance line.
	if there is '.' in the beginning of the line, it indicates that it's a guidance line.
	@param ParsedLineNode * line - The object of the current line. 
	@return int - A flag representing if the line is guidance line or not. 0 = False, 1 = True
*/
int isGuidanceType(ParsedLineNode* line);

/*
	Check if the line is Data line.
	@param ParsedLineNode * line - The object of the current line. 
	@return int - A flag representing if the line is Data line or not. 0 = False, 1 = True
*/
int isDataStorage(ParsedLineNode * line);

/*
	Encodes each char (which represented by is ASCII binary value) into the data image.

	@param ParsedLineNode * line - The object of the current line.
	@param DataImg* dataImg - The data image.
*/
void parseStringType(ParsedLineNode* line, DataImg *dataImg);

/*
	Encodes the data in the data image

	@param ParsedLineNode * line - The object of the current line.
	@param DataImg* dataImg - The data image.
*/
void parseDataType(ParsedLineNode* line, DataImg *dataImg);

/*
	gat the operand name from the line string and convert it to the suitable opcode,
	save it in the object of the line.

	@param ParsedLineNode * line - The object of the current line.
*/
void getInstrucName(ParsedLineNode* line);

/*
	Get the source operand first and than the destination operand and save them 
	in the InstructionStruct object

	@param ParsedLineNode * line - The object of the current line.
	@param SymbTable* symbTable - The symbol table
*/
void getFirstOperand(ParsedLineNode* line, SymbTable *symbTable);

/*
	Get the next operand from the given line

	@param ParsedLineNode * line - The object of the current line.
	@param char* operand - The string which represent the operand. 
	@param SymbTable* symbTable - The symbol table
	@return AddressingMethod - Integer which represent the addressing method code
*/
int getOperand(ParsedLineNode* line, char* operand, SymbTable *symbTable);

/*
	Get the destination operand and save it in the InstructionStruct object.

	@param ParsedLineNode * line - The object of the current line.
	@param SymbTable* symbTable - The symbol table
*/
void getSecOperand(ParsedLineNode *line, SymbTable *symbTable);

#endif
