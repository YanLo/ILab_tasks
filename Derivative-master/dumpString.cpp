#include "dumpString.hpp"
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>

void dumpString(char* array, size_t len)
{
	printf("\n\n\t__________Hi_from_dumpString______________\n");
	assert(array);
	size_t i = 0;

	printf("\n\t___________I'm dumping char array_%p_\n\n", array);
	for (i = 0; i < len; i++)
	{
		printf("'%c'  ", isprint(array[i]) ? array[i] : ' ');
	}
	printf("\n");
	for (i = 0; i < len; i++)
	{
		printf("0x%02x|", (unsigned int)array[i]);
	}
	printf("\n\n\t___________Bye_____________________________\n");
}
