compiler: dataStruct.o utils.o parser.o error.o compiler.o main.o
	gcc -g dataStruct.o utils.o parser.o error.o compiler.o main.o -o compiler
main.o: main.c compiler.h utils.h
	gcc -c main.c -o main.o -Wall -ansi -pedantic
compiler.o: compiler.c compiler.h dataStruct.h utils.h error.h
	gcc -c compiler.c -o compiler.o -Wall -ansi -pedantic
parser.o: parser.c parser.h utils.h
	gcc -c parser.c -o parser.o -Wall -ansi -pedantic
utils.o: utils.c utils.h dataStruct.h
	gcc -c utils.c -o utils.o -Wall -ansi -pedantic
dataStruct.o: dataStruct.c dataStruct.h 
	gcc -c dataStruct.c -o dataStruct.o -Wall -ansi -pedantic
error.o: error.c error.h
	gcc -c error.c -o error.o -Wall -ansi -pedantic
