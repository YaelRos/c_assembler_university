#include <stdio.h>
#include "errors.h"
#include "utils.h"
#include "compiler.h"

int main(int argc, char *argv[]) 
{
	InstructImg instructImg;
	DataImg dataImg;
	SymbTable symbTable;
	ParsedFile pf;

	if (argc == 1) //check for file input
	{
		printMisinFileError();
		return 1;
	}
	for (int i = 0; i < argc; ++i)
	{		
		initDataImg(&dataImg);
		initSymbTable(&symbTable);
		initParsedFile(&pf);

		parsingError = excute(argv[i], instructImg, dataImg, symbTable, pf);

		if (parsingError)
		{
			printfError(error);
		}
		else
		{
			createOutputFiles();
		}
		freeMem()
		printf("The file: %s\n has compiled successfully", argv[i]);
	}
	
	
}