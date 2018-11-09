#ifndef MY_DEFINES
#define MY_DEFINES

#ifdef DEBUG 

#define FUNC_HI printf("\nHi, I am %s!\n", __func__);
#define FUNC_BYE printf("\nBye from %s!\n", __func__);
#define PRINTF(...) printf(__VA_ARGS__)
#define curLine {printf("\nI have reached line #%d\n", __LINE__);}

#define IMPROVE

#else //not DEBUG
#define FUNC_HI
#define FUNC_BYE
#define PRINTF(...)
#define curLine


#endif //DEBUG

#define ALLOC(pointer)		\
	if(!pointer)			\
		printf("Allocation error at line %d", __LINE__);

#endif //MY_DEFINES
