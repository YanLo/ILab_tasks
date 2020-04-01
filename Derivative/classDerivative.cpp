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


#define WRONG_FORMAT_OF_INPUT_FILE                      \
		{												\
			printf("\nWrong format of input file\n");	\
			exit(1);									\
		} 

enum classDerivative
{
	MAX_STRING_LEN = 128,
};

bool equalDouble(double a, double b);
bool isZero(double a);

bool equalDouble(double a, double b)
{
	return abs(a - b) < 0.001;
}

bool isZero(double a)
{
	return abs(a) < 0.001;
}

Derivative::Derivative():
	inputFile_	(NULL),
	dotFile_	(NULL),
	tree_buf_	(NULL),
	slider_		(0),
	outputFile_	(NULL),
	texFile_	(NULL),
	root_		(NULL),
	droot_		(NULL)
{
	FUNC_HI
	inputFile_  = getInputFile();
	tree_buf_	= readInBufFromFile(inputFile_);
	FUNC_BYE
}

Derivative::Derivative(const char* fileName):
	inputFile_	(NULL),
	dotFile_	(NULL),
	tree_buf_	(NULL),
	slider_		(0),
	outputFile_	(NULL),
	texFile_	(NULL),
	root_		(NULL),
	droot_		(NULL)
{
	FUNC_HI
	inputFile_  = getInputFile(fileName);
	dotFile_	= getDotFile();
	tree_buf_	= readInBufFromFile(inputFile_);
	fclose(inputFile_);
	FUNC_BYE
}

Derivative::~Derivative()
{
	FUNC_HI
	free(tree_buf_);
	tree_buf_ = NULL;
	slider_	  = 0;
	root_	  = NULL; 
	droot_	  = NULL;
	FUNC_BYE
}

