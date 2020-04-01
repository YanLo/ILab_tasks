#ifndef ASM_INCLUDED
#define ASM_INCLUDED

#include <stdio.h>

enum
{	
	MAX_LABEL_QT = 10,
	MAX_STR_SIZE = 32,
	MAX_RAM_SIZE = 1024,
};

class Assembler
{
	public:
		Assembler();
		~Assembler();
		FILE*	asmfile_;
		FILE*	binfile_;
		char*	asm_buf_;
		char*	temp_buf_;
		char*	bin_buf_;
		size_t	asm_filesize_;
		size_t	bin_filesize_;
		size_t	cur_asm_line_;
		size_t	cur_cmd_num_;
		int		labels_[MAX_LABEL_QT];

		char	getCmdCode(const char* cptr);
		void	skipGaps(char** bufp);
		void	putCmdCodeInTempBuf(char** bufp);
		void	putCmdArgsInTempBuf(char** bufp);
		int		getLabelNumFromBuf(char** bufp);
		int		getCmdArgQt(const char CmdNum);
		char*	readInBuf();
		void	deleteComments();
		void	deleteLabelsInBuf();
		void	writeAsmFile();
		void	passingLabels();
		void	parseCommandsInBinFile();
};

#endif //ASM_INCLUDED
