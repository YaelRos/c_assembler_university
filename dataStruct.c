#include "dataStruct.h"

void initDataImg(DataImg* dataImg)
{
	if ((dataImg = (DataImg*)malloc(sizeof(DataImg))) == NULL)
	{
		printMemEllocateError();
	}
	dataImg->head = NULL;
	dataImg->dc = 0;
}

void initParsedFile(ParsedFile *pf)
{
	pf->head = NULL;
	pf->count = 0;
	pf->error = 0;
}

void initSymbTable(SymbTable *symbTable)
{
	symbTable->head = NULL;
	symbTable->counter = 0;

	symbTable->exTable = (ExternTable*)malloc(sizeof(ExternTable));
	symbTable->exTable->counter =0;
	symbTable->exTable->head = NULL;

}


ParsedLineNode* initPardedLineNode(ParsedLineNode* line)
{
	printf("%s\n", "initPardedLineNode");
	if  ((line = (ParsedLineNode*)malloc(sizeof(ParsedLineNode)*MAX_LINES_IN_FILE)) == NULL)
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
	printf("%s\n", "Finish to initPardedLineNode");
	return line;
}

SymbNode* getSymbFeature(SymbTable *st, char* labelName)
{
	int i;
    SymbNode *tmp;
    printf("getSymbFeature: %s\n", "here");
    /* Initialize the new node */
	tmp = st->head;
    printf("getSymbFeature1: %d\n", st->counter);
    for (i = 0; i<st->counter; i++) /* Handle empty list */
    { 
    	printf("getSymbFeature2 - tmp->symbName: %s\n", tmp->symbName);
	  	printf("getSymbFeature2 - labelName: %s\n", labelName);
        if (strcmp(tmp->symbName, labelName) == 0) 
        {
            return tmp;
        }
        tmp = tmp->next;
	}
	return NULL;
}

void updateValuesInSymbTable(SymbTable *symbTable, int ic)
{
	SymbNode *tmp;
	int i;
	tmp = symbTable->head;
	for(i =0 ; i< symbTable->counter; i++)
	{
		printf("updateValuesInSymbTable - tmp: %s\n", tmp->symbName);
		if (tmp->symbType == DATA_TYPE)
		{
			printf("updateValuesInSymbTable - %s, %d\n",tmp->symbName, tmp->symbAddr);
			tmp->symbAddr = tmp->symbAddr+ic+FIRST_ADDRES;
			printf("updateValuesInSymbTable - %d\n", tmp->symbAddr);	
		}
		tmp = tmp->next;
	}
}

void addLineToParsedFile(ParsedLineNode *line, ParsedFile *pf)
{	
	lineDFS *newNode = (lineDFS*)malloc(sizeof(lineDFS)*MAX_LINES_IN_FILE);
    lineDFS *tmp;

    /* Initialize the new node */
    newNode->line = *line;
    newNode->next = NULL;
	printf("addLineToParsedFile - %s\n", "here");


    if (pf->count == 0) { /* Handle empty list */
        pf->head = newNode;
        pf->count += 1;
        return;
    }

    tmp = pf->head;
    printf("addLineToParsedFile - count: %d\n", pf->count);
    printf("addLineToParsedFile - tmp->lineType: %d\n", tmp->line.lineType);
    while (tmp->next) { /* Go to end of list */
        tmp = tmp->next;
        printf("addLineToParsedFile - %d+", 1);
    }
    printf("addLineToParsedFile - newNode->lineType: %d\n", newNode->line.lineType);

    tmp->next = newNode;
    pf->count += 1;  
    printf("addLineToParsedFile, count:%d\n", pf->count);  	
}

void addNumToDataImg(char* binaryStr, DataImg *dataImg)
{
	DataImgNode *new, *tmp;
	if((new = (DataImgNode*)malloc(sizeof(DataImgNode))) == NULL)
		printMemEllocateError();

	strncpy(new->binaryData, binaryStr, MAX_WORD_LENGTH+1);
	new->next = NULL;
	if (dataImg->dc == 0)
	{
		printf("addNumToDataImg - %s\n", "new");
		dataImg->head = new;
	}
	else
	{
		tmp = dataImg->head;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new;
		printf("addNumToDataImg - %s\n", "add");
		printf("addNumToDataImg dataImg->dc - %d\n", dataImg->dc);
	}
	dataImg->dc++;
}
	

