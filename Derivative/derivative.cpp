#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>
#include <string.h>

#define DEBUG

typedef char data_t;

enum 
{
	MAX_FILE_NAME  = 32,
};

#include "classDerivative.hpp"

int main()
{
#ifndef DEBUG 
	char fileName[MAX_FILE_NAME] = {}; 
	printf("\nPlease, enter the name of file with data\n");
	assert(MAX_FILE_NAME == 32); 	
	scanf("%s", fileName);
	Derivative* derivative = new Derivative(fileName);
#else
	Derivative* derivative = new Derivative;
#endif //DEBUG

	//derivative->root_ = derivative->buildTree(NULL);
	derivative->root_ = derivative->headBuildFunc();
	derivative->root_ = derivative->computer(derivative->root_);

	derivative->dump(derivative->root_);

	derivative->droot_ = derivative->derivator(derivative->root_,
															NULL);
	derivative->dump(derivative->droot_);
//	derivative->droot_ = derivative->computer(derivative->droot_);
//	derivative->dump(derivative->droot_);

	derivative->mathDump();
	derivative->deleteTree(derivative->droot_);
	derivative->deleteTree(derivative->root_);

	delete derivative;
	return 0;
}
