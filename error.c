#include "error.h"

void printMisingFileError()	
{
	printf("%s%s\n", ERROR, MISSING_FILE_PARAMETER);
}

void printFetchFileError(char* inFileName)
{
	printf("%s%s%s\n", ERROR, FETCH_FILE, inFileName);
}

void printMemEllocateError()
{
	printf("%s%s\n", ERROR, OUT_OF_MEM);
	exit(1);
}

void printError(int error, char* fileName, int numOfRow)
{
	printf("%s in file:%s, in line:%d. ",ERROR, fileName, numOfRow);
	switch (error)
	{
		case LABEL_EXCEEDED_MAX_LEN_ERROR:
			printf("Label exceeded the limit of length (31)\n"); break;
		case LABEL_START_ERROR:
			printf("Label must start with an alphabet character (A-z).\n");break;
		case ILLIGALE_CHAR_ERROR:
			printf("Label contains illegal characters (can't contain spaces or non-alphanumeric)\n"); break;
		case DUP_LABEL_NAME_ERROR:
			printf("Label is already appear in the symbol table\n"); break;
		case LABEL_EQUAL_TO_SAVED_WORD_ERROR:
			printf("Label is equal to saved word in the language\n"); break;
		case EMPTY_DATA_ERROR:
			printf("Data declaration can't be empty!\n");break;
		case ILL_COMMA_ERROR:
			printf("Detected illegal comma in data declaration - must include numbers separated by a single comma\n");break;
		case ILL_CHAR_ERROR:
			printf("Detected illegal char in data declaration - number can include only digits\n");break;
		case MISS_COMMA_ERROR:
			printf("Detected illegal char in data declaration - between 2 number must be a comma sign\n");break;
		case MULTI_CONSEC_COMMAS_ERROR:
			printf("After a comma we must see a number, and we can't have multiple comma for a number\n");break;		
		case ILL_POSITIVITY_SIGN_ERROR:
			printf("Positivity sign appear in a wrong place\n");break;		
		case EXTRA_TEXT_ERROR:
			printf("Extra text after closing quotes\n");break;
		case ILL_DATA_TYPE_ERROR:
			printf("Data declaration type doesn't exist\n");break;
        case ILLIGALE_LINE_TYPE:
            printf("Detected illegal type of line, no: data or code\n");break;
        case ILL_REG_NAME:
            printf("Detected illegal type of register\n");break;
        case IMM_OPERAND_NOT_A_NUMBER:
            printf("Immediate opperand is not a valid number\n");break;
        case STRING_STARTS_QUOTES_IS_MISS:
            printf("The string of the String type does not start with quotes\n");break;
        case STRING_QUOTES_IS_MISS:
            printf("The String of the String type must be wrapped in quotes\n");break;
        case EMPTY_STRING_ERROR:
            printf("No String in the String line\n");break;
        case LABEL_NOT_IN_SYMB_TABLE:
            printf("The given Label (as operand) is not in the Symbol table\n");break;
        case NO_OPERANDS_ERROR:
            printf("There is no opernads in the line\n");break;
        case MISS_PARAM_ERROR:
            printf("Missing paramters in the code line\n");break;
        case INSTRUCTION_DOES_NOT_EXIST_ERROR:
            printf("The given instruction is illegal\n");break;
        case EXTRANEOS_TEXT_AFTER_OPERANDS:
            printf("Extraneous text after second operand\n");break;
        case ILL_CODE_OPERANDS_ERROR:
            printf("Given operands are not suitable to instruction type\n");break;

            
	}
}
