#ifndef COMPUTER
#define COMPUTER

#include <stdio.h>

class Computer
{
	public:
		FILE*	expFile_;
		size_t	slider_;
		char*	buf_;

		Computer();
		~Computer();

		int		workManFunc();
		int		hookFunc();
		int		powFunc();
		int		mulFunc();
		int		addFunc();
		int		headFunc();
};

#endif //COMPUTER
