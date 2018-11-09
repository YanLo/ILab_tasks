#ifndef INCLUDE_LIST
#define INCLUDE_LIST

#include <cstdio>

class Node
{
	public:
		char*	word_;
		Node*	next_;

		Node(const char* input);
		~Node();
};

#endif //INCLUDE_LIST
