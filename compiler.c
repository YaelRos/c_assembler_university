#include "compiler.h"
#include "error.h"
#include "parser.h"

extern OpCode opCodesMatch[];

void addDirectLabelToMem(ParsedLineNode* line, InstructImg *instructImg, 
	SymbTable *symbTable)
{
	SymbNode *symbNode = NULL;
	struct instructNode *in = NULL;
	char *dst = NULL; 

	if(getSymbFeature(symbTable, line->symbValue, symbNode) == NULL)
	{
		line->error = LABEL_NOT_IN_SYMB_TABLE;
	}
	else
	{
		/* add the converted (to binary) label address to the instruction row */
		convertNumToBinaryStr(dst, symbNode->symbAddr, 0, IMM_VAL_LEN_BITS);
		memcpy(in->instruction, dst, IMM_VAL_LEN_BITS);
		instructImg->instructions[instructImg->ic++] = *(in); 
	}
	if (symbNode->symbType == EXTERNAL_TYPE)
	{
		memcpy(in->instruction+IMM_VAL_LEN_BITS, E, ARE_LEN_BITS);
	}
	else
	{
		memcpy(in->instruction+IMM_VAL_LEN_BITS, R, ARE_LEN_BITS);
	}
}

void updateBinaryMachineCode(ParsedLineNode* line, InstructImg *instructImg, 
	SymbTable *symbTable)
{
	instructImg->ic++; /* no need to change first line of instruction */

	if (line->typeHandle.instruct.opSrcMethod != DIRECT &
		line->typeHandle.instruct.opDstMethod != DIRECT ) /* not a direct addressing method though no need to change instructions */
	{
		instructImg->ic = (instructImg->ic) + line->typeHandle.instruct.addLine;
	}
	else if (line->typeHandle.instruct.opSrcMethod != DIRECT)
	{
		addDirectLabelToMem(line, instructImg, symbTable);
	}
	else 
	{
		addDirectLabelToMem(line, instructImg, symbTable);
		addDirectLabelToMem(line, instructImg, symbTable);
	}
}

void addRegToMem(char *p, int isNegative, int len, int place, struct instructNode *in)
{
	char *dst = NULL; 
	convertDecStrToBinaryStr(dst, p, isNegative, len);
	memcpy(in->instruction+place, dst, len);
	memcpy(in->instruction+place+len, A, ARE_LEN_BITS);
}

void addRegMethodToLn(int opMethod, int place, struct instructNode *in, 
	ParsedLineNode *line, InstructImg* instructImg)
{
	char *p = line->typeHandle.instruct.opDst + 1; /* less '#' or 'r' or '*' */
	if (opMethod == INDIRECT_REG)
	{
		p = p + 1; /* less 'r' */
	}
	switch (opMethod)
	{
		case IMMEDIATE: addRegToMem(p, isNegative(p), IMM_VAL_LEN_BITS, 0, in);break;
		case DIRECT_REG: case INDIRECT_REG:
		 addRegToMem(p, 0, REG_VAL_BITS, place, in); break;
		default: break;
	}
	instructImg->instructions[instructImg->ic++]=*in; /* addBinaryCodeToInstructImg */
	instructImg->instructions[instructImg->ic].instruction[MAX_WORD_LENGTH] = '\0';
}

void buildBinaryCodeNextLn(ParsedLineNode *line, InstructImg* instructImg)
{
	int place = 0;
	struct instructNode *in1 = NULL;
	struct instructNode *in2 = NULL;
	memcpy(in1->instruction, EMPTY_BMC, MAX_WORD_LENGTH); /* create an empty word in binary format and save it as an instruction. */

	switch(line->typeHandle.instruct.addLine)
	{
		case ONE_ADDITIONAL_LINE: 
			addRegMethodToLn(line->typeHandle.instruct.opDstMethod, REG_PRIOR_BITS+REG_VAL_BITS, in1, line, instructImg);
			if (line->typeHandle.instruct.opSrcMethod != NO_ADD_METHOD)
			{
				instructImg->ic--;
				addRegMethodToLn(line->typeHandle.instruct.opSrcMethod, place, in1, line, instructImg);
			}
			break;
 		case TWO_ADDITIONAL_LINE:
 			memcpy(in2->instruction, EMPTY_BMC, MAX_WORD_LENGTH); /* create an empty word in binary format and save it as an instruction. */
 			addRegMethodToLn(line->typeHandle.instruct.opSrcMethod, place, in1, line, instructImg);
 			addRegMethodToLn(line->typeHandle.instruct.opSrcMethod, REG_PRIOR_BITS+REG_VAL_BITS, in2, line, instructImg);
			break;
		case NO_ADDITIONAL_LINE: break;
	}
}

