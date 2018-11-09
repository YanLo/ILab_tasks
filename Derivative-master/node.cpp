#include "node.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//#define DEBUG

#include "myDefines.hpp"

enum node_enum 
{
	MAX_STRING_LEN = 128,
};

Node::Node(const char* value, int type):
	buf_			(NULL),
	leftC_			(NULL),
	rghtC_			(NULL),
	parent_			(NULL),
	type_			(type),
	visit_count_qt_	(0)
{
	FUNC_HI
	if(value)
	{
		size_t len = strnlen(value, MAX_STRING_LEN);
		if (len == MAX_STRING_LEN)
		{
			printf("\n\tWrong data building node\n");
			exit(1);
		}
		buf_ = (char*) calloc(len + 1, sizeof(char));
		ALLOC(buf_);
		memcpy(buf_, value, len);
		buf_[len] = '\0';
	}
	FUNC_BYE
}

Node::Node(const char* value, Node* parent, int type):
	buf_			(NULL),
	leftC_			(NULL),
	rghtC_			(NULL),
	parent_			(parent),
	type_			(type),
	visit_count_qt_	(0)
{
	FUNC_HI
	if(value)
	{
		size_t len = strnlen(value, MAX_STRING_LEN);
		if (len == MAX_STRING_LEN)
		{
			printf("\n\tWrong data building node\n");
			exit(1);
		}
		buf_ = (char*) calloc(len + 1, sizeof(char));
		ALLOC(buf_);
		memcpy(buf_, value, len);
		buf_[len] = '\0';
	}
	FUNC_BYE
}

Node::~Node()
{
	FUNC_HI
	free(buf_);
	buf_ 	= NULL;
	leftC_	= NULL;
	rghtC_ 	= NULL;
	parent_	= NULL;
	type_	= TYPE_DEFAULT;
	FUNC_BYE
}
