#include "dataStruct.h"

void initDataImg(DataImg* dataImg)
{
	if ((dataImg = (DataImg*)malloc(sizeof(DataImg))) == NULL)
	{
		printMemEllocateError();
		exit(1);
	}
	dataImg->head = NULL;
	dataImg->dc = 0;
}

void initParsedFile(ParsedFile *pf)
{
	if ((pf = (ParsedFile*)malloc(sizeof(ParsedFile))) == NULL)
	{
		printMemEllocateError();
	}
	pf->head = NULL;
	pf->count = 0;
	pf->error = 0;
}

void initSymbTable(SymbTable *symbTable)
{
	ExternTable* exTable =NULL;
	if ((symbTable = (SymbTable*)malloc(sizeof(SymbTable))) == NULL)
	{
		printMemEllocateError();
	}
	if ((symbTable->head = (SymbNode*)malloc(sizeof(SymbNode))) == NULL)
	{
		printMemEllocateError();
	}
	exTable = initExTable(exTable);
	symbTable->counter = 0;
}

ExternTable* initExTable(ExternTable *exTable)
{
	if ((exTable = (ExternTable*)malloc(sizeof(ExternTable))) == NULL)
	{
		printMemEllocateError();
	}
	exTable->counter =0;
	return exTable;
}

ParsedLineNode* initPardedLineNode(ParsedLineNode* line)
{
	printf("%s\n", "initPardedLineNode");
	if  ((line = (ParsedLineNode*)malloc(sizeof(ParsedLineNode))) == NULL)
	{
		printMemEllocateError();
	}
	if((line->ln = (char*)malloc(sizeof(char)*MAX_LINE_LENGTH)) == NULL)
	{
		printMemEllocateError();
	}
	if((line->symbValue = (char*)malloc(sizeof(char))) == NULL)
	{
		printMemEllocateError();
	}
	line->error = NO_ERROR;
	line->next = NULL;
	printf("%s\n", "Finish to initPardedLineNode");
	return line;
}

SymbNode* getSymbFeature(SymbTable *symbTable, char* labelName)
{
	SymbNode* tmp;
	tmp = symbTable->head;
	printf("getSymbFeature: %s\n", symbTable->head->symbName);
	if ((strcmp(tmp->symbName,labelName)) == 0)
		return tmp;
	tmp = tmp->next;
	printf("getSymbFeature tmp: %s\n", tmp->symbName);
	while (tmp != (symbTable->head))
	{
		printf("getSymbFeature tmp: %s\n", tmp->symbName);
		if ((strcmp(tmp->symbName,labelName)) == 0)
			return tmp;
		tmp= tmp->next;
	}
	printf("getSymbFeature tmp: %s\n", "here1");
	return tmp;
}

void updateValuesInSymbTable(SymbTable *symbTable, int ic)
{
	SymbNode *tmp;

	tmp = symbTable->head;
	printf("updateValuesInSymbTable - type: %d\n", tmp->symbType);
	printf("updateValuesInSymbTable - tmp: %s\n", tmp->symbName);
	if (tmp->symbType == DATA_TYPE)
	{
		printf("updateValuesInSymbTable - %s\n", "if");
		tmp->symbAddr = tmp->symbAddr+ic+FIRST_ADDRES;
	}
	printf("updateValuesInSymbTable - %s\n", "here1");
	tmp = tmp->next;
	printf("updateValuesInSymbTable - tmp: %s\n", tmp->symbName);
	while (tmp != symbTable->head)
	{
		printf("updateValuesInSymbTable - %d\n", tmp->symbType);
		if (tmp->symbType == DATA_TYPE)
		{
			printf("updateValuesInSymbTable - %s\n", "if");
			tmp->symbAddr = tmp->symbAddr+ic+FIRST_ADDRES;
		}
		printf("updateValuesInSymbTable - %s\n", "here1");
		tmp = tmp->next;
	}
	printf("updateValuesInSymbTable - tmp: %s\n", tmp->symbName);
}

void addLineToParsedFile(ParsedLineNode *line, ParsedFile *pf, int count)
{	
	pf->count = count;

	if (line->error)
	{
		pf->error = 1;
	}
	if (count == 1)
	{
		pf->head = line;
	}
	else
	{
		line->next = pf->head;
		line->prev = pf->head->prev;
	}
	pf->head->prev = line;
}

