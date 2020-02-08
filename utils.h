#define FIRST_INDEX 0
#define LABEL_MAX_LEN 31
#define FIRST_ADDRES 100
#define MAX_WORD_LENGTH 15
#define MAX_LINE_LENGTH 80
#define MAX_LENTH_OF_INSTRUCT 4
#define MEM_SIZE 4096
#define OCT_KEYWORD_SIZE 5
#define DES_MEM_ADD_SIZE 4
#define MAX_NUM_WITH_3_DIGIT 999

#define OBJ_EXTENSION ".ob" 
#define EN_EXTENSION ".ent" 
#define EX_EXTENSION ".ext" 
#define ASSEMBLY_EXTENSION ".as" 

#define isNegative(num) (*num == '-' ? 1: 0)
#define isPositive(num) (*num == '+' ? 1: 0)
#define lenOfLine(len) (len = (len > MAX_LINE_LENGTH ? MAX_LINE_LENGTH : len))
/*
	check if the line is a comment line.
	@param char* line - The current read line from the file 
	@return int - A flag representing if the line is a comment line or not. 0 = False, 1 = True
*/
#define isCommentLine(ln) (*ln == ';' ? 1: 0)

/*
	check if the line is empty.
	@param char* line - The current read line from the file 
	@return int - A flag representing if the line is empty or not. 0 = False, 1 = True
*/
#define isEmptyLine(ln) (*ln == '\0' ? 1: 0) 

