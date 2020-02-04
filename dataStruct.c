void initSymbNode(ParsedLineNode* line, DataImg *dataImg, 
	InstructImg *instructImg,SymbNode symbNode, int feature)
{	
	symbNode->symbName = line->symbValue;
	if (feature == DATA) 
	{
		symbNode->symbType = DATA;
		symbNode->symbAddr = dataImg->dc;
	}
	else if (feature == CODE) 
	{
		symbNode->symbType = CODE;
		symbNode->symbAddr = instructImg->ic + START_ADDR;
	}
	else if (feature == EXTERN)
	{
		symbNode->symbType = EXTERN;
	}

	return symbName;
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

