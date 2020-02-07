void initSymbNode(char* name, DataImg *dataImg, 
	InstructImg *instructImg,SymbNode symbNode, int feature)
{	
	symbNode->symbName = name;
	symbNode->symbType = feature;

	switch (feature)
	{
		case DATA_TYPE: symbNode->symbAddr = dataImg->dc; break;
		case CODE_TYPE: symbNode->symbAddr = instructImg->ic + START_ADDR;  break;
		case defulat: break;
	}
}


void addNumToDataImg(char* binaryStr, DataImg dataImg)
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

