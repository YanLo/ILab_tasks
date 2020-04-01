#ifndef INCLUDE_HASH
#define INCLUDE_HASH

#include <cstdio>
#include "classNode.hpp"

class Hash
{
	public:
		char*	buf_;
		size_t	slider_;
		Node**	data_buf_;
		
		Hash(FILE* source);
		~Hash();
		void	deleteLists();

		void	makeTable(int (*hashfunc)(const char* string));
		void	skipGaps();
		void	addNode(int index, const char* word);

		void	printPlot();
};

#endif //INCLUDE_HASH