int addRegMethodToFirstLn(int opMet, struct instructNode *in, int place)
{
	switch (opMet)
	{
		case IMMEDIATE: memcpy(in->instruction+place,"0001", REG_METHOD_LEN_BITS); break;
		case DIRECT:	memcpy(in->instruction+place, "0010", REG_METHOD_LEN_BITS); break;
		case INDIRECT_REG:	memcpy(in->instruction+place, "0100", REG_METHOD_LEN_BITS); break;
		case DIRECT_REG:	memcpy(in->instruction+place, "1000", REG_METHOD_LEN_BITS); break;
		default:	memcpy(in->instruction+place, "0000", REG_METHOD_LEN_BITS); break;
	}
	return place+REG_METHOD_LEN_BITS;
}

void buildBinaryCodeFirstLn(ParsedLineNode* line, InstructImg* instructImg)
{
	struct instructNode* in;
	char* binary;
	int place = 0;;
	if ((in = (struct instructNode*) malloc(sizeof(struct instructNode))) ==NULL)
	{
		printMemEllocateError();
		exit(1);
	}
	binary = NULL;
	convertNumToBinaryStr(binary, line->typeHandle.instruct.opCode, 0, OPCODE_LEN_BITS);
	memcpy(in->instruction, binary, OPCODE_LEN_BITS); /* add opcode */
	place = addRegMethodToFirstLn(line->typeHandle.instruct.opSrcMethod, in, OPCODE_LEN_BITS); /* add src reg method */
	place = addRegMethodToFirstLn(line->typeHandle.instruct.opDstMethod, in, place); /* add dst reg method */
	memcpy(in->instruction+place, A, ARE_LEN_BITS); /* add ARE bits */
	instructImg->instructions[instructImg->ic++]=*in; /* addBinaryCodeToInstructImg */
	instructImg->instructions[instructImg->ic].instruction[MAX_WORD_LENGTH] = '\0';
	buildBinaryCodeNextLn(line, instructImg);
}

void calculateL(ParsedLineNode* line)
{
	switch(opCodesMatch[line->typeHandle.instruct.opCode].instrucGroup)
	{
		case TWO_OPERANDS: 
			if(line->typeHandle.instruct.opSrcMethod == INDIRECT_REG |
			 line->typeHandle.instruct.opSrcMethod == DIRECT_REG)
			{
				if (line->typeHandle.instruct.opDstMethod == INDIRECT_REG |
				 line->typeHandle.instruct.opDstMethod == DIRECT_REG)
				{
					line->typeHandle.instruct.addLine = ONE_ADDITIONAL_LINE;
				}
				else
				{
					line->typeHandle.instruct.addLine = TWO_ADDITIONAL_LINE;
				}
			}
			else
			{
				line->typeHandle.instruct.addLine = TWO_ADDITIONAL_LINE;
			}
			break;
		case ONE_OPERAND: line->typeHandle.instruct.addLine = ONE_ADDITIONAL_LINE; break;
		case NO_OPERANDS: line->typeHandle.instruct.addLine = NO_ADDITIONAL_LINE; break;
	}
}

void getOperandsStruct(ParsedLineNode* line, SymbTable *symbTable) 
{
	

	line->typeHandle.instruct.opSrcMethod = NO_ADD_METHOD;
	line->typeHandle.instruct.opDstMethod = NO_ADD_METHOD;

	switch(opCodesMatch[line->typeHandle.instruct.opCode].instrucGroup)
	{
		case TWO_OPERANDS:	getFirstOperand(line, symbTable); break;
		case ONE_OPERAND:	getSecOperand(line, symbTable); break;
		case NO_OPERANDS:	break;
		default:	line->error = UNEXPECTED_RESULT; break;
	}
}

void handleExternCase(ParsedLineNode* line,DataImg* dataImg,InstructImg* instructImg,
 SymbNode* symbNode, SymbTable* symbTable)
{
	if (line->symbFlag)
	{
		printf("%s\n", LABEL_BEFORE_EXTERN);
	}
	initSymbNode(line->typeHandle.et.labelName, dataImg, instructImg, symbNode, EXTERNAL_TYPE);
	addValuesInSymbTable(symbTable, symbNode);
}

