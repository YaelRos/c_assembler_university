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
#define ENTRY_LEN 5
#define EXTERN_LEN 6
#define ENTRY "entry"
#define EXTERN "extern"
#define DATA "data"
#define STRING "string"
#define EMPTY_BMC "000000000000000"
#define A "100"
#define R "010"
#define E "001"