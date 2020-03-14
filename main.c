#include <stdio.h>
#include "compiler.h"
#include "error.h"

int main(int argc, char *argv[]) 
{
	InstructImg instructImg;
	DataImg dataImg;
	SymbTable symbTable;
	ParsedFile pf;
	int i,parsingError;

	if (argc == 1) /* check for file input */
	{
		printMisingFileError();
		return 1;
	}
	for (i = 1; i < argc; i++)
	{		
		initDataImg(&dataImg);
		initSymbTable(&symbTable);
		initParsedFile(&pf);

		parsingError = excute(argv[i], &instructImg, &dataImg, &symbTable, &pf);

		if (!parsingError)
		{
			createOutputFiles(&instructImg, argv[i], &dataImg);
			createEntryExternFiles(&symbTable, argv[i]);
			printf("The file: %s has compiled successfully \n", argv[i]);
		}
		else
		{
			printf("Can not compile the file: %s\n", argv[i]);
		}
		freeMem(&symbTable, &instructImg, &dataImg, &pf);
	}
}
