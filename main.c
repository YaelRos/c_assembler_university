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

		if (!parsingError)
		{
			createOutputFiles(&instructImg, argv[i], &dataImg);
			createEntryExternFiles(&symbTable, argv[i]);
		}
		freeMem(&symbTable, &instructImg, &dataImg, &pf);
		printf("The file: %s has compiled successfully \n", argv[i]);
	}
}
