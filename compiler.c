#include "compiler.h"

void addDirectLabelToMem(ParsedLineNode* line, char* symbVal, InstructImg *instructImg, 
	SymbTable *symbTable)
{
	SymbNode *symbNode = NULL;
	struct instructNode *in;
	char *dst = NULL; 

	if ((in = (struct instructNode*) malloc(sizeof(struct instructNode))) ==NULL)
	{
		printMemEllocateError();
	} 

	if((symbNode = getSymbFeature(symbTable, symbVal)) == NULL) /* find the symbol definition */
	{
		line->error = LABEL_NOT_IN_SYMB_TABLE;
	}
	else
	{
		/* add the converted (to binary) label address to the instruction row */
		dst = convertNumToBinaryStr(dst, symbNode->symbAddr, 0, IMM_VAL_LEN_BITS);
		strncpy(in->instruction, dst, IMM_VAL_LEN_BITS);
	}
	/* add 3 last BITs */
	if (symbNode->symbType == EXTERNAL_TYPE)
	{
		strncpy(in->instruction, EMPTY_BMC, MAX_WORD_LENGTH);
		strncpy(in->instruction+IMM_VAL_LEN_BITS, E, ARE_LEN_BITS);
		strcpy(instructImg->instructions[instructImg->ic-100].instruction, in->instruction); /* addBinaryCodeToInstructImg */
	}
	else
	{
		memcpy(in->instruction+IMM_VAL_LEN_BITS, R, ARE_LEN_BITS);
		strcpy(instructImg->instructions[instructImg->ic-100].instruction, in->instruction); /* addBinaryCodeToInstructImg */
	}
}

void updateBinaryMachineCode(ParsedLineNode* line, InstructImg *instructImg, 
	SymbTable *symbTable)
{
	if (line->typeHandle.instruct.opSrcMethod != DIRECT & line->typeHandle.instruct.opDstMethod != DIRECT ) 
	{ /* not a direct addressing method though no need to change instructions */
		instructImg->ic = instructImg->ic + line->typeHandle.instruct.addLine;
	}
	else if (line->typeHandle.instruct.opSrcMethod != DIRECT)
	{
		instructImg->ic+= line->typeHandle.instruct.addLine;
		addDirectLabelToMem(line, line->typeHandle.instruct.opDst, instructImg, symbTable);
		updateExTable(instructImg->ic, line->typeHandle.instruct.opDst, symbTable);
	}
	else if (line->typeHandle.instruct.opDstMethod != DIRECT)
	{
		instructImg->ic++;
		addDirectLabelToMem(line, line->typeHandle.instruct.opSrc, instructImg, symbTable);
		updateExTable(instructImg->ic, line->typeHandle.instruct.opSrc, symbTable);
		instructImg->ic++;
	}
	else 
	{
		instructImg->ic++;
		addDirectLabelToMem(line, line->typeHandle.instruct.opSrc, instructImg, symbTable);
		updateExTable(instructImg->ic, line->typeHandle.instruct.opSrc, symbTable);
		instructImg->ic++;
		addDirectLabelToMem(line, line->typeHandle.instruct.opDst, instructImg, symbTable);
		updateExTable(instructImg->ic, line->typeHandle.instruct.opDst, symbTable);
	}
	instructImg->ic++;
}

void addRegToMem(char *p, int isNegative, int len, int place, struct instructNode *in)
{
	char* dst = NULL;
	dst = convertDecStrToBinaryStr(dst, p, isNegative, len);;
	strncpy(in->instruction+place, dst, len);
	strncpy((in->instruction+REG_PRIOR_BITS+(REG_VAL_BITS*2)), A, ARE_LEN_BITS);
}

