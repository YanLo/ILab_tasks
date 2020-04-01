#include <math.h>
#include <string.h>

#include "hashFunc.hpp"

enum
{
	HASH_TABLE_SIZE = 257,
	MAX_WORD_SIZE	= 128,
};

int hashNull(const char* word)
{
	return 0;
}

int hashFirstLetter(const char* word)
{
	return *word;
}

int hashLength(const char* word)
{
	return strnlen(word, MAX_WORD_SIZE) % HASH_TABLE_SIZE;
}

int hashSumCodesOfLetters(const char* word)
{
	int len = strnlen(word, MAX_WORD_SIZE);
	int i, sum;
	i = sum = 0;
	for(i = 0; i < len; i++)
		sum += word[i];

	return sum = sum % HASH_TABLE_SIZE;
}

int hashSumCycleShiftedCodesOfLetters(const char* word)
{
	int len = strnlen(word, MAX_WORD_SIZE);
	int i, sum;
	i = sum = 0;
	for(i = 0; i < len; i++)
		sum += word[i] >> 1;

	return sum = sum % HASH_TABLE_SIZE;
}

int hashMulCodesOfLetters(const char* word)
{
	int len = strnlen(word, MAX_WORD_SIZE);
	int i, mul;
	i = 0;
	mul = 1;
	for(i = 0; i < len; i++)
		mul *= word[i] >> 5;

	return mul = mul % HASH_TABLE_SIZE;
}
