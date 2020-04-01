#include <cstdio>
#include <cstdlib>
#include <assert.h>
#include <string.h>
#include <math.h>

#include "classHash.hpp"

//#define DEBUG
#include "myDefines.hpp"

#include "readInBufFromFile.hpp"

enum
{
	HASH_TABLE_SIZE = 257,
	MAX_WORD_SIZE	= 128,
};


Hash::Hash(FILE* source):
	buf_		(NULL),
	slider_		(0),
	data_buf_	(NULL)
{
	FUNC_HI
	buf_ = readInBufFromFile(source);
	data_buf_ = (Node**) calloc(HASH_TABLE_SIZE, sizeof(Node*));
	FUNC_BYE
}

Hash::~Hash()
{
	FUNC_HI
	free(buf_);
	buf_ = NULL;
	deleteLists();
	free(data_buf_);
	data_buf_ = NULL;
	FUNC_BYE
}

void Hash::deleteLists()
{
	FUNC_HI
	if(data_buf_)
	{
		int i = 0;
		for(i = 0; i < HASH_TABLE_SIZE; i++)
		{
			Node* node = data_buf_[i];
			Node* temp = node;
			while(node)
			{
				node = node->next_;
				delete(temp);
				temp = node;
			}
		}
	}
	FUNC_BYE
}

void Hash::makeTable(int (*hashfunc) (const char* string)) 
{
	FUNC_HI
	char word[MAX_WORD_SIZE] = {};
	int wordLen = 0;
	while(buf_[slider_] != '\0')
	{
		skipGaps();
		assert(MAX_WORD_SIZE == 128);
		sscanf(buf_ + slider_, "%127s%n", word, &wordLen);
		slider_ += wordLen;
		int index = (*hashfunc) (word);
		addNode(index, word);
		skipGaps();
		
	}
	FUNC_BYE
}

void Hash::skipGaps()
{
	FUNC_HI
	int gapLen = 0;
	sscanf(buf_ + slider_, "%*[ \n\t\r]%n", &gapLen);
	slider_ += gapLen;
	FUNC_BYE
}

void Hash::addNode(int index, const char* word)
{
	FUNC_HI
	Node* node = data_buf_[index];
	if(node)
	{
		while(node->next_)
		{
			node = node->next_;
		}
		node->next_ = new Node(word);
	}
	else
	{
		data_buf_[index] = new Node(word);
	}
	FUNC_BYE
}

void Hash::printPlot()
{
	FUNC_HI
	FILE* dataFile = fopen("data.txt", "w");
	if(!dataFile)
	{
		perror("data.txt");
		exit(1);
	}
	FILE* plotFile = fopen("plot_script", "w");
	if(!plotFile)
	{
		perror("plot_script");
		exit(1);
	}
	fprintf(plotFile, "set xrange[0:256]\n"
			"plot \"data.txt\" using 1:2 with lines");
	int x = 0;
	for(x = 0; x < HASH_TABLE_SIZE; x++)
	{
		int y = 0;
		Node* node = data_buf_[x];
		if(node)
		{
			while(node)
			{
				y++;
				node = node->next_;
			}
		}
		fprintf(dataFile, "%d\t%d\n", x, y);
	}
	fclose(dataFile);
	fclose(plotFile);
	system("gnuplot -persist plot_script"); 
	FUNC_BYE
}
