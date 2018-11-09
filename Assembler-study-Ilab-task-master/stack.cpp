//#include <iostream>
//#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

#define DEBUG

#ifdef DEBUG
#define FUNC_PRINT(...) printf(__VA_ARGS__)
#else
#define FUNC_PRINT(...)
#endif	//FUNC_PRINT

#define ALLOC(pointer)		\
	if(!pointer)			\
		printf("Allocation error at line %d", __LINE__);


typedef int data_t;

enum PSN
{
	PSN_INT = 0xCAABAABA,
	//ADD_INT = 0xABCDEFAB
};

enum STKERRS
{
	STKERR_OK,
#define PUT_ERR(condition, err)				\
	STKERR_##err,
#define STK_ERRORS
#include "stkerrors.h"
#undef STK_ERRORS
#undef PUT_ERR
};

//#define CHECK_IF_IMPROVED

#ifndef CHECK_IF_IMPROVED
#define IMPROVE
#endif
#undef CHECK_IF_IMPROVED

#include "dumpString.h"

class Stack
{
    public:
        Stack(size_t capacity);
        ~Stack ();
        void    Push(data_t value);
        data_t  Pop(int* errcode);       
        size_t  getSize();
        size_t  getCapacity();
        data_t  Add(int* errcode);
        data_t  Mul(int* errcode);
        data_t  Sub(int* errcode);
        data_t  Div(int* errcode);
        data_t  Peek(int* errcode);
		int 	Ok();
		void	Dump();

    private:
        data_t* data_buf_;
		data_t* Data_;
        data_t* data_canary_left_;
        data_t* data_canary_right_;
        size_t  capacity_;
        size_t  counter_;
        size_t  check_sum_;
        int     getCheckSum();
       	int     Extend();
		
};




Stack::Stack(size_t capacity) //stack_c_tor
//			capacity_(capacity),
{
	FUNC_PRINT("\nHi, I am %s\n", __func__);

	capacity_ = capacity;
	data_buf_ = NULL;
	data_buf_ = (data_t*) calloc (capacity_ + 2 , sizeof(data_t));
	ALLOC(data_buf_);
	data_canary_left_   = data_buf_;
	data_canary_right_  = data_buf_ + capacity_ + 1;
	*data_canary_left_  = *data_canary_right_ = PSN_INT;  // canary
	Data_ = data_buf_ + 1;
	counter_ = 0;
	check_sum_ = getCheckSum();
	printf("Capacity\t%zu\n", capacity_);
	printf("Counter\t%zu\n", counter_);

	FUNC_PRINT("\nBye from %s\n", __func__);

}

Stack::~Stack()
{
	FUNC_PRINT("\nHi, I am %s\n", __func__);

	capacity_ = 0;
	counter_ = 0;
	free(data_buf_);

	FUNC_PRINT("\nBye from %s\n", __func__);

}

int Stack::getCheckSum()
{

	FUNC_PRINT("\nHi! I am %s\n", __func__);

    int sum = 0;
    int i;

    for (i = 0; i < counter_; i++)
        sum += Data_[i];

    return getCapacity() + getSize() + sum;

	FUNC_PRINT("\nBye from %s\n", __func__);

}

int Stack::Ok()
{
	FUNC_PRINT("\nHi, I am %s\n", __func__);

	STKERRS error = STKERR_OK;
#define PUT_ERR(condition, err)			\
	if (condition)						\
		error = STKERR_##err;			
#define STK_ERRORS
#include "stkerrors.h"
#undef STK_ERRORS
	return error;
#undef	PUT_ERR 


	FUNC_PRINT("\nBye from %s\n", __func__);

}

void Stack::Dump()
{
	FUNC_PRINT("\nHi, I am %s\n", __func__);

#define PUT_ERR(condition, err)				\
	case STKERR_##err:						\
		printf("\n\tStackOk: %s\n", #err);	\
		break;
	switch(Ok())
	{
		case STKERR_OK:
			printf("\n\tStackOk: Ok\n");
			break;
#define STK_ERRORS
#include "stkerrors.h"
#undef STK_ERRORS
	}
#undef PUT_ERR
	printf("Counter\t%zu\n", counter_);
	printf("Size\t%zu\n", counter_);
	printf("Data\n");
	int i;
	for (i = 0; i < counter_; i++)
	{
		printf("%d\n", Data_[i]);
	}

	FUNC_PRINT("\nBye from %s\n", __func__);

}

int Stack::Extend()
{
	FUNC_PRINT("\nHi, I am %s\n", __func__);

    capacity_ *= 2;
    void* errcode = NULL;
	errcode = realloc(data_buf_, (capacity_ + 2) * sizeof(data_t));
	if (!data_buf_ || !errcode)
	{
		printf("Allocation error at line %d\n", __LINE__);
		return -1;
	}
    data_canary_left_  = data_buf_;
    data_canary_right_ = data_buf_ + capacity_ + 1;
    Data_ = data_buf_ + 1;
    *data_canary_left_  = *data_canary_right_ = PSN_INT;
    check_sum_ = getCheckSum();

	FUNC_PRINT("\nBye from %s\n", __func__);

    return 0;
}


IMPROVE

//Maybe without errcode, but check with Stack:Ok?

#define OPERATIONFUNC(namefunc, errString, act)	\
data_t Stack::namefunc(int* errcode)			\
{												\
    if( counter_ <= 1)							\
    {											\
        printf(errString);						\
        *errcode = STKERR_EMPTY;				\
        return -1;								\
    } else {									\
        Push(Pop(errcode) act Pop(errcode));	\
        check_sum_ = getCheckSum();				\
        *errcode = STKERR_OK;					\
    }											\
    return 0;									\
}

IMPROVE

OPERATIONFUNC(Add, "Cannot perform addition\n", +)
OPERATIONFUNC(Mul, "Cannot perform multiplication\n", *)
OPERATIONFUNC(Sub, "Cannot perform subtraction\n", -)
OPERATIONFUNC(Div, "Cannot perform dividing\n", /)


void Stack::Push(data_t value)
{
	FUNC_PRINT("\nHi, I am %s\n", __func__);

	if (counter_ >= capacity_)
		Extend();
    Data_[counter_++] = value;
}

size_t Stack::getSize()
{
	FUNC_PRINT("\nHi, I am %s\n", __func__);


	FUNC_PRINT("\nBye from %s\n", __func__);

    return counter_;
}

size_t Stack::getCapacity()
{

	FUNC_PRINT("\nHi! I am %s\n", __func__);


	FUNC_PRINT("\nBye from %s\n", __func__);

    return capacity_;
}

IMPROVE

//Stack::Ok?

#define POPEEK(namefunc, errString, index)	\
data_t Stack::namefunc(int* errcode)		\
{											\
    if (!getSize())							\
    {										\
        printf(errString);					\
        return -1;							\
    }										\
    data_t popped_data = Data_[index];		\
    check_sum_ = getCheckSum();				\
    *errcode = STKERR_OK;					\
    return popped_data;						\
}

POPEEK(Pop, "Stack is empty\n", counter_--)
POPEEK(Peek, "Stack is empty\n", counter_ - 1)

#ifndef DISABLE_STACK_MAIN

int main()
{
    Stack s(10);
	s.Dump();
    return 0;
}

#endif
