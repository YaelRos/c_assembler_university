#include "dataStruct.h"

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
	pf->error = 0;
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

void initPardedLineNode(ParsedLineNode* line, char* ln)
{
	strcpy(line->ln,ln);
	line->error = NO_ERROR;
}

void setSymbFromExternEntry(ParsedLineNode* line, int etLen)
{
	line->line = line->line + etLen
	trimwhitespace(line->line);
	memcpy(line->typeHandle->et.labelName, line->line);
}

/*
	Get the symbol node from the symbol table that has the same name like the labelName.

    @param SymbTable* symbTable - The symbol table
	@param char* labelName - The symbol value to b compered.
	@param SymbNode *tmp - The symbol node.
	@return SymbNode* - the symbol node from the symbol table that has the same name like the labelName,
	if there's not such a node, return NULL.
*/
SymbNode* getSymbFeature(SymbTable *symbTable, char* labelName, SymbNode *tmp)
{
	SymbNode *tmp;
	if((tmp = (SymbNodes*)malloc(sizeof(SymbNodes))) == NULL)
	{
		printMemEllocateError();
		exit(0);
	}
	tmp = symbTable->head;
	for (tmp!=NULL)
	{
		if (strcmp(tmp->symbValue,labelName) == 0)
			return tmp;
		tmp= tmp->next;
	}
	return NULL;
}

/*
	Add 100+IC to all the address of symbols from type DATA.

    @param SymbTable* symbTable - The symbol table
    @param int ic - IC 
*/
void updateValuesInSymbTable(SymbTable *symbTable, int ic)
{
	SymbNode *tmp;

	tmp = symbTable->head;
	while (tmp != NULL)
	{
		if (tmp->symbType == DATA_TYPE)
		{
			tmp->symbAddr == tmp->symbAddr +ic+START_ADDR;
		}
		tmp = tmp->next;
	}
}

/*
	Add a line object to a parsed file. 

	@param ParsedLineNode* line - The object of the current line. 
    @param ParsedFile* parsedFile - The object of the current file. 
    @param int count - number of lines that already read.
*/
void addLineToParsedFile(ParsedLineNode *line, ParsedFile *parsedFile, int count)
{	
	parsedFile->count = count;

	if (!parsedFile->error)
	{
		parsedFile->error = 1;
	}
	if (count == 1)
	{
		parsedFile->haed = line;
		parsedFile->tail = line;
	}
	else
	{
		parsedFile->tail.next = line;
	}
}

/*
	Add a new number to the Data image and increasing the data image counter by 1.

    @param char* binaryStr - The number represented by binary string
    @param DataImg* dataImg - The data image
*/
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

/*
	Add a new node to the symbol table.
    If the record already exists in our symbol table, return 1.
    @param SymbNode* symbNode - An empty symbol node 
    @param SymbTable* symbTable - The symbol table
    @return int - A flag representing if the symbol already exists in our symbol table. 0 = False, 1 = True
*/
int addValuesInSymbTable(SymbTable *symbTable, SymbNode *symbNode)
{
	if (symbTable->head == NULL)
	{
		symbTable->counter = 1;
		symbTable->head = symbNode;
		symbTable->tail = symbNode;
	}
	else 
	{
		if(!symbTableContains(symbNode->symbValue, symbTable))
		{
			symbTable->tail->next = symbNode;
			symbTable->counter++;
			return 0;
		}
		else
		{
			return 1;
		}
	}
}

/*
	initialize nre symbol node
	@param char* name - The name of the symbol
	@param DataImg* dataImg - The data image
    @param InstructImg* instructImg - The instruction image
    @param SymbNode* symbNode - An empty symbol node 
    @param int feature - The feature of the symbol
*/
void initSymbNode(char* name, DataImg *dataImg, 
	InstructImg *instructImg,SymbNode* symbNode, int feature)
{	
	strcpy(symbNode->symbName,name);
	symbNode->symbType = feature;

	switch (feature)
	{
		case DATA_TYPE: symbNode->symbAddr = dataImg->dc; break;
		case CODE_TYPE: symbNode->symbAddr = instructImg->ic + START_ADDR;  break;
		case EXTERN_TYPE: symbNode->symbAddr = 0; break;
		case defulat: break;
	}
}

/*
	Check if the symbol table contains the symbol value sy
	@param char* sy - The symbol value
    @param SymbTable* symbTable - The symbol table
	@return int - A flag representing if the symbol value is in the symbol table. 0 = False, 1 = True
*/
int symbTableContains(char* sy, SymbTable *symbTable)
{
	SymbNode *tmp;
	return (getSymbFeature(SymbTable *symbTable, char* sy, SymbNode *tmp) != NULL)
}

/*
	Set the symbol value in the line object to be 
	the symbol which defined at the beginning of the line,
	and move the string pointer forward as the length of the symbol value.
	@param ParsedLineNode* line - The object of the current line. 
	@param char* label - The substring of line->ln which start at the end of the label.
*/
void setSymbValue(ParsedLineNode* line, char* label)
{
	int lenLabel;
	lenLabel = strlen(line->ln)- strlen(label);
	memcpy(line->symbValue,line->ln, lenLabel);
	line->ln = line->ln + lenLabel;
}

