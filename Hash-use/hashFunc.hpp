#ifndef INCLUDE_HASH_FUNC
#define INCLUDE_HASH_FUNC

int		hashNull(const char* word);
int		hashFirstLetter(const char* word);
int		hashLength(const char* word);
int		hashSumCodesOfLetters(const char* word);
int		hashSumCycleShiftedCodesOfLetters(const char* word);
int		hashMulCodesOfLetters(const char* word);

#endif //INCLUDE_HASH_FUNC
