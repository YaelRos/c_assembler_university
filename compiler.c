#include "compiler.h"

void buildBinaryCodeFirstLn(ParsedLineNode *line)
{
	int place;
	char * binary;
	InstructionNode *in;
	convertNumToBinaryStr(line->typeHandle.instruct.opCode ,binary, false, OPCODE_LEN_BITS);
	memcpy(in->instruction, binary, OPCODE_LEN_BITS); // add opcode
	place = addregMethodToFirstLn(line->typeHandle.instruct.opSrcMethod, in, OPCODE_LEN_BITS); // add src reg method
	place = addregMethodToFirstLn(line->typeHandle.instruct.opDstMethod, in, place); // add dst reg method
	mamcpy(in->instruction+place, A, ARE_LEN_BITS); // add ARE bits
	instructImg->instructions[instructImg->ic++]=in; //addBinaryCodeToInstructImg
	*(instructImg->instructions[instructImg->ic] + MAX_WORD_LENGTH+1) = '\0';
	buildBinaryCodeNextLn();
}

void buildBinaryCodeNextLn(ParsedLineNode *line)
{
	int place;
	InstructionNode *in1;
	InstructionNode *in2;
	memcpy(in1->instruction, EMPTY_BMC, MAX_WORD_LENGTH); // create an empty word in binary format and save it as an instruction.

	switch(line->typeHandle.instruct.addLine)
	{
		case ONE_ADDITIONAL_LINE: 
			addRegMethodToLn(line->typeHandle.instruct.opDstMethod, REG_PRIOR_BITS+REG_VAL_BITS, in1, line, instructImg);
			if (src != NO_ADD_METHOD)
			{
				instructImg->ic--;
				addRegMethodToLn(line->typeHandle.instruct.opSrcMethod, place, in1, line, instructImg);
			}
			break;
 		case TWO_ADDITIONAL_LINE:
 			memcpy(in2->instruction, EMPTY_BMC, MAX_WORD_LENGTH); // create an empty word in binary format and save it as an instruction.
 			addRegMethodToLn(line->typeHandle.instruct.opSrcMethod, place, in1, line, instructImg);
 			addRegMethodToLn(line->typeHandle.instruct.opSrcMethod, REG_PRIOR_BITS+REG_VAL_BITS, in2, line, instructImg);
			break;
		case NO_ADDITIONAL_LINE: break;
	}
}

void calculateL(ParsedLineNode line)
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
		case ONE_OPERAND: 
			line->typeHandle->instruct.addLine = ONE_ADDITIONAL_LINE;
			break;
		case NO_OPERANDS:
			line->typeHandle->instruct.addLine = NO_ADDITIONAL_LINE;
			break;
	}
}

void getOperandsStruct(ParsedLineNode* line, SymbTable *symbTable) 
{
	line->typeHandle.opSrcMethod = NO_ADD_METHOD;
	line->typeHandle.opDstMethod = NO_ADD_METHOD;

	switch(opCodesMatch[line->typeHandle.opCode].instrucGroup)
	{
		case TWO_OPERANDS: 
			getFirstOperand(line, symbTable);
			break;
		case ONE_OPERAND: 
			getSecOperand(line);
			break;
		case NO_OPERANDS:
			break;
	}
}

void updateBinaryMachineCode(ParsedLineNode* line, InstructImg *instructImg, 
	SymbTable *symbTable)
{
	int i;
	instructImg->ic++;

	if (line->typeHandle->instruct->opSrcMethod != DIRECT &
		line->typeHandle->instruct->opDstMethod != DIRECT )
	{
		instructImg->ic = instructImg->ic+line->typeHandle->instruct->addLine;
	}
	else if (line->typeHandle->instruct->opSrcMethod != DIRECT)
	{
		addDirectLabelToMem(line, instructImg, symbTable);
	}
	else 
	{
		addDirectLabelToMem(line, instructImg, symbTable);
		addDirectLabelToMem(line, instructImg, symbTable);
	}
}


void addRegMethodToLn(int opMethod, int place, InstructionNode *in, 
	ParsedLineNode *line, InstructImg* instructImg)
{
	char *p = line->typeHandle.instruct.opDst + 1; //less '#' or 'r' or '*'
	if (opMethod == INDIRECT_REG)
	{
		p = p + 1;//less 'r'
	}
	switch (opMethod)
	{
		case IMMEDIATE: 
			addRegToMem(p, isNegative(p), IMM_VAL_LEN_BITS, 0, in);break;
		case INDIRECT_REG: DIRECT_REG:
			addRegToMem(p, false, REG_VAL_BITS, place, in); break;
		case defalut: break;
	}
	instructImg->instructions[instructImg->ic++]=in; //addBinaryCodeToInstructImg
	*(instructImg->instructions[instructImg->ic] + MAX_WORD_LENGTH + 1) = '\0';
}

void addRegToMem(char *p, bool isNegative, int len, int place, InstructionNode *in)
{
	char *dst; 
	convertDecToBinaryStr(p ,dst, isNegative, len)
	memcpy(in->instruction+place, dst, len);
	memcpy(in->instruction+place+len, A, ARE_LEN_BITS);
}


