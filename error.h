
enum ERRORS 
{
	NO_ERROR,
	LABEL_EXCEEDED_MAX_LEN_ERROR,
	LABEL_START_ERROR,
	ILLIGALE_CHAR_ERROR,
	DUP_LABEL_NAME_ERROR,
	LABEL_EQUAL_TO_SAVED_WORD_ERROR,
	EMPTY_DATA_ERROR,
	ILL_COMMA_ERROR,
	ILL_DATA_TYPE_ERROR,
	MISS_PARAM_ERROR,
	INSTRUCTION_DOES_NOT_EXIST_ERROR,
	UNEXPECTED_RESULT,
	NO_OPERANDS_ERROR,
	MISS_COMMA_ERROR,
	EXTRA_TEXT_ERROR,
	LABEL_NOT_IN_SYMB_TABLE
};

#define ERROR "ERROR: "
#define LABEL_BEFORE_EXTERN "Warning: There's a label at the beginning of the extern line"

#define MISSING_FILE_PARAMETER "missing file parameter"
#define FETCH_FILE "can't fetch file - "
#define OUT_OF_MEM "out of memory, can't allocate memory"


void printMisinFileError();
void printFetchFileError(char* inFileName);
void printMemEllocateError();
void printError(int error, char* fileName, int numOfRow);
