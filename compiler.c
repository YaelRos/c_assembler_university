#include "dataStruct.h"
#define LINE_MAX_LENGTH 80


buildOutPutFiles()
{

}



int firstIteration(InstructImg *instructImg, DataImg *dataImg, SymbTable *symbTable,
 char *fileName, ParsedFile *parsedFile)
{
	FILE *fp;
	ParsedLineNode *line;
	int lineNum = 0;
	instructImg->ic = 0;
	dataImg->dc = 0;

	if ((fp = fopen(fileName, 'r')) == NULL)
	{
		printError(ERROR_FETCH_FILE, fileName)
		return ERROR_FETCH_FILE;
	}

	while (fgets(&(line->line), LINE_MAX_LENGTH, &fp) != NULL) 
	{
		if(!isEmptyLine(line) & !isCommentLine(line))
		{
			if  ((line = (ParsedLineNode*)malloc(sizeof(ParsedLineNode))) == NULL)
			{
				exit(0);
				//TO do somthing when no more storage;
			}
			strcpy(line->fileName,fileName);
			line->lineNum = ++lineNum;
			parseFirstLine(line, instructImg, dataImg, symbTable);
			addLineToParsedFile(line, parsedFile, lineNum);
		}
	}
	fclose(fileName);
	if(line->error)
	{
		return line->error;
	}
	updateValuesInSymbTable(*symbTable, instructImg->ic)

	return NO_ERROR
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

int ecxcute()
{
	int error;
	ParsedFile *pf;
	error = firstIteration(*instructImg, *dataImg, *symbTable, char *fileName, ParsedFile *pf);
	if (error)
	{
		return error; 
	}
	else
	{
		secIteration();
	}
	buildOutPutFiles();

}