void addRegMethodToLn(char* op, int opMethod, int place, struct instructNode *in, ParsedLineNode *line, InstructImg* instructImg)
{
	int negative;
	char *p = op + 1; /* less '#' or 'r' or '*' */
	if (opMethod == INDIRECT_REG)
	{
		p = p + 1; /* less 'r' */
	}
	switch (opMethod)
	{
		case IMMEDIATE: 
			if ((negative = isNegative(p)) || isPositive(p))
				p++;
			addRegToMem(p, negative, IMM_VAL_LEN_BITS, 0, in);break;
		case DIRECT_REG: case INDIRECT_REG:addRegToMem(p, 0, REG_VAL_BITS, place, in); break;
		default: break;
	}	
	strcpy(instructImg->instructions[instructImg->ic].instruction, in->instruction); /* addBinaryCodeToInstructImg */
	instructImg->instructions[instructImg->ic].instruction[MAX_WORD_LENGTH] = '\0';
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

	strncpy(in1->instruction, EMPTY_BMC, MAX_WORD_LENGTH); /* create an empty word in binary format and save it as an instruction. */
	switch(line->typeHandle.instruct.addLine)
	{
		case ONE_ADDITIONAL_LINE: 
			addRegMethodToLn(line->typeHandle.instruct.opDst, line->typeHandle.instruct.opDstMethod, REG_PRIOR_BITS+REG_VAL_BITS, in1, line, instructImg);
			if (line->typeHandle.instruct.opSrcMethod != NO_ADD_METHOD)
			{
				instructImg->ic--;
				addRegMethodToLn(line->typeHandle.instruct.opSrc, line->typeHandle.instruct.opSrcMethod, place, in1, line, instructImg);
			}
			break;
 		case TWO_ADDITIONAL_LINE:
 			strncpy(in2->instruction, EMPTY_BMC, MAX_WORD_LENGTH); /* create an empty word in binary format and save it as an instruction. */
 			addRegMethodToLn(line->typeHandle.instruct.opSrc, line->typeHandle.instruct.opSrcMethod, place, in1, line, instructImg);
 			addRegMethodToLn(line->typeHandle.instruct.opDst, line->typeHandle.instruct.opDstMethod, place+REG_VAL_BITS, in2, line, instructImg);
			break;
		case NO_ADDITIONAL_LINE: break;
	}

}

int addRegMethodToFirstLn(int opMet, struct instructNode *in, int place)
{
	switch (opMet)
	{
		case IMMEDIATE: strncpy(in->instruction+place, IMMEDIATE_BITS, REG_METHOD_LEN_BITS); break;
		case DIRECT:	strncpy(in->instruction+place, DIRECT_BITS, REG_METHOD_LEN_BITS); break;
		case INDIRECT_REG:	strncpy(in->instruction+place, INDIRECT_REG_BITS, REG_METHOD_LEN_BITS); break;
		case DIRECT_REG:	strncpy(in->instruction+place, DIRECT_REG_BITS, REG_METHOD_LEN_BITS); break;
		default:	strncpy(in->instruction+place, NO_METHOD_BITS, REG_METHOD_LEN_BITS); break;
	}
	return place+REG_METHOD_LEN_BITS;
}

void buildBinaryCodeFirstLn(ParsedLineNode* line, InstructImg* instructImg)
{
	int place = 0;
	struct instructNode* in;
	char* binary = NULL;
	if ((in = (struct instructNode*) malloc(sizeof(struct instructNode))) ==NULL)
	{
		printMemEllocateError();
	}

	binary = convertNumToBinaryStr(binary, line->typeHandle.instruct.opCode, 0, OPCODE_LEN_BITS);
	strncpy(in->instruction, binary, OPCODE_LEN_BITS); /* add opcode */

	place = addRegMethodToFirstLn(line->typeHandle.instruct.opSrcMethod, in, OPCODE_LEN_BITS); /* add src reg method */
	place = addRegMethodToFirstLn(line->typeHandle.instruct.opDstMethod, in, place); /* add dst reg method */
	strncpy(in->instruction+place, A, ARE_LEN_BITS); /* add ARE bits */

	strcpy(instructImg->instructions[instructImg->ic].instruction, in->instruction); /* addBinaryCodeToInstructImg */
	instructImg->instructions[instructImg->ic].instruction[MAX_WORD_LENGTH] = '\0';

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
	switch(opCodesMatch[line->typeHandle.instruct.opCode].instrucGroup)
	{	
		case TWO_OPERANDS:	getFirstOperand(line, symbTable); break;
		case ONE_OPERAND:	getSecOperand(line, symbTable); break;
		case NO_OPERANDS:	break;
		default:	line->error = UNEXPECTED_RESULT; break;
	}
}

void handleExternCase(ParsedLineNode* line,DataImg* dataImg,InstructImg* instructImg, SymbTable* symbTable)
{
	if (line->symbFlag)
	{
		printf("warning: %s\n", LABEL_BEFORE_EXTERN);
	}
	if (!initSymbNode(line->typeHandle.et.labelName, dataImg, instructImg, symbTable, EXTERNAL_TYPE))
		line->error = DUP_LABEL_NAME_ERROR;
}