void firstLineAlgo(ParsedLineNode* line, InstructImg *instructImg, DataImg *dataImg,
 SymbTable *symbTable)
{
	SymbNode *symbNode = NULL;
	void (*parseDType[NUM_OF_DATA_GUIDANCE])(ParsedLineNode*, DataImg*) = {parseDataType, parseStringType};

	if((line->symbFlag = isFirstFieldSymb(line->ln)) == 1) 
	{
		validateSymb(line, symbTable);
	} 

	/*DATA part: */
	if(isGuidanceType(line))
	{
		if (line->lineType == DATA_TYPE)
		{
			if (line->symbFlag)
			{
				initSymbNode(line->symbValue, dataImg, instructImg, symbNode, DATA_TYPE);
				if(addValuesInSymbTable(symbTable, symbNode))
				{
					line->error = DUP_LABEL_NAME_ERROR;
				}
			}
			parseDType[line->typeHandle.dataType](line, dataImg);
		}
		else if (line->lineType == EXTERNAL_TYPE)
		{
			handleExternCase(line, dataImg, instructImg, symbNode, symbTable);
		}
		return;
	}

	/*CODE part:*/
	else 
	{
		if (line->symbFlag)
		{
			initSymbNode(line->symbValue, dataImg, instructImg, symbNode, CODE_TYPE);
			addValuesInSymbTable(symbTable, symbNode);
		}
		getInstrucName(line);
		getOperandsStruct(line, symbTable);
		calculateL(line);
		buildBinaryCodeFirstLn(line, instructImg);
	}	
}


int firstIteration(char *fileName, InstructImg *instructImg, DataImg *dataImg,
 SymbTable *symbTable, ParsedFile *parsedFile)
{
	FILE *fp;
	char inFileName[FILE_NAME_MAX_LEM] ,*ln;
	ParsedLineNode *line;
	int lineNum = 0;


	instructImg->ic = 0;
	appendExtensionToFilename(inFileName, fileName, ASSEMBLY_EXTENSION);

	if ((fp = fopen(inFileName, "r")) == NULL)
	{
		printFetchFileError(inFileName);
		return 1;
	}

	while (fgets(ln, MAX_LINE_LENGTH, fp) != NULL) 
	{
		trimwhitespace(ln);
		if(!isEmptyLine(ln) & !isCommentLine(ln))
		{
			if  ((line = (ParsedLineNode*)malloc(sizeof(ParsedLineNode))) == NULL)
			{
				printMemEllocateError();
				exit(1);
			}
			initPardedLineNode(line, ln);
			firstLineAlgo(line, instructImg, dataImg, symbTable);
			addLineToParsedFile(line, parsedFile, ++lineNum);
		}
		if (line->error)
		{
			printError(line->error, parsedFile->filename, lineNum);
		}
	}
	fclose(fp);
	if(parsedFile->error)
	{
		return parsedFile->error;
	}
	updateValuesInSymbTable(symbTable, instructImg->ic);

	return NO_ERROR;
}

void secLineAlgo(ParsedLineNode* line, InstructImg *instructImg, 
	DataImg *dataImg, SymbTable *symbTable)
{
	if(line->lineType == DATA_TYPE | line->lineType == EXTERNAL_TYPE)
	{
		return;
	}
	else if (line->lineType == ENTRY_TYPE)
	{
		SymbNode *symbNode = NULL;
		if(getSymbFeature(symbTable, line->typeHandle.et.labelName, symbNode) == NULL)
		{
			line->error = LABEL_NOT_IN_SYMB_TABLE;
		}
		else
		{
			symbNode->symbType = ENTRY_TYPE;
		}
		return;
	}
	else 
	{
		updateBinaryMachineCode(line, instructImg, symbTable);
	}
	return;
}

int secIteration(InstructImg *instructImg, DataImg *dataImg, SymbTable *symbTable,
 char *fileName, ParsedFile *parsedFile)
{
	ParsedLineNode *line, *tmp;
	int lineNum = 0;
	instructImg->ic = 0;

	if  (((line = (ParsedLineNode*)malloc(sizeof(ParsedLineNode))) == NULL)
		| ((tmp = (ParsedLineNode*)malloc(sizeof(ParsedLineNode))) == NULL))
	{
		printMemEllocateError();
		exit(1);
	}
	line = parsedFile->head;
	while (line != NULL) 
	{
		secLineAlgo(line, instructImg, dataImg, symbTable);
	
		if (line->error)
		{
			printError(line->error, parsedFile->filename, ++lineNum);
		}
		line = line->next;
	}
	return NO_ERROR;
}

int ecxcute(char* fileName, InstructImg* instructImg,DataImg* dataImg,SymbTable* symbTable,ParsedFile* pf)
{
	int errorNum;
	errorNum = firstIteration(fileName, instructImg, dataImg, symbTable, pf);
	pf->filename = fileName;
	if (errorNum)
	{
		return errorNum; 
	}
	else
	{
		secIteration(instructImg, dataImg, symbTable, fileName, pf);
	}
	return errorNum; 
}
