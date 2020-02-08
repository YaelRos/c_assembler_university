void initSymbNode(char* name, DataImg *dataImg, 
	InstructImg *instructImg,SymbNode symbNode, int feature)
{	
	symbNode->symbName = name;
	symbNode->symbType = feature;

	switch (feature)
	{
		case DATA_TYPE: symbNode->symbAddr = dataImg->dc; break;
		case CODE_TYPE: symbNode->symbAddr = instructImg->ic + START_ADDR;  break;
		case EXTERN_TYPE: symbNode->symbAddr = 0; break;

		case defulat: break;
	}
}

void initDataImg(DataImg* dataImg)
{
	dataImg->head = NULL;
	dataImg->tail = NULL;
	dataImg->dc = 0;
}

void initParsedFile(ParsedFile *pf)
{
	pf->head = NULL;
	pf->tail = NULL;
	pf->count = 0;

}

void initSymbTable(SymbTable *symbTable)
{
	ExternNode exTable;
	initExTable(exTable);
	symbTable->head = NULL;
	symbTable->tail = NULL;
	symbTable->counter = 0;
}

void initExTable(ExternNode *exTable)
{
	exTable->counter =0;
}

void initPardedLineNode(ParsedLineNode* line, char* ln, char* fileName, char lineNum)
{
	strcpy(line->ln,ln);
	strcpy(line->fileName,fileName);
	line->lineNum = lineNum;
}
				

void addNumToDataImg(char* binaryStr, DataImg *dataImg)
{
	DataImgNode *tmp;
	tmp = (DataImgNode*)malloc(sizeof(DataImgNode));

	strcpy(tmp->binaryData, binaryStr)
	tmp->next = NULL;

	if (dataImg->dc == FIRST_ADDRES)
	{
		dataImg->haed = tmp;
		dataImg->tail = tmp;
	}
	else
	{
		dataImg->tail.next = tmp;
	}
	dataImg->dc++;
}

void addLineToParsedFile(ParsedLineNode *line, ParsedFile *parsedFile, int count)
{	
	ParsedLineNode *tmp;
	tmp = (ParsedLineNode*)malloc(sizeof(ParsedLineNode));
	parsedLineNodeCpy (tmp, line)
	tmp->next = NULL;

	parsedFile->count = count;

	if (count == 1)
	{
		parsedFile->haed = tmp;
		parsedFile->tail = tmp;
	}
	else
	{
		parsedFile->tail.next = tmp;
	}
}


SymbNode* getSymbFeature(SymbTable *symbTable, char* labelName, SymbNode *tmp)
{
	tmp = (SymbNode*) malloc(sizeof(SymbNode));
	tmp = symbTable->head;

	while(tmp != NULL)
	{
		if (strcmp(tmp.symbName,labelName) == 0)
		{
			return tmp;
		}
	}
	return NULL;
}

void updateValuesInSymbTable(SymbTable *symbTable, SymbNode *symbNode)
{
	if (symbTable->head == NULL)
	{
		symbTable->counter = START;
		symbTable->head = symbNode;
		symbTable->tail = symbNode;
	}
	else 
	{
		if(isSymbExist(symbTable, symbNode->symbValue))
		{
			symbTable->tail.next = symbNode;
			symbTable->counter++;
		}
		else
		{
			printError(SYMBOL_ALREADY_EXIST_ERROR);
		}
	}
}