int initSymbNode(char* name, DataImg *dataImg, InstructImg *instructImg, 
	SymbTable* st, int feature)
{	
	SymbNode *newNode = (SymbNode*)malloc(sizeof(SymbNode));
    SymbNode *tmp;
    printf("initSymbNode: %s\n", "here");
    /* Initialize the new node */
    strcpy(newNode->symbName, name);
    printf("initSymbNode: %s\n", newNode->symbName);
    printf("initSymbNode: %d\n", instructImg->ic + FIRST_ADDRES);
    printf("initSymbNode: %d\n", dataImg->dc);

    switch (feature)
	{
		case DATA_TYPE: newNode->symbAddr = dataImg->dc; break;
		case CODE_TYPE: newNode->symbAddr = instructImg->ic + FIRST_ADDRES;  break;
		case EXTERNAL_TYPE: newNode->symbAddr = 0; break;
		default: break;
	}
    newNode->symbType = feature;

    newNode->next = NULL;

    if (st->counter == 0) { /* Handle empty list */
        st->head = newNode;
        st->counter += 1;
        return 0;
    }

    tmp = st->head;

    while (tmp->next) 
    { /* Go to end of list, while looking for a duplicate node */
        if (strcmp(tmp->symbName, name) == 0) 
        {
            return -1;
        }
        printf("initSymbNode: %s\n", tmp->symbName);
        printf("initSymbNode: %d\n", st->counter);
        tmp = tmp->next;
    }

    tmp->next = newNode;
    printf("initSymbNode: %s\n", tmp->symbName);
    st->counter += 1;  

    return 0;
}

int symbTableContains(char* sy, SymbTable *symbTable)
{
	SymbNode* tmp;
	int i;
	printf("symbTableContains - : %s\n", "here1");

	tmp = symbTable->head;
	for (i = 0; i < symbTable->counter; i++, tmp=tmp->next)
	{
		printf("symbTableContains - : %s\n", "here2");
		if ((strcmp(tmp->symbName,sy)) == 0)
				return 1; 
	}
	return 0;
}

int setSymbEntryType(SymbTable *st, char* labelName)
{
	int i, symbExsit = 0;
    SymbNode *tmp;
    printf("getSymbFeature: %s\n", "here");
    /* Initialize the new node */
	tmp = st->head;
    printf("getSymbFeature1: %d\n", st->counter);
    for (i = 0; i<st->counter; i++) /* Handle empty list */
    { 
    	printf("getSymbFeature2: %s\n", tmp->symbName);
        if (strcmp(tmp->symbName, labelName) == 0) 
        {
            tmp->symbType = ENTRY_TYPE;
            symbExsit = 1;
        }
        tmp = tmp->next;
	}
	return 0;
}

void updateExTable(int mamadd, char* operand, SymbTable *symbTable)
{
	int i;
	SymbNode *tmp;

	ExternNode *exNode = (ExternNode*)malloc(sizeof(ExternNode));
    ExternNode *tmpNode;

	tmp = symbTable->head;
	printf("updateExTable - operand: %s\n", operand);
	for (i = 0; i < symbTable->counter; i++)
	{
		if ((tmp->symbType != EXTERNAL_TYPE) || strcmp(operand, tmp->symbName))
		{
			tmp = tmp->next;
		}
		else
		{
			/* if a label came from extern source than add the instruction line address to the exTable */
			printf("updateExTable - memAddr: %d\n", mamadd);
			exNode->memAddr = mamadd;
			exNode->next = NULL;
			strcpy(exNode->SymbName, operand);

			if (symbTable->exTable->counter == 0)
			{
				symbTable->exTable->head = exNode;
			}
			else
			{
				tmpNode = symbTable->exTable->head;

			    while (tmpNode->next) 
			    { /* Go to end of list, while looking for a duplicate node */
			        tmpNode = tmpNode->next;
			    }

			    tmpNode->next = exNode;
			}
			symbTable->exTable->counter += 1;

			printf("updateExTable - exNode.SymbName: %s\n", exNode->SymbName);
			return;	
		}
	}
}


