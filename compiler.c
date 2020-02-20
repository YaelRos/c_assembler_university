#include "compiler.h"

/* extern OpCode opCodesMatch[]; */

void addDirectLabelToMem(ParsedLineNode* line, InstructImg *instructImg, 
	SymbTable *symbTable)
{
	SymbNode *symbNode = NULL;
	struct instructNode *in = NULL;
	char *dst = NULL; 

	if((symbNode = getSymbFeature(symbTable, line->symbValue)) == NULL)
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
		updateExTable(instructImg->ic, line->typeHandle.instruct.opSrc, symbTable);
		addDirectLabelToMem(line, instructImg, symbTable);
	}
	else 
	{
		updateExTable(instructImg->ic, line->typeHandle.instruct.opSrc, symbTable);
		addDirectLabelToMem(line, instructImg, symbTable);
		updateExTable(instructImg->ic, line->typeHandle.instruct.opDst, symbTable);
		addDirectLabelToMem(line, instructImg, symbTable);
	}
}

void addRegToMem(char *p, int isNegative, int len, int place, struct instructNode *in)
{
	char *dst = (char *) malloc(sizeof(char)); 
	printf("addRegToMem isNegative:%d\n", isNegative);
	convertDecStrToBinaryStr(dst, p, isNegative, len);
	printf("addRegToMem dst:%s\n", dst);
	memcpy((in->instruction+place), dst, len);
	printf("addRegToMem in->instruction:%s\n", in->instruction);
	memcpy((in->instruction+REG_PRIOR_BITS+(REG_VAL_BITS*2)), A, ARE_LEN_BITS);
}

void addRegMethodToLn(char* op, int opMethod, int place, struct instructNode *in, 
	ParsedLineNode *line, InstructImg* instructImg)
{
	char *p = op + 1; /* less '#' or 'r' or '*' */
	int negative;
	printf("addRegMethodToLn - operand: %s\n", p);
	if (opMethod == INDIRECT_REG)
	{
		p = p + 1; /* less 'r' */
	}
	printf("addRegMethodToLn - p:%s\n", p);
	printf("addRegMethodToLn - opMethod:%d\n", opMethod);
	switch (opMethod)
	{
		case IMMEDIATE: 
			if ((negative = isNegative(p)) || isPositive(p))
				p++;
			addRegToMem(p, negative, IMM_VAL_LEN_BITS, 0, in);break;
		case DIRECT_REG: case INDIRECT_REG:
		 addRegToMem(p, 0, REG_VAL_BITS, place, in); break;
		default: break;
	}	
	strcpy(instructImg->instructions[instructImg->ic].instruction, in->instruction); /* addBinaryCodeToInstructImg */
	instructImg->instructions[instructImg->ic].instruction[MAX_WORD_LENGTH] = '\0';
	printf("addRegMethodToLn - %s\n", instructImg->instructions[instructImg->ic].instruction);
	instructImg->ic++;
}

