#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>
#include <string.h>

//#define DEBUG

#define WRONG_FORMAT_OF_INPUT_FILE                      \
		{												\
			printf("\nWrong format of input file\n");	\
			exit(1);									\
		} 
#include "myDefines.h"

typedef char data_t;

enum 
{
	MAX_FILE_NAME  = 32,
	MAX_STRING_LEN = 100,
};

//For future, rewrite all in .h and .hpp !!!

void dumpString(char* array, size_t len);
#include "dumpString.h"

FILE* getInputFile();
FILE* getOutputFile();
FILE* getInputFile(const char* fileName);
FILE* getOutputFile(const char* filename);
FILE* getDotFile();
char* readInBufFromFile(FILE* stream);
#include "readInBufFromFile.h"


class Node
{
	public:
		data_t*	buf_;
		Node*	leftC_;
		Node*	rghtC_;
		Node*	parent_;

		Node();
		Node(const data_t* value);
		Node(const data_t* value, Node* parent);
		Node(const data_t* value, Node* rghtC, Node* leftC);
		Node(const data_t* value, Node* rghtC, Node* leftC, Node* parent);
		~Node();
	private:
		
};

Node::Node(const data_t* value, Node* parent):
	buf_			(NULL),
	leftC_			(NULL),
	rghtC_			(NULL),
	parent_			(parent)
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
		buf_ = (data_t*) calloc(len + 1, sizeof(char));
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
	FUNC_BYE
}

void deleteTree(Node* node);
#include "functions.h"

class Akinator
{
	public:
		FILE*	inputFile_;
		FILE*	dotFile_;
		char*	tree_buf_;
		size_t 	slider_;
		FILE*	outputFile_;
		Node*	root_;
		///Where is private?!?!!?!?!?!?[2]
		
		Akinator();
		Akinator(const char* fileName);
		~Akinator();
		void	loadInFile();
		void	loadInFile(const char* fileName);
		void	writeTree(Node* node);
		Node*	buildTree(Node* parent);
		void	playAkinator(Node* node);
		void	dumpTree(Node* node);
		void	dump();
};

Akinator::Akinator():
	inputFile_	(NULL),
	dotFile_	(NULL),
	tree_buf_	(NULL),
	slider_		(0),
	outputFile_	(NULL),
	root_		(NULL)
{
	FUNC_HI
	inputFile_  = getInputFile();
	dotFile_	= getDotFile();
	tree_buf_	= readInBufFromFile(inputFile_);
	FUNC_BYE
}

Akinator::Akinator(const char* fileName):
	inputFile_	(NULL),
	dotFile_	(NULL),
	tree_buf_	(NULL),
	slider_		(NULL),
	outputFile_	(NULL),
	root_		(NULL)
{
	FUNC_HI
	inputFile_  = getInputFile(fileName);
	dotFile_	= getDotFile();
	tree_buf_	= readInBufFromFile(inputFile_);
	fclose(inputFile_);
	FUNC_BYE
}

Akinator::~Akinator()
{
	FUNC_HI
	free(tree_buf_);
	tree_buf_ = NULL;
	slider_	  = 0;
	fclose(outputFile_);
	root_	  = NULL; 	
	FUNC_BYE
}

Node* Akinator::buildTree(Node* parent)
{
	FUNC_HI
	assert(tree_buf_);
	data_t value[MAX_STRING_LEN] = {};
	
	int   argLen      = -1;

	if ((tree_buf_[slider_]     == '(') &&
	    (tree_buf_[slider_ + 1] == ')')    )
	{
		slider_ += 2;
		return NULL;
	}

	assert(MAX_STRING_LEN == 100);
	if(!sscanf(tree_buf_ + slider_, "(%99[^()]%n", &value, &argLen))
	{
		printf("\n\t__val__is__%s__\n", value);
		WRONG_FORMAT_OF_INPUT_FILE	
	}
	slider_ += argLen;
	Node*   node = new Node(value, parent);
	
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

void Akinator::loadInFile()
{
	FUNC_HI
	outputFile_ = getOutputFile();
	writeTree(root_);
	FUNC_BYE
}

void Akinator::loadInFile(const char* fileName)
{
	FUNC_HI
	outputFile_ = getOutputFile(fileName);
	writeTree(root_);
	FUNC_BYE
}

void Akinator::writeTree(Node* node)
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
	}
	FUNC_BYE
}

#ifdef DEBUG
void Akinator::dumpTree(Node* node)
{
	FUNC_HI
	if(node)
	{
		assert(dotFile_);
		fprintf(dotFile_, "TreeNode_%p [label=\""
						"TreeNode%p\\l"
						"data %s\\l"
						"parent %p\\l" 
						"leftC_ %p\\l"
						"rghtC_ %p\\l"
						"\"]\n",
						node, node, node->buf_, node->parent_, 
						node->leftC_, node->rghtC_);
		if(node->leftC_ && node->rghtC_)
		{
			dumpTree(node->leftC_);
			fprintf(dotFile_, "TreeNode_%p -> TreeNode_%p\n",
					node, node->leftC_);

			dumpTree(node->rghtC_);
			fprintf(dotFile_, "TreeNode_%p -> TreeNode_%p\n",
					node, node->rghtC_);
		}
	}
	FUNC_BYE
}
#else
void Akinator::dumpTree(Node* node)
{
	FUNC_HI
	if(node)
	{
		fprintf(dotFile_, "TreeNode_%p [label=\""
						"%s\\l"
						"\"]\n",
						node, node->buf_);
		if(node->leftC_ && node->rghtC_)
		{
			dumpTree(node->leftC_);
			fprintf(dotFile_, "TreeNode_%p -> TreeNode_%p\n",
					node, node->leftC_);

			dumpTree(node->rghtC_);
			fprintf(dotFile_, "TreeNode_%p -> TreeNode_%p\n",
					node, node->rghtC_);
		}
	}
	FUNC_BYE
}
#endif

void Akinator::dump()
{
	FUNC_HI
	fprintf(dotFile_, "digraph G{\ngraph [dpi = 300]\n");
	dumpTree(root_);
	fprintf(dotFile_, "}");
	fclose(dotFile_);
	system("dot dumpTree.gv -Tpng -o treeImage.png");
	system("xdot dumpTree.gv");
}

#include "playAkinator.h"

int main()
{
#ifndef DEBUG 
	char fileName[MAX_FILE_NAME] = {}; 
	printf("\nPlease, enter the name of file with data\n");
	assert(MAX_FILE_NAME == 32); 	
	scanf("%s", fileName);
	Akinator* akinator = new Akinator(fileName);
#else
	Akinator* akinator = new Akinator;
#endif //DEBUG

	akinator->root_ = akinator->buildTree(NULL);

	akinator->playAkinator(akinator->root_);
	akinator->dump();
	deleteTree(akinator->root_);

	delete akinator;
	return 0;
}
