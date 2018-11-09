#include "classDerivative.hpp"
#include <math.h>
#include "getfiles.hpp"
#include "readInBufFromFile.hpp"
#include <assert.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

//#define DEBUG
#include "myDefines.hpp"

enum classDerivative
{
	MAX_STRING_LEN = 128,
};

Node* Derivative::headBuildFunc()
{
	FUNC_HI
	assert(tree_buf_);
	Node* node = addBuildFunc();
	if(tree_buf_[slider_] == '\0')
	{
		FUNC_BYE
		return node;
	}
	else
	{
		printf("\n\tWrong syntax\n");
		exit(1);
	}
}

#define BUILD_OP_NODE(OPERATION, KID_FUNC)				\
{														\
	node->parent_ = new Node(#OPERATION, TYPE_OPER);	\
	node->parent_->leftC_ = node;						\
	node->parent_->rghtC_ = KID_FUNC();					\
	node = node->parent_;								\
}

Node* Derivative::addBuildFunc()
{
	FUNC_HI
	Node* node = mulBuildFunc();
	while(tree_buf_[slider_] == '-' || tree_buf_[slider_] == '+')
	{
		slider_++;
		if(tree_buf_[slider_ - 1] == '-')
			BUILD_OP_NODE(-, mulBuildFunc)
		else
			BUILD_OP_NODE(+, mulBuildFunc)
	}
	FUNC_BYE
	return node;
}

Node* Derivative::mulBuildFunc()
{
	FUNC_HI
	Node* node = mathBuildFunc();
	while(tree_buf_[slider_] == '/' || tree_buf_[slider_] == '*')
	{
		slider_++;
		if(tree_buf_[slider_ - 1] == '/')
			BUILD_OP_NODE(/, mathBuildFunc)
		else
			BUILD_OP_NODE(*, mathBuildFunc)
	}
	FUNC_BYE
	return node;
}

Node* Derivative::mathBuildFunc()
{
	FUNC_HI
	Node* node = NULL;

	char string[MAX_STRING_LEN] = {};
	int opLen  = -1;
	int hookId = -1;
	sscanf(tree_buf_ + slider_, "%[^()+-/*^\n]%n", string,&opLen);
	sscanf(tree_buf_ + slider_ + opLen, "(%n", &hookId);
	if(hookId == -1)
	{
		node = powBuildFunc();
		FUNC_BYE
		return node;
	}
#define OPER_BUILD(oper)								\
{														\
	if(strncmp(#oper, string, MAX_STRING_LEN) == 0)		\
	{													\
		slider_ += opLen + hookId;						\
		node = addBuildFunc();							\
		node->parent_ = new Node(#oper, TYPE_OPER);		\
		node->parent_->leftC_ = node;					\
		node = node->parent_;							\
		if(tree_buf_[slider_] != ')')					\
		{												\
			printf("\n\tMissing closing hook\n");		\
			exit(1);									\
		}												\
		slider_++;										\
		return node;									\
	}													\
}			

#define INCLUDE_OPER_BUILD
#include "operBuildTree.hpp"
#undef INCLUDE_OPER_BUILD
#undef OPER_BUILD

	node = powBuildFunc();
	FUNC_BYE
	return node;
}

Node* Derivative::powBuildFunc()
{
	FUNC_HI
	Node* node = hookBuildFunc();
	while(tree_buf_[slider_] == '^')
	{
		slider_++;
		node->parent_ = new Node("^", TYPE_OPER);
		node->parent_->leftC_ = node;
		node->parent_->rghtC_ = hookBuildFunc();
		node = node->parent_;
	}
	FUNC_BYE
	return node;
}

Node* Derivative::hookBuildFunc()
{
	FUNC_HI
	Node* node = NULL;
	if(tree_buf_[slider_] == '(')
	{
		slider_++;
		node = addBuildFunc();
		if(tree_buf_[slider_] != ')')
		{
			printf("\n\tMissing closing hook\n");
			exit(1);
		}
		slider_++;
	}
	else
		node = workManBuildFunc();

	FUNC_BYE
	return node;
}

Node* Derivative::workManBuildFunc()
{
	FUNC_HI
	Node* node = NULL;
	int val = 0;
	if(tree_buf_[slider_] >= '0' && tree_buf_[slider_] <= '9')
	{
		while(tree_buf_[slider_] >= '0' 
									&& tree_buf_[slider_] <= '9')
		{
			val = val * 10 + tree_buf_[slider_] - '0';
			slider_++;
		}

		char value[MAX_STRING_LEN] = {};
		sprintf(value, "%d", val);
		node = new Node(value, TYPE_CONST);
		FUNC_BYE
		return node;
	}

	if(tree_buf_[slider_] == 'x')
	{
		node = new Node("x", TYPE_VAR);
		slider_++;
		FUNC_BYE
		return node;
	}
	if(tree_buf_[slider_] == 'e')
	{
		char value[MAX_STRING_LEN] = {};
		sprintf(value, "%lg", exp(1));
		node = new Node(value, TYPE_CONST);
		slider_++;
		FUNC_BYE
		return node;
	}
	FUNC_BYE
	return node;
}