void addNumToDataImg(char* binaryStr, DataImg *dataImg)
{
	DataImgNode *new, *tmp;
	new = (DataImgNode*)malloc(sizeof(DataImgNode));

	memcpy(new->binaryData, binaryStr, MAX_WORD_LENGTH+1);
	new->next = NULL;

	if (dataImg->dc == FIRST_ADDRES)
	{
		printf("addNumToDataImg - %s\n", "new");
		dataImg->head = new;
	}
	else
	{
		tmp = dataImg->head;
		while (tmp != NULL)
			tmp = tmp->next;
		tmp = new;
		printf("addNumToDataImg - %s\n", "add");

	}
	dataImg->dc++;
}

SymbNode* addValuesInSymbTable(SymbTable *symbTable, SymbNode *symbNode)
{
	int i;
	SymbNode* tmp = NULL;
	symbNode->next = symbTable->head;
	printf("addValuesInSymbTable - counter:%d\n", symbTable->counter);
	if (symbTable->counter == 0)
	{
		printf("addValuesInSymbTable - %s\n", "NULL");
		symbTable->head = symbNode;
		
	}
	else
	{
		symbNode->prev = symbTable->head->prev;
		printf("addValuesInSymbTable - add:%s\n", symbNode->symbName);
		printf("addValuesInSymbTable - head:%d\n", symbNode->symbType);
	}
	symbTable->head->prev = symbNode;
	symbTable->counter++;
	tmp = symbTable->head;
	for (i = 0; i < symbTable->counter; ++i, tmp=tmp->next)
	{
		printf("addValuesInSymbTable - name:%s\n", tmp->symbName);

	}
	return symbNode;
}

SymbNode* initSymbNode(char* name, DataImg *dataImg, 
	InstructImg *instructImg, SymbTable* symbTable, SymbNode* symbNode, int feature)
{	
	int i;
	SymbNode* tmp = NULL;
	if((symbNode = (SymbNode*) malloc (sizeof(SymbNode))) == NULL)
	{
		printMemEllocateError();
	}
	if(((symbNode->symbName) = (char*) malloc(sizeof(char)* LABEL_MAX_LEN)) == NULL)
		printMemEllocateError();
	strcpy(symbNode->symbName, name);
	printf("initSymbNode - symbName:%s\n", symbNode->symbName);
	symbNode->symbType = feature;

	switch (feature)
	{
		case DATA_TYPE: symbNode->symbAddr = dataImg->dc; break;
		case CODE_TYPE: symbNode->symbAddr = instructImg->ic + FIRST_ADDRES;  break;
		case EXTERNAL_TYPE: symbNode->symbAddr = 0; break;
		default: break;
	}
	symbNode->next = symbTable->head;
	printf("addValuesInSymbTable - counter:%d\n", symbTable->counter);
	if (symbTable->counter == 0)
	{
		printf("addValuesInSymbTable - %s\n", "NULL");
		symbTable->head = symbNode;
	}
	else
	{
		symbNode->prev = symbTable->head->prev;
		printf("addValuesInSymbTable - add:%s\n", symbNode->symbName);
		printf("addValuesInSymbTable - head:%d\n", symbNode->symbType);
	}
	symbTable->head->prev = symbNode;
	symbTable->counter++;
	tmp = symbTable->head;
	for (i = 0; i < symbTable->counter; ++i, tmp=tmp->next)
	{
		printf("addValuesInSymbTable - name:%s\n", tmp->symbName);

	}
	return symbNode;

	// return addValuesInSymbTable(symbTable, symbNode);
}

int symbTableContains(char* sy, SymbTable *symbTable)
{
	printf("symbTableContains - : %s\n", "here1");
	if (symbTable->head)
	{
		printf("symbTableContains - symbTable->head: %s\n", symbTable->head->symbName);
		if (getSymbFeature(symbTable, sy))
			return 1;	
	}
	return 0;
}

// void setSymbValue(ParsedLineNode* line, char* label)
// {
// 	int lenLabel;
// 	lenLabel = strlen(line->ln)- strlen(label);
// 	memcpy(line->symbValue,line->ln, lenLabel);
// 	line->ln = line->ln + lenLabel;
// }