void buildBinaryCodeNextLn(ParsedLineNode *line, InstructImg* instructImg)
{
	int place = REG_PRIOR_BITS;
	struct instructNode *in1, *in2;
	in2 = (struct instructNode*) malloc(sizeof(struct instructNode));
	if (((in1 = (struct instructNode*) malloc(sizeof(struct instructNode))) ==NULL)
		|| (in2 == NULL) )
	{
		printMemEllocateError();
	}

	memcpy(in1->instruction, EMPTY_BMC, MAX_WORD_LENGTH); /* create an empty word in binary format and save it as an instruction. */
	printf("buildBinaryCodeNextLn - empty instruction:%s\n", in1->instruction);
	switch(line->typeHandle.instruct.addLine)
	{
		case ONE_ADDITIONAL_LINE: 
			addRegMethodToLn(line->typeHandle.instruct.opDst, line->typeHandle.instruct.opDstMethod, REG_PRIOR_BITS+REG_VAL_BITS, in1, line, instructImg);
			printf("buildBinaryCodeNextLn - opSrcMeth: %d\n", line->typeHandle.instruct.opSrcMethod);
			if (line->typeHandle.instruct.opSrcMethod != NO_ADD_METHOD)
			{
				instructImg->ic--;
				addRegMethodToLn(line->typeHandle.instruct.opSrc, line->typeHandle.instruct.opSrcMethod, place, in1, line, instructImg);
			}
			printf("buildBinaryCodeNextLn - 1 ->in1: %s\n", in1->instruction);
			break;
 		case TWO_ADDITIONAL_LINE:
 			memcpy(in2->instruction, EMPTY_BMC, MAX_WORD_LENGTH); /* create an empty word in binary format and save it as an instruction. */
 			addRegMethodToLn(line->typeHandle.instruct.opSrc, line->typeHandle.instruct.opSrcMethod, place, in1, line, instructImg);
 			addRegMethodToLn(line->typeHandle.instruct.opDst, line->typeHandle.instruct.opDstMethod, place+REG_VAL_BITS, in2, line, instructImg);
			printf("buildBinaryCodeNextLn - 2 -> in1: %s\n", in1->instruction);
			printf("buildBinaryCodeNextLn - 2 -> in2: %s\n", in2->instruction);
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
	int place = 0;
	binary = (char*) malloc(sizeof(char));
	if (((in = (struct instructNode*) malloc(sizeof(struct instructNode))) ==NULL)
		|| (binary == NULL) )
	{
		printMemEllocateError();
	}
	convertNumToBinaryStr(binary, line->typeHandle.instruct.opCode, 0, OPCODE_LEN_BITS);
	memcpy(in->instruction, binary, OPCODE_LEN_BITS); /* add opcode */

	place = addRegMethodToFirstLn(line->typeHandle.instruct.opSrcMethod, in, OPCODE_LEN_BITS); /* add src reg method */
	place = addRegMethodToFirstLn(line->typeHandle.instruct.opDstMethod, in, place); /* add dst reg method */
	memcpy(in->instruction+place, A, ARE_LEN_BITS); /* add ARE bits */

	strcpy(instructImg->instructions[instructImg->ic].instruction, in->instruction); /* addBinaryCodeToInstructImg */
	instructImg->instructions[instructImg->ic].instruction[MAX_WORD_LENGTH] = '\0';
	printf("buildBinaryCodeFirstLn1 - in->instruction:%s\n", instructImg->instructions[instructImg->ic].instruction);

	instructImg->ic++;
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
	printf("getOperandsStruct - %d\n", opCodesMatch[line->typeHandle.instruct.opCode].instrucGroup);
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
	symbNode = initSymbNode(line->typeHandle.et.labelName, dataImg, instructImg, symbTable, symbNode, EXTERNAL_TYPE);
}

void firstLineAlgo(ParsedLineNode* line, InstructImg *instructImg, DataImg *dataImg,
 SymbTable *symbTable)
{
	SymbNode *symbNode = NULL;
	void (*parseDType[NUM_OF_DATA_GUIDANCE])(ParsedLineNode*, DataImg*) = {parseDataType, parseStringType};

	printf("firstLineAlgo 1 - line->ln: %s. \n", line->ln);
	if((line->symbFlag = isFirstFieldSymb(line->ln)) == 1) 
	{
		validateSymb(line, symbTable);
		printf("firstLineAlgo 2 - line->ln: %s. \n", line->ln);
	} 
	
	/*DATA part: */
	if(isGuidanceType(line))
	{
		printf("firstLineAlgo 3 - line->ln: %s. \n", line->ln);
		if (line->lineType == DATA_TYPE)
		{
			printf("firstLineAlgo 3o - line->ln: %s. \n", line->ln);
			if (line->symbFlag)
			{
				printf("firstLineAlgo 3a - line->ln: %s. \n", line->ln);
				symbNode=initSymbNode(line->symbValue, dataImg, instructImg, symbTable, symbNode, DATA_TYPE);
				printf("firstLineAlgo 3b - line->ln: %s. \n", line->ln);
				if(symbNode == NULL)
				{
					printf("firstLineAlgo 3c - line->ln: %s. \n", line->ln);
					line->error = DUP_LABEL_NAME_ERROR;
				}
			}
			printf("firstLineAlgo 3d - line->ln: %s. \n", line->ln);
			parseDType[line->typeHandle.dataType](line, dataImg);
			printf("firstLineAlgo 3e - line->ln: %s. \n", line->ln);
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
		printf("firstLineAlgo 3b - line->ln: %s. \n", line->ln);
		if (line->symbFlag)
		{
			printf("firstLineAlgo 4b - line->symbValue: %s. \n", line->symbValue);
			symbNode = initSymbNode(line->symbValue, dataImg, instructImg, symbTable, symbNode, CODE_TYPE);
			printf("firstLineAlgo 5b - line->ln: %s. \n", line->ln);
		}
		getInstrucName(line);
		printf("firstLineAlgo 7b - line->ln: %s. \n", line->ln);
		getOperandsStruct(line, symbTable);
		printf("firstLineAlgo 8b - line->ln: %s. \n", line->ln);
		calculateL(line);
		printf("firstLineAlgo 9b - addLine: %d. \n", line->typeHandle.instruct.addLine);
		buildBinaryCodeFirstLn(line, instructImg);
		printf("firstLineAlgo 10b - line->ln: %s. \n", line->ln);
	}	
}


int firstIteration(char *fileName, InstructImg *instructImg, DataImg *dataImg,
 SymbTable *symbTable, ParsedFile *parsedFile)
{
	FILE *fp;
	char inFileName[FILE_NAME_MAX_LEM];
	ParsedLineNode *line= NULL;
	int lineNum = 0;
	instructImg->ic = 0;
	line = initPardedLineNode(line);

	appendExtensionToFilename(inFileName, fileName, ASSEMBLY_EXTENSION);
	printf("file name - %s: %s\n",inFileName, "first iteration 1");

	if ((fp = fopen(inFileName, "r")) == NULL)
	{
		printFetchFileError(inFileName);
		return 1;
	}
	
	printf("line->ln: %s. %s\n", (line->ln), "first iteration 2");
	while (fgets(line->ln, MAX_LINE_LENGTH, fp) != NULL) 
	{
		lineNum++;
		printf("%s\n", "first iteration 3");
		line->ln = trimwhitespace(line->ln);
		if(!isEmptyLine(line->ln) & !isCommentLine(line->ln))
		{
			printf("%s\n", "first iteration 4");

			firstLineAlgo(line, instructImg, dataImg, symbTable);
			printf("%s\n", "first iteration 5");

			addLineToParsedFile(line, parsedFile, lineNum);
			printf("%s\n", "first iteration 6");

		}
		if (line->error)
		{	
			printError(line->error, parsedFile->filename, lineNum);
		}
		printf("%s\n", "first iteration 7");

	}
	fclose(fp);
	if(parsedFile->error)
	{
		return parsedFile->error;
	}
	printf("%s\n", "first iteration 8");
	updateValuesInSymbTable(symbTable, instructImg->ic);
	printf("%s\n", "first iteration 9");
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
		if((symbNode = getSymbFeature(symbTable, line->typeHandle.et.labelName)) == NULL)
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

int excute(char* fileName, InstructImg* instructImg,DataImg* dataImg,SymbTable* symbTable,ParsedFile* pf)
{
	int errorNum;
	pf->filename = fileName;
	errorNum = firstIteration(fileName, instructImg, dataImg, symbTable, pf);
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
