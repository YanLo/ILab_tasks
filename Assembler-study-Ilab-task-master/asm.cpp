#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include <cstddef>

#include "classAssembler.hpp"

typedef int data_t;

//#define DEBUG

#include "myDefines.hpp"

enum Commands
{
#define CMD_NAME

#define CMD_DEF(name, arg_qt, cpu_instruction)				\
	CMD_##name,
#include "commands.hpp"
#undef CMD_DEF

#undef CMD_NAME
	CMD_DEFAULT
};


#include "dumpString.hpp"

#define GETFILE(name, mode)						\
FILE* get##name##File()							\
{												\
	FILE* file = fopen(#name"File.txt", #mode);	\
	if (file == NULL)							\
		perror(#name);							\
	return file;								\
}

GETFILE(Asm, r)
GETFILE(Bin, w)

Assembler::Assembler():
	asmfile_		(NULL),
	binfile_		(NULL),
	asm_buf_		(NULL),
	temp_buf_		(NULL),
	bin_buf_		(NULL),
	asm_filesize_	(0),
	bin_filesize_	(0),
	cur_asm_line_	(0),
	cur_cmd_num_	(0)
{
	asmfile_ = getAsmFile();
	binfile_ = getBinFile();
	asm_buf_ = readInBuf(); //asm_filesize initialised there
	temp_buf_ = (char*) calloc(asm_filesize_ * sizeof(data_t),
								sizeof(char));

	for (int i = 0; i < MAX_LABEL_QT; i++)
		labels_[i] = -1;
}

Assembler::~Assembler()
{
	fclose(asmfile_);
	fclose(binfile_);
	free(asm_buf_);
	free(temp_buf_);
	free(bin_buf_);
	asm_filesize_ = 0;
	bin_filesize_ = 0;
	cur_asm_line_ = 0;
	cur_cmd_num_  = 0;
}

char Assembler::getCmdCode(const char* cptr)
{
	#define CMD_DEF(name, arg_qt, cpu_instruction)	\
	if (!strcmp(#name, cptr))						\
		return	CMD_##name;
	#define CMD_NAME
	#include "commands.hpp"
	#undef CMD_NAME
	#undef CMD_DEF
	printf("At line %zu: command %s was not declared\n",
											cur_asm_line_, cptr);
	return CMD_DEFAULT;
}

int Assembler::getCmdArgQt(const char cmdNum)
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

char* Assembler::readInBuf()
{
	FUNC_HI
	if (!asmfile_)
	{
		printf("asmfile_ hasn't been opened\n");
		return NULL;
	}
	fseek(asmfile_, 0, SEEK_END);
	asm_filesize_ = ftell(asmfile_);
#ifdef USE_FSEEK_READINBUF
	fseek(asmfile_, 0, SEEK_SET);
#else
	rewind(asmfile_);
#endif

	asm_buf_ = (char*) calloc(asm_filesize_ + 1, sizeof(char));
	ALLOC(asm_buf_)
	fread(asm_buf_, sizeof(char), asm_filesize_, asmfile_);

	asm_buf_[asm_filesize_] = '\n';
#ifdef DEBUG
	dumpString(asm_buf_, asm_filesize_);
#endif //DEBUG
	PRINTF("\n\tAsmFileSize___is_%zu\n", asm_filesize_);
	FUNC_BYE
	return asm_buf_;
}

void Assembler::deleteComments()
{
	FUNC_HI
	assert(asm_buf_);
	char* semiColonPtr = NULL;
	char* newLinePos = NULL;
	while (semiColonPtr = strchr(asm_buf_, ';'))
	{
		newLinePos = strchr(semiColonPtr, '\n');
		for (ptrdiff_t j = 0; j < newLinePos - semiColonPtr; j++)
		{
			semiColonPtr[j]  = ' ';
		}
	}
	FUNC_BYE
}

void Assembler::deleteLabelsInBuf()
{
	FUNC_HI
	assert(asm_buf_);
	char* semiColonPtr = NULL;
	char* newLinePos = NULL;
	while (semiColonPtr = strchr(asm_buf_, ':'))
	{
		newLinePos = strchr(semiColonPtr, '\n');
		for (ptrdiff_t j = 0; j < newLinePos - semiColonPtr; j++)
		{
			semiColonPtr[j]  = ' ';
		}
	}
	FUNC_BYE
}

void Assembler::skipGaps(char** bufp)
{
	FUNC_HI
	assert(bufp);
	assert(*bufp);
	int gapLen = 0;
	sscanf(*bufp, "%*[ \n\t\r]%n", &gapLen);
	*bufp += gapLen;
	FUNC_BYE
}


void Assembler::putCmdCodeInTempBuf(char** bufp)
{
	FUNC_HI
	assert(bufp);
	assert(*bufp);
	int cmdLen = -1;
	char cmdCode = CMD_DEFAULT; //IMPROVE//Coomands instead of char

	char cmdName[MAX_STR_SIZE] = {};
	assert(MAX_STR_SIZE == 32);
	sscanf(*bufp, "%32[A-Za-z]%n", cmdName, &cmdLen);

	PRINTF("\n\t____COMMAND___%s___\n", cmdName);
	*bufp += cmdLen;
	cur_cmd_num_++;

	cmdCode = getCmdCode(cmdName);
	if(cmdCode == CMD_DEFAULT)
	{
		printf("\nAt line %zu: wrong command\n", cur_asm_line_);
		exit(1);
	}
	temp_buf_[bin_filesize_++] = cmdCode;
	FUNC_BYE
}

void Assembler::putCmdArgsInTempBuf(char** bufp)
{
	FUNC_HI
	assert(bufp);
	assert(*bufp);
	int j, k;
	j = k = 0;
	int argLen = 0;
	data_t arg = 0;

	char cmdCode = temp_buf_[bin_filesize_ - 1];
	int arg_qt = getCmdArgQt(cmdCode);

	if((cmdCode >= CMD_J) && (cmdCode <= CMD_JZ))
	{
		for (j = 0; j < arg_qt; j++)
		{
			if (!sscanf(*bufp, "%d%n", &arg, &argLen))
			{
				printf("\nInvalid argument(NaN) at line %zu\n",
												cur_asm_line_);
				exit(1);
			}
			if (labels_[arg] == -1)
			{
				printf("\nLabel %d wasn't defined\n", arg);
				exit(1);
			}
			if(j == arg_qt - 1)
				*((data_t*)(&temp_buf_[bin_filesize_])) =
												labels_[arg];
			else
				*((data_t*)(&temp_buf_[bin_filesize_])) = arg;

			PRINTF("\targument\t%d\n",
					*((data_t*)(temp_buf_ + bin_filesize_)));
			bin_filesize_ += sizeof(data_t);
			*bufp += argLen;
			skipGaps(bufp);
		}
	}
	else
	{
		for(j = 0; j < arg_qt; j++)
		{
			if (!sscanf(*bufp, "%d%n", &arg, &argLen))
			{
				printf("\nInvalid argument(NaN) at line %zu\n",
													cur_asm_line_);
				exit(1);
			}

			*((data_t*)(&temp_buf_[bin_filesize_])) = arg;
			PRINTF("\targument\t%d\n",
					*((data_t*)(temp_buf_ + bin_filesize_)));
			bin_filesize_ += sizeof(data_t);
			*bufp += argLen;
			skipGaps(bufp);
		}
	}
	FUNC_BYE
}

int Assembler::getLabelNumFromBuf(char** bufp)
{
	FUNC_HI
	int gapLen = -1;
	int slashNIndicator = -1;
	int label_len = -1;
	char label_str[MAX_STR_SIZE] = {};
	sscanf(*bufp, ":%s%n", label_str, &label_len);
	if(label_len != -1)
		*bufp += label_len;

	if (label_len == -1)
	{
		//printf("\nAt line %zu: wrong label\n", cur_asm_line_);
				//Maybe without printf?
		return -1;
	}
	if (**bufp == '\n')
	{
		*bufp += 1;
	}
	else
	{
		sscanf(*bufp,"%*[ \t\r]%n\n%n", &gapLen, &slashNIndicator);
		if (slashNIndicator == -1)
		{
			printf("\nAt line %zu: labels_ must be on "
					"separate lines\n",
					cur_asm_line_);
			exit(1);
		}
		*bufp += slashNIndicator;
	}
	PRINTF("\n\t__SlashInd_is__%d\n", slashNIndicator);

	if (strlen(label_str) > 1)
	{
		printf("\nAt line %zu: wrong label, only 1 digit\n",
				cur_asm_line_);
		exit(1);
	}
	if (!isdigit(*label_str))
	{
		printf("\nAt line %zu: wrong label, only digits\n",
				cur_asm_line_);
		exit(1);
	}
	if (labels_[*label_str - '0'] != -1)
	{
		printf("\nAt line %zu: second label definition\n",
				cur_asm_line_);
		exit(1);
	}

	labels_[*label_str - '0'] = cur_cmd_num_ + 1;
	PRINTF("\n\t__Label_%s_points_to_%zu\n",
			label_str, cur_cmd_num_ + 1);
	return 0;
	FUNC_BYE
}

void Assembler::passingLabels()
{
	FUNC_HI
	assert(asm_buf_);
	char* cptr = asm_buf_;
	char* end_of_asm_buf_ = asm_buf_ + asm_filesize_;

	int skipLen = -1;
	int cmdLen  = -1;
	char cmdName[MAX_STR_SIZE] = {};

#define COLLECT_LABELS
#ifdef COLLECT_LABELS
	while(cptr < end_of_asm_buf_ - 1)
	{
		skipGaps(&cptr);
		int ind = getLabelNumFromBuf(&cptr);

		if(ind == -1)
		{
			PRINTF("\n\t!!!*cptr__%c\n", *cptr);
			assert(MAX_STR_SIZE == 32);
			sscanf(cptr, "%32[A-Za-z]%n", cmdName, &cmdLen);
			PRINTF("\n\t_CMD_LEN_%d\n", cmdLen);
			PRINTF("\n\t_COMMAND_%s\n", cmdName);

			cptr += cmdLen;
			cur_cmd_num_++;
			sscanf(cptr, "%*[^:A-Za-z]%n", &skipLen);
			cur_asm_line_++;
			cptr += skipLen;
		}
		if(ind == 0)
			cur_asm_line_++;
	}
#endif //COLLECT_LABELS
	FUNC_BYE
}

void Assembler::parseCommandsInBinFile()
{
	FUNC_HI
	assert(asm_buf_);
	char* cptr = asm_buf_;
	char* end_of_asm_buf_ = asm_buf_ + asm_filesize_;

	cur_asm_line_  = 0;
	while(cptr < end_of_asm_buf_ - 1)
	{
		skipGaps(&cptr);
		putCmdCodeInTempBuf(&cptr);
		skipGaps(&cptr);

		putCmdArgsInTempBuf(&cptr);

		cur_asm_line_++;
	}

	bin_buf_ = (char*) calloc(bin_filesize_, sizeof(char));
	ALLOC(bin_buf_)

	memcpy(bin_buf_, temp_buf_, bin_filesize_);

#ifdef DEBUG
	dumpString(bin_buf_, bin_filesize_);
#endif //DEBUG
	assert(binfile_);
	assert(bin_filesize_ >= 0);
	fwrite(bin_buf_, sizeof(char), bin_filesize_, binfile_);
	FUNC_BYE
}

int main()
{
	Assembler asm1;
	asm1.deleteComments();
#ifdef DEBUF
	dumpString(asm1.asm_buf_, asm1.asm_filesize_);
#endif //DEBUG
	asm1.passingLabels();
	asm1.deleteLabelsInBuf();
	asm1.parseCommandsInBinFile();

    printf("Assembler file has been compiled into Binary file successfully\n");
	return 0;
}
