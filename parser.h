#define isImmediate(operand) (operand[0] == '#' ? true: false)
#define isIndirectReg(operand) (((operand[0] == '*') && (operand[1] == 'r')) ? true: false) //can add check for valid register
#define isDirectReg(operand) ((operand[0] == 'r') ? true: false)
#define ONE_ADDITIONAL_LINE 1
#define TWO_ADDITIONAL_LINE 2
#define NO_ADDITIONAL_LINE 0
#define OPCODE_LEN_BITS 4
#define REG_METHOD_LEN_BITS 4
#define ARE_LEN_BITS 3
#define IMM_VAL_LEN_BITS 12
#define REG_VAL_BITS 3
#define REG_PRIOR_BITS 6
#define EMPTY_BMC "000000000000000"
#define A "100"
#define R "010"
#define E "001"


#define ENTRY "entry"
#define EXTERN "extern"
#define DATA "data"
#define STRING "string"
#define ENTRY_LEN 5
#define EXTERN_LEN 6
#define STR_LEN 6
#define DATA_LEN 4

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
	Set the symbol value in the line object to be 
	the symbol which defined at the beginning of the line,
	and move the string pointer forward as the length of the symbol value.
	@param ParsedLineNode* line - The object of the current line. 
	@param char* label - The substring of line->ln which start at the end of the label.
*/
void setSymbValue(ParsedLineNode* line, char* label);

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