#include <cstdio>
#include <cstdlib>
#include <string.h>

#include "classNode.hpp"

//#define DEBUG
#include "myDefines.hpp"

enum 
{
	MAX_STRING_LEN = 128,
};

Node::Node(const char* value):
	word_	(NULL),
	next_	(NULL)
{
	FUNC_HI
	int len = strnlen(value, MAX_STRING_LEN);
	if(len == MAX_STRING_LEN)
	{
		printf("\n\tWrong data building node!\n");
		exit(1);
	}
	word_ = (char*) calloc(len + 1, sizeof(char));
	ALLOC(word_);
	memcpy(word_, value, len);
	word_[len] = '\0';
	FUNC_BYE
}

Node::~Node()
{
	FUNC_HI
	free(word_);
	word_ = NULL;
	next_ = NULL;
	FUNC_BYE
}
