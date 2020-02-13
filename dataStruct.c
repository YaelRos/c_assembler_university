#include "dataStruct.h"
#include "error.h"

void initDataImg(DataImg* dataImg)
{
	if ((dataImg = (DataImg*)malloc(sizeof(DataImg))) == NULL)
	{
		printMemEllocateError();
		exit(1);
	}
	dataImg->head = NULL;
	dataImg->tail = NULL;
	dataImg->dc = 0;
}

void initParsedFile(ParsedFile *pf)
{
	if ((pf = (ParsedFile*)malloc(sizeof(ParsedFile))) == NULL)
	{
		printMemEllocateError();
		exit(1);
	}
	pf->head = NULL;
	pf->tail = NULL;
	pf->count = 0;
	pf->error = 0;
}

void initSymbTable(SymbTable *symbTable)
{
	ExternTable* exTable =NULL;
	if ((symbTable = (SymbTable*)malloc(sizeof(SymbTable))) == NULL)
	{
		printMemEllocateError();
		exit(1);
	}
	initExTable(exTable);
	symbTable->head = NULL;
	symbTable->tail = NULL;
	symbTable->counter = 0;
}

void initExTable(ExternTable *exTable)
{
	if ((exTable = (ExternTable*)malloc(sizeof(ExternTable))) == NULL)
	{
		printMemEllocateError();
		exit(1);
	}
	exTable->counter =0;
}

void initPardedLineNode(ParsedLineNode* line, char* ln)
{
	if ((line = (ParsedLineNode*)malloc(sizeof(ParsedLineNode))) == NULL)
	{
		printMemEllocateError();
		exit(1);
	}
	strcpy(line->ln,ln);
	line->error = NO_ERROR;
}

SymbNode* getSymbFeature(SymbTable *symbTable, char* labelName, SymbNode *tmp)
{
	if((tmp = (SymbNode*)malloc(sizeof(SymbNode))) == NULL)
	{
		printMemEllocateError();
		exit(0);
	}
	tmp = symbTable->head;
	while (tmp!=NULL)
	{
		if (strcmp(tmp->symbName,labelName) == 0)
			return tmp;
		tmp= tmp->next;
	}
	return NULL;
}

void updateValuesInSymbTable(SymbTable *symbTable, int ic)
{
	SymbNode *tmp;

	tmp = symbTable->head;
	while (tmp != NULL)
	{
		if (tmp->symbType == DATA_TYPE)
		{
			tmp->symbAddr = tmp->symbAddr +ic+FIRST_ADDRES;
		}
		tmp = tmp->next;
	}
}

void addLineToParsedFile(ParsedLineNode *line, ParsedFile *parsedFile, int count)
{	
	parsedFile->count = count;

	if (!parsedFile->error)
	{
		parsedFile->error = 1;
	}
	if (count == 1)
	{
		parsedFile->head = line;
		parsedFile->tail = line;
	}
	else
	{
		parsedFile->tail->next = line;
	}
}

void addNumToDataImg(char* binaryStr, DataImg *dataImg)
{
	DataImgNode *tmp;
	tmp = (DataImgNode*)malloc(sizeof(DataImgNode));

	strcpy(tmp->binaryData, binaryStr);
	tmp->next = NULL;

	if (dataImg->dc == FIRST_ADDRES)
	{
		dataImg->head = tmp;
		dataImg->tail = tmp;
	}
	else
	{
		dataImg->tail->next = tmp;
	}
	dataImg->dc++;
}

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
		if(!symbTableContains(symbNode->symbName, symbTable))
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
	return 0;
}

void initSymbNode(char* name, DataImg *dataImg, 
	InstructImg *instructImg,SymbNode* symbNode, int feature)
{	
	strcpy(symbNode->symbName,name);
	symbNode->symbType = feature;

	switch (feature)
	{
		case DATA_TYPE: symbNode->symbAddr = dataImg->dc; break;
		case CODE_TYPE: symbNode->symbAddr = instructImg->ic + FIRST_ADDRES;  break;
		case EXTERNAL_TYPE: symbNode->symbAddr = 0; break;
		default: break;
	}
}

int symbTableContains(char* sy, SymbTable *symbTable)
{
	SymbNode *tmp = NULL;
	return (getSymbFeature(symbTable, sy, tmp) != NULL);
}

void setSymbValue(ParsedLineNode* line, char* label)
{
	int lenLabel;
	lenLabel = strlen(line->ln)- strlen(label);
	memcpy(line->symbValue,line->ln, lenLabel);
	line->ln = line->ln + lenLabel;
}

