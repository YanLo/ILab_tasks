#include "getfiles.hpp"
#include <stdio.h>
#include <stdlib.h>

//#define DEBUG
#include "myDefines.hpp"

FILE* getInputFile(const char* fileName)
{
	FUNC_HI
	FILE* file = fopen(fileName, "r");
	if(file == NULL)
	{
		perror(fileName);
		exit(1);
	}
	return file;
	FUNC_BYE
}

FILE* getOutputFile(const char* fileName)
{
	FUNC_HI
	FILE* file = fopen(fileName, "w");
	if(file == NULL)
	{
		perror(fileName);
		exit(1);
	}
	return file;

	FUNC_BYE
	return file;
}

FILE* getDotFile()
{
	FUNC_HI
	FILE* file = fopen("dumpTree.gv", "w");
	if(file == NULL)
	{
		perror("dumpTree.gv");
		exit(1);
	}
	FUNC_BYE
	return file;
}

FILE* getInputFile()
{
	FUNC_HI
	FILE* file = fopen("Tree", "r");
	if(file == NULL)
	{
		perror("Tree");
		exit(1);
	}
	FUNC_BYE
	return file;
}

FILE* getOutputFile()
{
	FUNC_HI
	FILE* file = fopen("resTree", "w");
	if(file == NULL)
	{
		perror("resTree");
		exit(1);
	}
	FUNC_BYE
	return file;
}

FILE* getTexFile()
{
	FUNC_HI
	FILE* file = fopen("mathDump.tex", "w");
	if(file == NULL)
	{
		perror("mathDump.tex");
		exit(1);
	}
	FUNC_BYE
	return file;
}