void firstLineAlgo(ParsedLineNode* line, InstructImg *instructImg, DataImg *dataImg,
 SymbTable *symbTable)
{
	SymbNode * tmp;
	void (*parseDType[NUM_OF_DATA_GUIDANCE])(ParsedLineNode*, DataImg*) = {parseDataType, parseStringType};

	if((line->symbFlag = isFirstFieldSymb(line->ln)) == 1) 
	{
		validateSymb(line, symbTable);
	} 
	tmp = symbTable->head;
	while (tmp) 
	{ /* Go to end of list, while looking for a duplicate node */
    	tmp = tmp->next;
    }
	/*DATA part: */
	if(isGuidanceType(line))
	{
		if (line->lineType == DATA_TYPE)
		{
			if (line->symbFlag)
			{
				if(!initSymbNode(line->symbValue, dataImg, instructImg, symbTable, DATA_TYPE))
				{
					line->error = DUP_LABEL_NAME_ERROR;
				}
			}
			parseDType[line->typeHandle.dataType](line, dataImg);
		}
		else if (line->lineType == EXTERNAL_TYPE)
		{
			handleExternCase(line, dataImg, instructImg, symbTable);
		}
		return;
	}

	/*CODE part:*/
	else 
	{
		if (line->symbFlag)
		{
			if(!initSymbNode(line->symbValue, dataImg, instructImg, symbTable, CODE_TYPE))
				line->error = DUP_LABEL_NAME_ERROR;
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
	char inFileName[FILE_NAME_MAX_LEM];
	SymbNode* tmp;
	ParsedLineNode *line= NULL;
	instructImg->ic = 0;
	line = initPardedLineNode(line);
	
	appendExtensionToFilename(inFileName, fileName, ASSEMBLY_EXTENSION);

	if ((fp = fopen(inFileName, "r")) == NULL)
	{
		printFetchFileError(inFileName);
		return 1;
	}
	
	while (fgets(line->ln, MAX_LINE_LENGTH, fp) != NULL) 
	{
		tmp = symbTable->head;
		while (tmp) 
		{ /* Go to end of list, while looking for a duplicate node */
        	tmp = tmp->next;
        }
		line->ln = trimwhitespace(line->ln);
		if(!isEmptyLine(line->ln) & !isCommentLine(line->ln))
		{
			tmp = symbTable->head;
    		while (tmp) 
    		{ /* Go to end of list, while looking for a duplicate node */
	        	tmp = tmp->next;
	        }
			firstLineAlgo(line, instructImg, dataImg, symbTable);
			tmp = symbTable->head;
    		while (tmp) 
    		{ /* Go to end of list, while looking for a duplicate node */
	        	tmp = tmp->next;
	        }
			addLineToParsedFile(line, parsedFile);
			tmp = symbTable->head;
			while (tmp) 
			{ /* Go to end of list, while looking for a duplicate node */
	        	tmp = tmp->next;
	        }
		}
		if (line->error)
		{	
			printError(line->error, parsedFile->filename, parsedFile->count);
		}
		tmp = symbTable->head;
		while (tmp) 
		{ /* Go to end of list, while looking for a duplicate node */
        	tmp = tmp->next;
        }
		line = initPardedLineNode(line);

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
	if(line->lineType == DATA_TYPE || line->lineType == EXTERNAL_TYPE)
	{
		return;
	}
	else if (line->lineType == ENTRY_TYPE)
	{
		if(!setSymbEntryType(symbTable, line->typeHandle.et.labelName))
		{
			line->error = LABEL_NOT_IN_SYMB_TABLE;
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
	lineDFS *row;
	int lineNum = 1;
	instructImg->ic = FIRST_ADDRES;
	if ((row = (lineDFS*)malloc(sizeof(lineDFS))) == NULL)
	{
		printMemEllocateError();
	}
	row = parsedFile->head;
	while (lineNum< parsedFile->count) 
	{
		secLineAlgo(&row->line, instructImg, dataImg, symbTable);
		if (row->line.error)
		{
			printError(row->line.error, parsedFile->filename, lineNum);
		}
		lineNum++;
		row = row->next;
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
