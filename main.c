#include <stdio.h>
#include "parser.h"
#include "assembler.h"
#include "errors.h"
#include "utils.h"

int main(int argc, char *argv[]) 
{

	if (argc == 1)
		#TO DO: print error - missing file;
		return 1;

	for (int i = 0; i < count; ++i)
	{
		init();
		error = excute(argv[i]);
		if (error)
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