int addRegMethodToFirstLn(int opMet, InstructionNode *in, int place)
{
	switch (opMet)
	{
		case IMMEDIATE: memcpy(in->instruction+place,"0001", REG_METHOD_LEN_BITS); break;
		case DIRECT:	memcpy(in->instruction+place, "0010", REG_METHOD_LEN_BITS); break;
		case INDIRECT_REG:	memcpy(in->instruction+place, "0100", REG_METHOD_LEN_BITS); break;
		case DIRECT_REG:	memcpy(in->instruction+place, "1000", REG_METHOD_LEN_BITS); break;
		case defalut:	memcpy(in->instruction+place, "0000", REG_METHOD_LEN_BITS); break;
	}
	return place+REG_METHOD_LEN_BITS;
}

int secIteration(InstructImg *instructImg, DataImg *dataImg, SymbTable *symbTable,
 char *fileName, parsedFile *parsedFile)
{
	FILE *fp;
	ParsedLineNode *line, *tmp;
	int lineNum = 0;
	instructImg->ic = 0;

	if  (((line = (ParsedLineNode*)malloc(sizeof(ParsedLineNode))) == NULL)
		| ((tmp = (ParsedLineNode*)malloc(sizeof(ParsedLineNode))) == NULL))
	{
		exit(0);
		//TO do somthing when no more storage;
	}
	
	while (line != NULL) 
	{
		parseSecLine(line, instructImg, dataImg, symbTable);
		if (line->storageData)
		{
			//do line 7
		}
		tmp = (ParsedLineNode*)malloc(sizeof(ParsedLineNode));
		parsedLineNodeCpy(tmp, line->next);
		line = (ParsedLineNode*)malloc(sizeof(ParsedLineNode));
		parsedLineNodeCpy(line, tmp);
	}
	if(line->error)
	{
		return line->error;
	}
	updateValuesInSymbTable(*symbTable, instructImg->ic)

	return NO_ERROR
}

void firstLineAlgo(ParsedLineNode* line, InstructImg *instructImg, DataImg *dataImg, SymbTable *symbTable)
{
	SymbNode *symbNode;
	void (parseDType[2])(ParsedLineNode*, DataImg*) = {parseDataType, parseStringType};

	if((line->symbFlag = isFirstFieldSymb(line->ln)) == 1) 
	{
		validateSymb(line, symbTable);
	} 
	if(isGuidanceType(line))
	{
		if (line->lineType == DATA_TYPE)
		{
			if (line->symbFlag)
			{
				initSymbNode(line->symbValue, dataImg, instructImg, symbNode, DATA_TYPE);
				updateValuesInSymbTable(symbTable, symbNode);
			}
			parseDType[line->dataType](line, dataImg);
		}
		else if (line->lineType == EXTERN_TYPE)
		{
			return handleExternCase(line, dataImg, instructImg, symbNode, symbTable);
		}
		
		return;
	}
	else 
	{
		if (line->symbFlag)
		{
			SymbNode *symbNode
			initSymbNode(line->symbValue, dataImg, instructImg, symbNode, CODE_TYPE);
			updateValuesInSymbTable(symbTable, symbNode);
		}
		getInstrucName(line);
		getOperandsStruct(line);
		calculateL(line);
		buildBinaryCodeFirstLn(line);
		instructImg->ic = instructImg->ic + L
	}	
}


int firstIteration(char *fileName, InstructImg *instructImg, DataImg *dataImg,
 SymbTable *symbTable, ParsedFile *parsedFile)
{
	FILE *fp;
	char inFileName, *ln;
	ParsedLineNode *line;
	int lineNum = 0;
	instructImg->ic = 0;

	appendExtensionToFilename(inFileName, fileName, ASSEMBLY_EXTENSION);

	if ((fp = fopen(inFileName, 'r')) == NULL)
	{
		printFetchFileError(inFileName);
		return ERROR_FETCH_FILE;
	}

	while (fgets(&(ln), LINE_MAX_LENGTH, &fp) != NULL) 
	{
		trimwhitespace(ln);
		if(!isEmptyLine(ln) & !isCommentLine(ln))
		{
			if  ((line = (ParsedLineNode*)malloc(sizeof(ParsedLineNode))) == NULL)
			{
				printMemEllocateError();
				return MEM_ELLOCATE_ERROR;
			}
			initPardedLineNode(line, ln, fileName, ++lineNum);
			firstLineAlgo(line, instructImg, dataImg, symbTable);
			addLineToParsedFile(line, parsedFile, lineNum);
		}
	}
	fclose(inFileName);
	if(line->error)
	{
		return line->error;
	}
	updateValuesInSymbTable(*symbTable, instructImg->ic)

	return NO_ERROR
}

int ecxcute(char* fileName, InstructImg* instructImg,DataImg* dataImg,SymbTable* symbTable,ParsedFile* pf)
{
	int errorNum;
	errorNum = firstIteration(fileName, instructImg, dataImg, symbTable, pf);
	if (errorNum)
	{
		return errorNum; 
	}
	else
	{
		secIteration();
	}
	return errorNum; 
}