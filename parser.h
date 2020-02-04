#define isImmediate(operand) (operand[0] == '#' ? true: false)
#define isIndirectReg(operand) (((operand[0] == '*') && (operand[1] == 'r')) ? true: false) //can add check for valid register
#define isDirectReg(operand) ((operand[0] == 'r') ? true: false)