int Derivative::typeDefiner(const char* value)
{
	FUNC_HI
	assert(value);
	if(isdigit(*value))
		return TYPE_CONST;
	else
	{
#define OP_DEF(oper, instruction, derivate, dumptex)		\
		if(strncmp(#oper, value, MAX_STRING_LEN) == 0)		\
			return TYPE_OPER;						
#define INCLUDE_OP
#include "operations.hpp"
#undef	INCLUDE_OP
#undef	OP_DEF
	}
	return TYPE_VAR;
	FUNC_BYE
}

Node* Derivative::computer(Node* node)
{
	FUNC_HI
	if(node)
	{
		if(node->visit_count_qt_ >= 2) //one after 
									//simplification of children
		{
			FUNC_BYE
			return node;
		}
		node->visit_count_qt_++;
		//dump(droot_);
		switch(node->type_)
		{
			case TYPE_CONST:
				{
					FUNC_BYE
					return node;
				}
			case TYPE_OPER:
				{
#define OP_DEF(oper, instruction, derivate, dumptex)		\
		if(strncmp(#oper, node->buf_, MAX_STRING_LEN) == 0)	\
				{instruction}
#define INCLUDE_OP
#include "operations.hpp"
#undef	INCLUDE_OP
#undef	OP_DEF
				}
			case TYPE_VAR:
				{
					FUNC_BYE
					return node;
				}
			default:
				{}
		}
	} 
	FUNC_BYE
	return NULL;
}

Node* Derivative::buildTree(Node* parent)
{
	FUNC_HI
	assert(tree_buf_);
	char value[MAX_STRING_LEN] = {};
	
	int   argLen      = -1;

	if ((tree_buf_[slider_]     == '(') &&
	    (tree_buf_[slider_ + 1] == ')')    )
	{
		slider_ += 2;
		return NULL;
	}

	assert(MAX_STRING_LEN == 128);
	if(!sscanf(tree_buf_ + slider_, "(%127[^()]%n",
												value, &argLen))
	{
		printf("\n\t__val__is__%s__\n", value);
		WRONG_FORMAT_OF_INPUT_FILE	
	}
	slider_ += argLen;
	int type = typeDefiner(value);
	Node* node = NULL;
	if(strncmp("e", value, MAX_STRING_LEN) == 0)
	{
		char exponent[MAX_STRING_LEN] = {};
		sprintf(exponent, "%lg", exp(1));
		node = new Node(exponent, parent, TYPE_CONST);
	}
	else
		node = new Node(value, parent, type);
	
	node->leftC_ = buildTree(node);
	node->rghtC_ = buildTree(node); 
	if (tree_buf_[slider_] != ')')
	{
		WRONG_FORMAT_OF_INPUT_FILE
	}
	slider_++;

	FUNC_BYE
	return node;
}

void Derivative::deleteTree(Node* node)
{
	FUNC_HI
	if(node)
	{
		if(node->leftC_)
			deleteTree(node->leftC_);
		if(node->rghtC_)
			deleteTree(node->rghtC_);
		delete(node);
	}
	else
		return;
	FUNC_BYE
}

void Derivative::loadInFile(Node* node)
{
	FUNC_HI
	outputFile_ = getOutputFile();
	writeTree(node);
	fclose(outputFile_);
	FUNC_BYE
}

void Derivative::loadInFile(const char* fileName, Node* node)
{
	FUNC_HI
	outputFile_ = getOutputFile(fileName);
	writeTree(node);
	fclose(outputFile_);
	FUNC_BYE
}

void Derivative::writeTree(Node* node)
{
	FUNC_HI
	if(node)
	{
		assert(outputFile_);
		fprintf(outputFile_, "(%s", node->buf_);
		if(node->leftC_ && node->rghtC_)
		{
			writeTree(node->leftC_);
			writeTree(node->rghtC_);
			fprintf(outputFile_, ")");
		}
		if(!node->leftC_ && !node->rghtC_)
		{
			fprintf(outputFile_, "()())");	
		}
		if(node->leftC_ && !node->rghtC_)
		{
			writeTree(node->leftC_);
			fprintf(outputFile_, "())");
		}
		if(!node->leftC_ && node->rghtC_)
		{
			fprintf(outputFile_, "()");
			writeTree(node->rghtC_);
			fprintf(outputFile_, ")");
		}
	}
	FUNC_BYE
}

#ifdef DEBUG
void Derivative::dumpTree(Node* node)
{
	//FUNC_HI
	if(node)
	{
		assert(dotFile_);
		fprintf(dotFile_, "TreeNode_%p [label=\""
						"TreeNode%p\\l"
						"type %d\\l"
						"data %s\\l"
						"parent %p\\l" 
						"leftC_ %p\\l"
						"rghtC_ %p\\l"
						"\"]\n",
						node, node, node->type_, node->buf_,
						node->parent_, 
						node->leftC_, node->rghtC_);
		if(node->leftC_)
		{
			dumpTree(node->leftC_);
			fprintf(dotFile_, "TreeNode_%p -> TreeNode_%p\n",
					node, node->leftC_);
		}
		if(node->rghtC_)
		{
			dumpTree(node->rghtC_);
			fprintf(dotFile_, "TreeNode_%p -> TreeNode_%p\n",
					node, node->rghtC_);
		}
	}
	//FUNC_BYE
}
#else
void Derivative::dumpTree(Node* node)
{
	FUNC_HI
	if(node)
	{
		fprintf(dotFile_, "TreeNode_%p [label=\""
						"%s\\l"
						"\"]\n",
						node, node->buf_);
		if(node->leftC_) 
		{
			dumpTree(node->leftC_);
			fprintf(dotFile_, "TreeNode_%p -> TreeNode_%p\n",
					node, node->leftC_);
		}
		if(node->rghtC_)
		{
			dumpTree(node->rghtC_);
			fprintf(dotFile_, "TreeNode_%p -> TreeNode_%p\n",
					node, node->rghtC_);
		}
	}
	FUNC_BYE
}
#endif

void Derivative::dump(Node* root)
{
	FUNC_HI
	dotFile_ = getDotFile();
	fprintf(dotFile_, "digraph G{\ngraph [dpi = 300]\n");
	dumpTree(root);
	fprintf(dotFile_, "}");
	fclose(dotFile_);
	system("dot dumpTree.gv -Tpng -o treeImage.png");
	system("xdot dumpTree.gv");
	FUNC_BYE
}

void Derivative::checkTwoArgsNotNULL(Node* node)
{
	FUNC_HI
	if(!node->leftC_ || !node->rghtC_)
	{
		printf("\n\tWrong format of a tree! Must be two args\n");
		exit(1);
	}
	FUNC_BYE
}

void Derivative::checkOnlyOneArg(Node* node)
{
	FUNC_HI
	if((!node->leftC_ && !node->rghtC_) ||
	   (node->leftC_ && node->rghtC_))
	{
		printf("\nWrong format of a tree! Must be one arg\n");
		exit(1);
	}
	FUNC_BYE
}

bool Derivative::TwoArgsAreConst(Node* node)
{
	FUNC_HI
	if(node->leftC_ && node->rghtC_)
		if((node->rghtC_->type_ == TYPE_CONST) &&
		   (node->leftC_->type_ == TYPE_CONST))
		{
			PRINTF("\n\tTrue\n");
			FUNC_BYE
			return true;
		}
	PRINTF("\n\tFalse\n");
	FUNC_BYE
	return false;
}

Node* Derivative::curNodeReplacedByRght(Node* node)
{
	FUNC_HI
	delete node->leftC_;
	if(node->parent_)
	{
		if(node == node->parent_->leftC_)
		{
			node->parent_->leftC_ = node->rghtC_;
			node->rghtC_->parent_ = node->parent_;
		}
		if(node == node->parent_->rghtC_)
		{
			node->parent_->rghtC_ = node->rghtC_;
			node->rghtC_->parent_ = node->parent_;
		}
		Node* rght = node->rghtC_;
		delete node;
		FUNC_BYE
		return rght;
	}
	else //root
	{
		Node* rght = node->rghtC_;
		if(node == root_)
		{
			delete node;
			root_  = rght;
		}
		else //node == droot_
		{
			delete node;
			droot_ = rght;
		}
		FUNC_BYE
		return rght;
	}
}

Node* Derivative::curNodeReplacedByLeft(Node* node)
{
	FUNC_HI
	delete node->rghtC_;
	if(node->parent_)
	{
		if(node == node->parent_->leftC_)
		{
			node->parent_->leftC_ = node->leftC_;
			node->leftC_->parent_ = node->parent_;
		}
		if(node == node->parent_->rghtC_)
		{
			node->parent_->rghtC_ = node->leftC_;
			node->leftC_->parent_ = node->parent_;
		}
		Node* temp = node;
		Node* left = node->leftC_;
		delete node;
		if(temp == root_)
			root_  = left;
		if(temp == droot_)
			droot_ = left;
		FUNC_BYE
		return left;
	}
	else //root_
	{
		Node* left = node->leftC_;
		if(node == root_)
		{
			delete node;
			root_ = left;
		}
		else // node == droot_
		{
			delete node;
			droot_ = left;
		}
		FUNC_BYE
		return left;
	}
}

Node* Derivative::addSimplifications(Node* node)
{
	FUNC_HI
	if(strncmp(node->buf_, "+", MAX_STRING_LEN) == 0)
	{
		if(node->leftC_->type_ == TYPE_CONST)
		{
			double lval = 0;
			sscanf(node->leftC_->buf_, "%16lg", &lval);
			if(isZero(lval))
			{
				PRINTF("\n\tDone\n");
				FUNC_BYE
				return curNodeReplacedByRght(node);
			}
		}
	}

	if(node->rghtC_->type_ == TYPE_CONST)
	{
		double rval  = 0;
		sscanf(node->rghtC_->buf_, "%16lg", &rval);
		if(isZero(rval))
		{
			PRINTF("\n\tDone\n");
			FUNC_BYE
			return curNodeReplacedByLeft(node);		
		}
	}
	PRINTF("\n\tNothing to simplify\n");
	FUNC_BYE
	return node;
}

Node* Derivative::mulSimplifications(Node* node)
{
	FUNC_HI
	if(node->leftC_->type_ == TYPE_CONST)
	{
		double lval = 0;
		sscanf(node->leftC_->buf_, "%16lg", &lval);
		if(isZero(lval))
		{
			delete node->leftC_;
			node->leftC_ = NULL;
			delete node->rghtC_;
			node->rghtC_ = NULL;
			sprintf(node->buf_, "%lg", 0.000);
			node->type_ = TYPE_CONST;
			PRINTF("\n\tDone\n");
			FUNC_BYE
			return node;
		}
		if(equalDouble(lval, 1.000))
		{
			PRINTF("\n\tDone\n");
			FUNC_BYE
			return curNodeReplacedByRght(node);
		}
	}

	if(node->rghtC_->type_ == TYPE_CONST)
	{
		double rval  = 0;
		sscanf(node->rghtC_->buf_, "%16lg", &rval);
		if(isZero(rval))
		{
			delete node->leftC_;
			node->leftC_ = NULL;
			delete node->rghtC_;
			node->rghtC_ = NULL;
			sprintf(node->buf_, "%lg", 0.000);
			node->type_ = TYPE_CONST;
			PRINTF("\n\tDone\n");
			FUNC_BYE
			return node;
		}
		if(equalDouble(rval, 1.000))
		{
			PRINTF("\n\tDone\n");
			FUNC_BYE
			return curNodeReplacedByLeft(node);
		}
	}
	PRINTF("\n\tNothing to simplify\n");
	FUNC_BYE
	return node;
}

Node* Derivative::divSimplifications(Node* node)
{
	FUNC_HI
	if(node->leftC_->type_ == TYPE_CONST)
	{
		double lval = 0;
		sscanf(node->leftC_->buf_, "%16lg", &lval);
		if(isZero(lval))
		{
			delete node->leftC_;
			node->leftC_ = NULL;
			delete node->rghtC_;
			node->rghtC_ = NULL;
			sprintf(node->buf_, "%lg", 0.000);
			node->type_ = TYPE_CONST;
			PRINTF("\n\tDone\n");
			FUNC_BYE
			return node;
		}
	}

	if(node->rghtC_->type_ == TYPE_CONST)
	{
		double rval  = 0;
		sscanf(node->rghtC_->buf_, "%16lg", &rval);
		if(isZero(rval))
		{
			printf("\n\tDividing on zero!!!\n");	
			PRINTF("\n\tDone\n");
			FUNC_BYE
			return node;
		}
		if(equalDouble(rval, 1.000))
		{
			PRINTF("\n\tDone\n");
			FUNC_BYE
			return curNodeReplacedByLeft(node);
		}
	}

	FUNC_BYE
	PRINTF("\n\tNothing to simplify!\n");
	return node;
}

Node* Derivative::powSimplifications(Node* node)
{
	FUNC_HI
	if(node->leftC_->type_ == TYPE_CONST)
	{
		double lval = 0;
		sscanf(node->leftC_->buf_, "%16lg", &lval);
		if(isZero(lval))
		{
			delete node->leftC_;
			node->leftC_ = NULL;
			delete node->rghtC_;
			node->rghtC_ = NULL;
			sprintf(node->buf_, "%lg", 0.000);
			node->type_ = TYPE_CONST;
			PRINTF("\n\tDone\n");
			FUNC_BYE
			return node;
		}
		if(equalDouble(lval, 1.000))
		{
			delete node->leftC_;
			node->leftC_ = NULL;
			delete node->rghtC_;
			node->rghtC_ = NULL;
			sprintf(node->buf_, "%4lg", 1.000);
			node->type_ = TYPE_CONST;
			PRINTF("\n\tDone1\n");
			FUNC_BYE
			return node;
		}
	}

	if(node->rghtC_->type_ == TYPE_CONST)
	{
		double rval  = 0;
		sscanf(node->rghtC_->buf_, "%16lg", &rval);
		if(isZero(rval))
		{
			delete node->leftC_;
			node->leftC_ = NULL;
			delete node->rghtC_;
			node->rghtC_ = NULL;
			sprintf(node->buf_, "%lg", 1.000);
			node->type_ = TYPE_CONST;
			PRINTF("\n\tDone\n");
			FUNC_BYE
			return node;
		}
		if(equalDouble(rval, 1.000))
		{
			PRINTF("\n\tDone\n");
			FUNC_BYE
			return curNodeReplacedByLeft(node);
		}
	}
	PRINTF("\n\tNothing to simplify\n");
	FUNC_BYE
	return node;
}

void Derivative::performPow(Node* node)
{
	FUNC_HI
	double lval = 0;
	sscanf(node->leftC_->buf_, "%16lg", &lval);
	double rval = 0;
	sscanf(node->rghtC_->buf_, "%16lg", &rval);
	delete node->leftC_;
	node->leftC_ = NULL;
	delete node->rghtC_;
	node->rghtC_ = NULL;
	double res = pow(lval, rval);
	sprintf(node->buf_, "%lg", res);
	node->type_ = TYPE_CONST;
	FUNC_BYE
}

#define RETURN_IF_CHILD_IS_VAR				\
if(child->type_ == TYPE_VAR)				\
	{										\
		FUNC_BYE							\
		return child->parent_;				\
	}					


#define PERFORM_ONE_ARG_OP(operation)			\
{												\
	double res = operation(val);				\
	sprintf(child->parent_->buf_, "%lg", res);	\
	child->parent_->type_ = TYPE_CONST;			\
	Node* parent = child->parent_;				\
	delete child;								\
	if(child == parent->leftC_)					\
		parent->leftC_ = NULL;					\
	if(child == parent->rghtC_)					\
		parent->rghtC_ = NULL;					\
	FUNC_BYE									\
	return parent;								\
}



Node* Derivative::performLog(Node* child)
{
	FUNC_HI

	RETURN_IF_CHILD_IS_VAR
		
	if(child->type_ == TYPE_CONST)
	{
		double val = 0;
		sscanf(child->buf_, "%16lg", &val);
		if(val <= 0)
		{
			printf("\n\tWrong field of accepted values(log)\n");
			FUNC_BYE
			return child->parent_;
		}
		PERFORM_ONE_ARG_OP(log)
	}
	child = computer(child);
	FUNC_BYE
	return computer(child->parent_);
}

#define PERFORM_TRIG_FUNC(trig_func)					\
Node* Derivative::perform##trig_func(Node* child)		\
{														\
	FUNC_HI												\
														\
	RETURN_IF_CHILD_IS_VAR								\
														\
	if(child->type_ == TYPE_CONST)						\
	{													\
		double val = 0;									\
		sscanf(child->buf_, "%16lg", &val);				\
		PERFORM_ONE_ARG_OP(trig_func)					\
	}													\
	child = computer(child);							\
	FUNC_BYE											\
	return computer(child->parent_);					\
}


PERFORM_TRIG_FUNC(sin)
PERFORM_TRIG_FUNC(cos)

Node* Derivative::performTan(Node* child)
{
	FUNC_HI
	if(child->type_ == TYPE_VAR)
	{
		FUNC_BYE
		return child->parent_;
	}
	if(child->type_ == TYPE_CONST)
	{
		double val = 0;
		sscanf(child->buf_, "%16lg", &val);
		if(equalDouble(val, M_PI))
		{
			printf("\n\tWrong field of accepted values(tan)\n");
			return child->parent_;
		}
		PERFORM_ONE_ARG_OP(tan)
	}
	child = computer(child);
	FUNC_BYE
	return computer(child->parent_);
}

Node* Derivative::derivator(Node* node, Node* dnodeParent)
{
	FUNC_HI
	if(node)
	{
		//dump(node);
		switch(node->type_)
		{
			case TYPE_CONST:
				{
					Node* dnode = new Node("0", dnodeParent, 
													TYPE_CONST);
					FUNC_BYE
					return dnode;
				}
			case TYPE_VAR:
				{
					Node* dnode = new Node("1", dnodeParent, 
													TYPE_CONST);
					FUNC_BYE
					return dnode;
				}
			case TYPE_OPER:
				{
#define OP_DEF(oper, instruction, derivate, dumptex)			\
			if(strncmp(#oper, node->buf_, MAX_STRING_LEN) == 0)	\
					{derivate}
#define INCLUDE_OP
#include "operations.hpp"
#undef INCLUDE_OP
#undef OP_DEF
				}
			default:
				{}
		}
	}
	FUNC_BYE
	return NULL;
}

Node* Derivative::copyTree(Node* node, Node* cnodeParent)
{
	FUNC_HI
	if(node)
	{
	Node* cnode = new Node(node->buf_, cnodeParent, node->type_);
	cnode->leftC_ = copyTree(node->leftC_, cnode);
	cnode->rghtC_ = copyTree(node->rghtC_, cnode);
	FUNC_BYE
	return cnode;
	}
		FUNC_BYE
		return NULL;
}

void Derivative::mathDump()
{
	FUNC_HI
	texFile_ = getTexFile();
	fprintf(texFile_, "\\documentclass{article}\n"
					  "\\title{LaTeX\ Derivative}\n"
					  "\\author{YanLo}\n"
					  "\\begin{document}\n\n"
					  "\\maketitle\n\n"
					  "\\fontsize{14}{10pt}\\selectfont\n"
					  "\\begin{equation}\n"
					  "\\left[");
					  
	dumpTex(root_);
	fprintf(texFile_, "\\right]' = ");
	dumpTex(droot_);
	fprintf(texFile_, "\n\\end{equation}\n\n\\end{document}");
	fclose(texFile_);
	system("pdflatex mathDump.tex");
	system("evince mathDump.pdf");
	FUNC_BYE
}

void Derivative::dumpTex(Node* node)
{
	FUNC_HI
	if(node)
	{
		switch (node->type_)
		{
			case TYPE_CONST: 
			case TYPE_VAR:
				{
					char exponent[MAX_STRING_LEN] = {};
					sprintf(exponent, "%lg", exp(1));
					if(strncmp(exponent, node->buf_, 
								MAX_STRING_LEN) == 0)
						fprintf(texFile_, "{e}");
					else
						fprintf(texFile_, "{%s}", node->buf_);
					FUNC_BYE
					return;
				}
			case TYPE_OPER:
				{
#define OP_DEF(oper, instruction, derivate, dumptex)			\
			if(strncmp(#oper, node->buf_, MAX_STRING_LEN) == 0)	\
					{dumptex}
#define INCLUDE_OP
#include "operations.hpp"
#undef INCLUDE_OP
#undef OP_DEF
					FUNC_BYE
					return;
				}
			default:
				{}
		}
	}
	else
	{
		FUNC_BYE
		return;
	}
}
