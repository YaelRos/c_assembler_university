#include <stdio.h>
#include "compiler.h"

int main(int argc, char *argv[]) 
{
	InstructImg instructImg;
	DataImg dataImg;
	SymbTable symbTable;
	ParsedFile pf;
	int i,parsingError;

	if (argc == 1) /* check for file input */
	{
		printMisinFileError();
		return 1;
	}
	for (i = 1; i < argc; i++)
	{		
		initDataImg(&dataImg);
		initSymbTable(&symbTable);
		initParsedFile(&pf);
		printf("%s %d\n", argv[i], i);
		parsingError = excute(argv[i], &instructImg, &dataImg, &symbTable, &pf);
		printf("Main - Error in The file: %d\n", parsingError);

		if (!parsingError)
		{
			createOutputFiles(&instructImg, argv[i], &dataImg);
			printf("Main - createOutputFiles:\n");
			createEntryExternFiles(&symbTable, argv[i]);
			printf("Main - createEntryExternFiles:\n");
		}
		freeMem(&symbTable, &instructImg, &dataImg, &pf);
		printf("The file: %s has compiled successfully \n", argv[i]);
	}
}
