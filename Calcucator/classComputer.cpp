#include "classComputer.hpp"
#include "readInBufFromFile.hpp"
#include <assert.h>
#include <stdlib.h>
#include <math.h>

//#define DEBUG
#include "myDefines.hpp"

Computer::Computer():
	expFile_	(NULL),
	slider_		(0),
	buf_		(NULL)
{
	FUNC_HI
	expFile_ = fopen("exp", "r");
	if(!expFile_)
	{
		perror("exp");
		exit(1);
	}
	buf_ = readInBufFromFile(expFile_);
	FUNC_BYE
}

Computer::~Computer()
{
	expFile_	= NULL;
	slider_		= 0;
	free(buf_);
	buf_		= NULL;
}

int Computer::headFunc()
{
	FUNC_HI
	assert(buf_);
	int val = addFunc();
	if(buf_[slider_] == '\0')
		return val;
	else
	{
		printf("\n\tWrong syntax!\n");
		exit(1);
	}
	FUNC_BYE
}

int Computer::addFunc()
{
	FUNC_HI
	int val = mulFunc();
	//printf("\n\tcurRes %d\n", res_);
	while(buf_[slider_] == '-' || buf_[slider_] == '+')
	{
		slider_++;
		if(buf_[slider_ - 1] == '-')
			val -= mulFunc();
		if(buf_[slider_ - 1] == '+')
			val += mulFunc();
	//printf("\n\tcurRes %d curslider %d \n", res_, slider_);

	}

	FUNC_BYE
	return val;
}

int Computer::mulFunc()
{
	FUNC_HI
	int val = powFunc();
	while(buf_[slider_] == '*' || buf_[slider_] == '/')
	{
		slider_++;
		if(buf_[slider_ - 1] == '/')
			val /= powFunc();
		if(buf_[slider_ - 1] == '*')
			val *= powFunc();
	//	printf("\n\tcurRes %d curslider %d \n", res_, slider_);
	}
	FUNC_BYE
	return val;
}

int Computer::powFunc()
{
	FUNC_HI
	int val = hookFunc();
	while(buf_[slider_] == '^')
	{
		slider_++;
		val = pow(val, hookFunc());
	}
	FUNC_BYE
	return val;
}

int Computer::hookFunc()
{
	FUNC_HI
	int val = 0;
	if(buf_[slider_] == '(')
	{
		slider_++;
		val = addFunc();
		if(buf_[slider_] != ')')
		{
			printf("\n\tMiising closing hook\n");
			exit(1);
		}
		slider_++;
	}
	else
		val = workManFunc();
	FUNC_BYE
	return val;
}

int Computer::workManFunc()
{
	FUNC_HI
	int val = 0;
	while(buf_[slider_] >= '0' && buf_[slider_] <= '9')
	{
		val = val * 10 + buf_[slider_] - '0';
		slider_++;
	}
	FUNC_BYE
	return val;
}

