#include <cstdio>
#include <cstdlib>
#include <assert.h>

//#define DEBUG
#include "myDefines.hpp"

#include "classHash.hpp"
#include "hashFunc.hpp"

enum
{
	MAX_FILE_NAME = 32,
};

int main()
{
#ifndef DEBUG
	printf("\n\tPlease, enter the name of file with data.\n");
	char name[MAX_FILE_NAME];
	assert(MAX_FILE_NAME == 32);
	scanf("%31s", name);	
	FILE* source = fopen(name, "r");
	if(!source)
	{
		perror(name);
		exit(1);
	}
#else //DEBUG
	FILE* source = fopen("words", "r");
#endif //DEBUG
	Hash hash(source);
	hash.makeTable(&hashSumCycleShiftedCodesOfLetters);
	hash.printPlot();
	fclose(source);
	return 0;
}
