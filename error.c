#include "error.h"

void printMisinFileError()	
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
}

void printError(int error, char* fileName, int numOfRow)
{
	printf("%s %s: line-%d\n",ERROR, fileName, numOfRow);
	switch (error)
	{
		case LABEL_EXCEEDED_MAX_LEN_ERROR:
			printf("Label exceeded the limit of length (31)"); break;
		case LABEL_START_ERROR:
			printf("Label must start with an alphabet character (A-z).\n");break;
		case ILLIGALE_CHAR_ERROR:
			printf("Label contains illegal characters (can't contain spaces or non-alphanumeric)"); break;
		case DUP_LABEL_NAME_ERROR:
			printf("Label is already appear in the symbol table"); break;
		case LABEL_EQUAL_TO_SAVED_WORD_ERROR:
			printf("Label is equal to saved word in c language"); break;
		case EMPTY_DATA_ERROR:
			printf("Data declaration can't be empty!\n");break;
		case ILL_COMMA_ERROR:
			printf("Detected illegal comma in data declaration - must include numbers separated by a single comma\n");break;
		case ILL_DATA_TYPE_ERROR:
			printf("Data declaration type doesn't exist\n");break;
	}
	
	/* MISS_PARAM_ERROR,
		INSTRUCTION_DOES_NOT_EXIST_ERROR,
		UNEXPECTED_RESULT,
		NO_OPERANDS_ERROR,
		MISS_COMMA_ERROR,
		EXTRA_TEXT_ERROR,
		LABEL_NOT_IN_SYMB_TABLE

        case INVALID_OPCODE_ERROR:
            printf(BOLD_WHITE_PRINT "Op code '%s' doesn't exist!\n", 
                    pr->rowMetadata.codeRowMetadata.oc.opCodeName);
            break;
        case DATA_STRING_DECLARATION_MISSING_QUOTES:
            printf(BOLD_WHITE_PRINT "String data declarations must be wrapped in quotes!\n");
            break;
        case EXTRANEOUS_TEXT_AFTER_STRING_QUOTES:
            printf(BOLD_WHITE_PRINT "Extraneous text after quotes\n");
            break;
        case ILLEGAL_POSITIVITY_SIGN_IN_DATA_DECLARATION:
            printf(BOLD_WHITE_PRINT "Illegal sign for number - each number can only have one '+' or '-' sign, followed by a number!\n");
            break;
        case ILLEGAL_DATA_DECLARATION_CHARACTER:
            printf(BOLD_WHITE_PRINT "Illegal character for number - can only contain numeric characters, separated by a comma!\n");
            break;
        case ILLEGAL_DATA_DECLARATION_EXTRANEOUS_COMMA:
            printf(BOLD_WHITE_PRINT "Data declaration can't start or end with a comma! (a comma always has to between numbers)\n");
            break;
        case ILLEGAL_DATA_DECLARATION_EXTRANEOUS_SPACE:
            printf(BOLD_WHITE_PRINT "Extraneous whitespace between numbers!\n");
            break;    
        case ILLEGAL_CODE_OPERANDS:
            printf(BOLD_WHITE_PRINT "Operation code '%s' received illegal arguments!\n", pr->rowMetadata.codeRowMetadata.oc.opCodeName);
            break;
        case MISSING_COMMA_BETWEEN_OPERANDS:
            printf(BOLD_WHITE_PRINT "Operands must be separated between a comma!\n");
            break;
        case MULTIPLE_COMMAS_BETWEEN_OPERANDS:
            printf(BOLD_WHITE_PRINT "Detected more than one comma. Operations can have up to 2 arguments, separated by at most 1 comma\n");
            break;
        case NO_TEXT_AFTER_COMMA:
            printf(BOLD_WHITE_PRINT "Missing text after comma - expecting a second argument\n");
            break;
        case EXTRANEOS_TEXT_AFTER_OPERANDS:
            printf(BOLD_WHITE_PRINT "Extraneous text after second operand\n");
            break;
        case EXTRANEOUS_TEXT_AFTER_EXTERN_ENTRY:
            printf(BOLD_WHITE_PRINT ".entry / .extern declarations can only have a single symbol name\n");
            break;
        case DUPLICATE_SYMBOL_DECLARATION:
            printf(BOLD_WHITE_PRINT "Symbol '%s' already declared - a symbol can only be declared once!\n", pr->symbolName);
            break;
        case INVALID_REGISTER_NAME:
            printf(BOLD_WHITE_PRINT "Invalid register '%s'. Available registers are between @r0 and @r7 \n", pr->rowMetadata.codeRowMetadata.srcOperand);
            break;
        case UNKNOWN_ERROR:
            printf(BOLD_WHITE_PRINT "Encountered an unknown error...\n");
            break;
        default:
            printf(BOLD_WHITE_PRINT "Encountered an unknown error...\n");
            break;
    } 
    */
}
