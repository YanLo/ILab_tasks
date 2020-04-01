#ifdef CMD_NAME

CMD_DEF(POP, 1, )
CMD_DEF(PUSH, 1, )
CMD_DEF(ADD, 2, 
	Registers_[arguments[0]] += Registers_[arguments[1]];
	)
CMD_DEF(INC, 1,
	Registers_[arguments[0]]++;
	)
CMD_DEF(DEC, 1,
	Registers_[arguments[0]]--;
	)

CMD_DEF(SUB, 2,
	Registers_[arguments[0]] -= Registers_[arguments[1]];
	)	
CMD_DEF(MUL, 2, 
	Registers_[arguments[0]] *= Registers_[arguments[1]];
	)
CMD_DEF(DIV, 2,
	Registers_[arguments[0]] /= Registers_[arguments[1]];
	)
CMD_DEF(IN, 1,
	data_t val = 0;
	printf("\n\tPlease, enter the number, which will be saved in register%d\n",  arguments[0]);
	scanf("%d", &val);
	Registers_[arguments[0]] = val;
	)
CMD_DEF(OUT, 1,
	printf("\nResult is %d\n", Registers_[arguments[0]]);	
	)
CMD_DEF(MOV, 2, 
	Registers_[arguments[0]] = arguments[1];
	)

#define JUMP(argQt)												\
		size_t k;												\
		int j;													\
		k = j = 0;												\
		for(j = 1; j < arguments[argQt -1]; j++)				\
		{														\
			k += sizeof(RAM_[k]) + 								\
						getCmdArgQt(RAM_[k]) * sizeof(data_t);	\
		}														\
		IP_ = k;	

CMD_DEF(J, 1,
	{JUMP(1)}	
	)
CMD_DEF(JE, 3,
	if(Registers_[arguments[0]] == Registers_[arguments[1]])
	{JUMP(3)}
	)
CMD_DEF(JNE, 3,
	if(Registers_[arguments[0]] != Registers_[arguments[1]])
	{JUMP(3)}
	)
CMD_DEF(JG, 3,
	if(Registers_[arguments[0]] > Registers_[arguments[1]])
	{JUMP(3)}	
	)
CMD_DEF(JGE, 3,
	if(Registers_[arguments[0]] >= Registers_[arguments[1]])
	{JUMP(3)}	
	)
CMD_DEF(JL, 3,
	if(Registers_[arguments[0]] < Registers_[arguments[1]])
	{JUMP(3)}		
	)
CMD_DEF(JLE, 3,
	if(Registers_[arguments[0]] <= Registers_[arguments[1]])
	{JUMP(3)}		
	)
CMD_DEF(JZ, 2,
	if(Registers_[arguments[0]] == 0)
	{JUMP(2)}		
	)
#undef JUMP
#endif //CMD_NAME
