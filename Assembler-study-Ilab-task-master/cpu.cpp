#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

//#define DEBUG

#include "myDefines.hpp"
#include "dumpString.hpp"
typedef int data_t;

#define GETFILE(name, mode)						\
FILE* get_##name##File()						\
{												\
	FILE* file = fopen(#name"File.txt", #mode);	\
	if (file == NULL)							\
		perror(#name);							\
	return file;								\
}

GETFILE(Bin, r)
GETFILE(Out, w)

const size_t STACK_SIZE = 1024;

enum
{
	RAM_SIZE = 1024,
	MAX_ARG_QT = 4
};

enum commands
{
#define CMD_NAME

#define CMD_DEF(name, argQt, cpu_instruction)				\
	CMD_##name,
#include "commands.hpp"
#undef CMD_DEF

#undef CMD_NAME
	CMD_DEFAULT
};

int getCmdArgQt(const char cmdNum)
{
#define CMD_NAME
#define CMD_DEF(name, arg_qt, cpu_instruction)	\
	case CMD_##name:							\
		return arg_qt;

	switch (cmdNum)
	{
#include "commands.hpp"
		default: return -1;
	}

#undef CMD_DEF
#undef CMD_NAME
}



class Cpu
{
	public:
		Cpu();
		~Cpu();
		FILE*	binfile_;
		FILE*	outfile_;
		size_t	bin_filesize_;
		char*	RAM_;
		size_t	IP_;
		size_t	CP_;
		data_t*	Registers_;
		void	Performer();
};

Cpu::Cpu()
{
	FUNC_HI
	binfile_ = 	get_BinFile();
	assert(binfile_);
	outfile_ =	get_OutFile();
	assert(outfile_);
	fseek(binfile_, 0, SEEK_END);
	bin_filesize_ = ftell(binfile_);
	rewind(binfile_);
	PRINTF("\n___binFileSize__is__%zu___\n", bin_filesize_);

	RAM_	 = (char*) calloc(RAM_SIZE, sizeof(char));
	ALLOC(RAM_); //defined in stack.cpp

	assert(RAM_SIZE >= bin_filesize_);
	fread(RAM_, sizeof(*RAM_), bin_filesize_, binfile_);
	IP_	= 0;
	CP_	= 0;
	Registers_ = (data_t*) calloc(16, sizeof(data_t));
	ALLOC(Registers_);
	FUNC_BYE
}

Cpu::~Cpu()
{
	FUNC_HI
	binfile_	= NULL;
	IP_			= 0;
	CP_		 	= 0;
	bin_filesize_	= 0;
	free(RAM_);
	free(Registers_);
	FUNC_BYE
}

void Cpu::Performer()
{
	FUNC_HI
	data_t arguments[MAX_ARG_QT] = {};
	size_t END = bin_filesize_;

	while(CP_ < END)
	{
		CP_ = IP_;
		char cmd = RAM_[IP_];
		PRINTF("\n\t____Command____%d\n", cmd);
		CP_ += sizeof(char);
		int argQt = getCmdArgQt(cmd);
		int i = 0;
		for (i = 0; i < argQt; i++)
		{
			arguments[i] = *(data_t*)(&RAM_[CP_]);
			PRINTF("\n\t____Argument___%d\n", arguments[i]);
			CP_ += sizeof(data_t);
		}
		IP_ = CP_;


#define CMD_DEF(name, argQt, code)		\
		case CMD_##name:				\
		{								\
			{code}						\
			PRINTF("\nI have performed command\n");	\
			break;						\
		}
		switch(cmd)
		{
#define CMD_NAME
#include "commands.hpp"
#undef CMD_NAME
#undef CMD_DEF
		}
	}
	FUNC_BYE
}

int main()
{
	Cpu cpu;
	PRINTF("\n\tDump of an binFile\n");
#ifdef DEBUG
	dumpString(cpu.RAM_, cpu.bin_filesize_);
#endif //DEBUG
	cpu.Performer();
	return 0;
}

