
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

void initSymbTable(SymbTable* symbTable)
{	
	symbTable->head = NULL;
	symbTable->counter = 0;

	symbTable->exTable = (ExternTable*)malloc(sizeof(ExternTable));
	symbTable->exTable->counter =0;
	symbTable->exTable->head = NULL;
}


ParsedLineNode* initPardedLineNode(ParsedLineNode* line)
{
	if  ((line = (ParsedLineNode*)malloc(sizeof(ParsedLineNode))) == NULL)
	{
		printMemEllocateError();
	}
	if((line->ln = (char*)malloc(sizeof(char)*MAX_LINE_LENGTH)) == NULL)
	{
		printMemEllocateError();
	}
	
	line->error = NO_ERROR;
	return line;
}

SymbNode* getSymbFeature(SymbTable *st, char* labelName)
{
    SymbNode *tmp;
   
	tmp = st->head;
    while (tmp) /* Handle empty list */
    { 
        if (strcmp(tmp->symbName, labelName) == 0) 
        {
            return tmp;
        }
        tmp = tmp->next;
	}
	return NULL;
}

void updateValuesInSymbTable(SymbTable *st, int ic)
{
	SymbNode *tmp;

	tmp = st->head;
	while(tmp)
	{
		if (tmp->symbType == DATA_TYPE)
		{
			tmp->symbAddr = tmp->symbAddr+ic+FIRST_ADDRES;
		}
		tmp = tmp->next;
	}
}

void addLineToParsedFile(ParsedLineNode *line, ParsedFile *pf)
{	
	lineDFS  *tmp, *newNode = (lineDFS*)malloc(sizeof(lineDFS)*MAX_LINES_IN_FILE);

    /* Initialize the new node */
    newNode->line = *line;
    newNode->next = NULL;


    if (pf->count == 0) { /* Handle empty list */
        pf->head = newNode;
        pf->count += 1;
        return;
    }

    tmp = pf->head;
    while (tmp->next) { /* Go to end of list */
        tmp = tmp->next;
    }
    printf("addLineToParsedFile - newNode->lineType: %d\n", newNode->line.lineType);

    tmp->next = newNode;
    pf->count += 1;  
}

void addNumToDataImg(char* binaryStr, DataImg *dataImg)
{
	DataImgNode *tmp, *new = (DataImgNode*)malloc(sizeof(DataImgNode));

	/* Initialize the new node */
	strncpy(new->binaryData, binaryStr, MAX_WORD_LENGTH+1);
	new->next = NULL;

	if (dataImg->dc == 0)
	{
		dataImg->head = new;
		dataImg->dc++;
		return;
	}

	tmp = dataImg->head;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;

	dataImg->dc++;
}
	

int initSymbNode(char* name, DataImg *dataImg, InstructImg *instructImg, 
	SymbTable* st, int feature)
{	
	SymbNode *tmp, *newNode = (SymbNode*)malloc(sizeof(SymbNode));

    /* Initialize the new node */
    strcpy(newNode->symbName, name);
    switch (feature)
	{
		case DATA_TYPE: newNode->symbAddr = dataImg->dc; break;
		case CODE_TYPE: newNode->symbAddr = instructImg->ic + FIRST_ADDRES;  break;
		case EXTERNAL_TYPE: newNode->symbAddr = 0; break;
		default: break;
	}
    newNode->symbType = feature;
    newNode->next = NULL;


    if (!st->head) { /* Handle empty list */
        st->head = newNode;
        st->counter += 1;
        return 1;
    }

    tmp = st->head;

    while (tmp->next) 
    { /* Go to end of list, while looking for a duplicate node */
        if (strcmp(tmp->symbName, name) == 0) 
        {
            return 0;
        }
        tmp = tmp->next;
    }

    tmp->next = newNode;
    st->counter = st->counter+1;  

    return 1;
}

int symbTableContains(char* sy, SymbTable *symbTable)
{
	int i;
	SymbNode* tmp;
	
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
	int symbExsit = 0;
    SymbNode *tmp;
    printf("getSymbFeature: %s\n", "here");
    /* Initialize the new node */
	tmp = st->head;
    while (tmp) /* Handle empty list */
    { 
    	printf("getSymbFeature2: %s\n", tmp->symbName);
        if (strcmp(tmp->symbName, labelName) == 0) 
        {
            tmp->symbType = ENTRY_TYPE;
            symbExsit = 1;
        }
        tmp = tmp->next;
	}
	return symbExsit;
}

void updateExTable(int mamadd, char* operand, SymbTable *symbTable)
{
	int i;
	SymbNode *tmp;

	ExternNode *tmpNode, *exNode = (ExternNode*)malloc(sizeof(ExternNode));

	exNode->memAddr = mamadd;
	exNode->next = NULL;
	strcpy(exNode->SymbName, operand);

	tmp = symbTable->head;
	printf("updateExTable - operand: %s\n", operand);
	while (tmp)
	{
		if ((tmp->symbType != EXTERNAL_TYPE) || strcmp(operand, tmp->symbName))
		{
			tmp = tmp->next;
		}
		else
		{
			/* if a label came from extern source than add the instruction line address to the exTable */
			printf("updateExTable - memAddr: %d\n", mamadd);

			if (symbTable->exTable->counter == 0) /* Handle empty list */
			{
				symbTable->exTable->head = exNode;
				symbTable->exTable->counter += 1;
				return;
			}

			tmpNode = symbTable->exTable->head;
		    while (tmpNode->next) 
		    { /* Go to end of list, while looking for a duplicate node */
		        tmpNode = tmpNode->next;
		    }

		    tmpNode->next = exNode;
			
			symbTable->exTable->counter += 1;

			printf("updateExTable - exNode.SymbName: %s\n", exNode->SymbName);
			return;
		}
	}
}


