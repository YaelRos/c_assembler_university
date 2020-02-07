#define FIRST_INDEX 0
#define LABEL_MAX_LEN 31
#define FIRST_ADDRES 100
#define MAX_WORD_LENGTH 15
#define MAX_LINE_LENGTH 80
#define MAX_LENTH_OF_INSTRUCT 4
#define MEM_SIZE 4096

#define isNegative(num) (num[0] == '-' ? 1: 0)
#define isPositive(num) (num[0] == '+' ? 1: 0)
#define lenOfLine(len) (len = (len > MAX_LINE_LENGTH ? MAX_LINE_LENGTH : len))
