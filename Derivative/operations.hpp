#ifdef INCLUDE_OP

#include "myDefines.hpp"

#define OPERATION_WITH_TWO_ARGS(operation)		\
{												\
	double lval = 0;							\
	sscanf(node->leftC_->buf_, "%16lg", &lval);	\
	double rval  = 0;							\
	sscanf(node->rghtC_->buf_, "%16lg", &rval);	\
	delete node->leftC_;						\
	node->leftC_ = NULL;						\
	delete node->rghtC_;						\
	node->rghtC_ = NULL;						\
	double res = lval operation rval;			\
	sprintf(node->buf_, "%lg", res);			\
	node->type_ = TYPE_CONST;					\
	return node;								\
}


#define COMPUTE_LEFT_RGHT_AND_NODE				\
{												\
	node->leftC_ = computer(node->leftC_);		\
	node->rghtC_ = computer(node->rghtC_);		\
	return computer(node);						\
}

#define DERIVATE_LOG(child)									\
{															\
	Node* dnode = new Node("/", dnodeParent, TYPE_OPER);	\
	dnode->leftC_ = derivator(node->child, dnode);			\
	dnode->rghtC_ = copyTree(node->child, dnode);			\
	FUNC_BYE												\
	return dnode;											\
}

#define DERIVATE_COS(child)									\
{															\
	Node* dnode   = new Node("*", dnodeParent, TYPE_OPER);	\
	dnode->leftC_ = new Node("*", dnode, TYPE_OPER);		\
	dnode->leftC_->leftC_ = new Node("-1", dnode,			\
											TYPE_CONST);	\
	dnode->leftC_->rghtC_ = derivator(node->child,	 		\
											dnode->leftC_);	\
	dnode->rghtC_ = new Node("sin", dnode, TYPE_OPER);		\
	dnode->rghtC_->leftC_ = copyTree(node->child,			\
										dnode->rghtC_);		\
	FUNC_BYE												\
	return dnode;											\
}

#define DERIVATE_SIN(child)									\
{															\
	Node* dnode   = new Node("*", dnodeParent, TYPE_OPER);	\
	dnode->leftC_ = new Node("cos", dnode, TYPE_OPER);		\
	dnode->leftC_->leftC_ = copyTree(node->child, 			\
											dnode->leftC_);	\
	dnode->rghtC_ = derivator(node->child, dnode);			\
	FUNC_BYE												\
	return dnode;											\
}

#define DERIVATE_TAN(child)									\
{															\
	Node* dnode = new Node("/", dnodeParent, TYPE_OPER);	\
	dnode->leftC_ = derivator(node->child, dnode);			\
	dnode->rghtC_ = new Node("^", dnode, TYPE_OPER);		\
	dnode->rghtC_->leftC_ = new Node("cos", 				\
								dnode->rghtC_, TYPE_OPER);	\
	dnode->rghtC_->leftC_->leftC_ = copyTree(node->child,	\
								dnode->rghtC_->leftC_);		\
	dnode->rghtC_->rghtC_ = new Node("2", dnode->rghtC_,	\
										TYPE_CONST);		\
	FUNC_BYE												\
	return dnode;											\
}

#define DUMP(oper)					\
{									\
	dumpTex(node->leftC_);			\
	fprintf(texFile_, #oper);		\
	dumpTex(node->rghtC_);			\
}

#define DUMP_ONE_ARG_FUNC(oper)			\
{										\
	fprintf(texFile_, #oper"\\left(");	\
	if(node->leftC_)					\
		dumpTex(node->leftC_);			\
	if(node->rghtC_)					\
		dumpTex(node->rghtC_);			\
	fprintf(texFile_, "\\right)");		\
}



OP_DEF(+,

		checkTwoArgsNotNULL(node);

		if(TwoArgsAreConst(node))
		OPERATION_WITH_TWO_ARGS(+)	

		node = addSimplifications(node);

		COMPUTE_LEFT_RGHT_AND_NODE		
		,

		checkTwoArgsNotNULL(node);

		Node* dnode = new Node("+", dnodeParent, TYPE_OPER);
		dnode->leftC_ = derivator(node->leftC_, dnode);
		dnode->rghtC_ = derivator(node->rghtC_, dnode);
		FUNC_BYE
		return dnode;
		,

		fprintf(texFile_, "{");
		DUMP(+)	
		fprintf(texFile_, "}");
		)
OP_DEF(-,
			
		checkTwoArgsNotNULL(node);

		if(TwoArgsAreConst(node))
		OPERATION_WITH_TWO_ARGS(-)

		node = addSimplifications(node);

		COMPUTE_LEFT_RGHT_AND_NODE
		,

		checkTwoArgsNotNULL(node);

		Node* dnode = new Node("-", dnodeParent, TYPE_OPER);
		dnode->leftC_ = derivator(node->leftC_, dnode);
		dnode->rghtC_ = derivator(node->rghtC_, dnode);
		FUNC_BYE
		return dnode;
		,

		fprintf(texFile_, "{");
		DUMP(-)	
		fprintf(texFile_, "}");

		)
		
OP_DEF(*, 
		
		checkTwoArgsNotNULL(node);

		if(TwoArgsAreConst(node))
		OPERATION_WITH_TWO_ARGS(*)

		node = mulSimplifications(node);

		COMPUTE_LEFT_RGHT_AND_NODE
		,

		checkTwoArgsNotNULL(node);

		Node* dnode   = new Node("+", dnodeParent, TYPE_OPER);
		dnode->leftC_ = new Node("*", dnode, TYPE_OPER);
		dnode->rghtC_ = new Node("*", dnode, TYPE_OPER);
		dnode->leftC_->leftC_ = derivator(node->leftC_, 
											dnode->leftC_);
		dnode->leftC_->rghtC_ = copyTree(node->rghtC_, 
											dnode->leftC_);
		dnode->rghtC_->rghtC_ = derivator(node->rghtC_,
											dnode->rghtC_);
		dnode->rghtC_->leftC_ = copyTree(node->leftC_,
											dnode->rghtC_);

		FUNC_BYE
		return dnode;
		,

		fprintf(texFile_, "{\\left(");
		dumpTex(node->leftC_);
		fprintf(texFile_, "\\right)*\\left(");
		dumpTex(node->rghtC_);
		fprintf(texFile_, "\\right)}");
		)
OP_DEF(/,

		checkTwoArgsNotNULL(node);

		if(TwoArgsAreConst(node))
		OPERATION_WITH_TWO_ARGS(/)	

		node = divSimplifications(node);
		
		COMPUTE_LEFT_RGHT_AND_NODE
		,

		checkTwoArgsNotNULL(node);

		Node* dnode   = new Node("/", dnodeParent, TYPE_OPER);

		dnode->leftC_ = new Node("-", dnode, TYPE_OPER);
		dnode->leftC_->leftC_ = new Node("*", dnode->leftC_,
													TYPE_OPER);
		dnode->leftC_->rghtC_ = new Node("*", dnode->leftC_,
													TYPE_OPER);
		dnode->leftC_->leftC_->leftC_ = derivator(node->leftC_,
										dnode->leftC_->leftC_);
		dnode->leftC_->leftC_->rghtC_ = copyTree(node->rghtC_,
										dnode->leftC_->leftC_);
		dnode->leftC_->rghtC_->leftC_ = copyTree(node->leftC_,
										dnode->leftC_->rghtC_);
		dnode->leftC_->rghtC_->rghtC_ = derivator(node->rghtC_,
										dnode->leftC_->rghtC_);

		dnode->rghtC_ = new Node("^", dnode, TYPE_OPER);
		dnode->rghtC_->leftC_ = copyTree(node->rghtC_,
												dnode->rghtC_);
		dnode->rghtC_->rghtC_ = new Node("2", dnode->rghtC_,
												   TYPE_CONST);

		FUNC_BYE
		return dnode;
		,

		fprintf(texFile_, "{\\frac{");
		dumpTex(node->leftC_);
		fprintf(texFile_, "}{");
		dumpTex(node->rghtC_);
		fprintf(texFile_, "}}");
		)

OP_DEF(^, 
		
		checkTwoArgsNotNULL(node);

		if(TwoArgsAreConst(node))
		{
			performPow(node);
			return node;
		}

		node = powSimplifications(node);

		COMPUTE_LEFT_RGHT_AND_NODE
		,

		checkTwoArgsNotNULL(node);

		Node* dnode   = new Node("*", dnodeParent, TYPE_OPER);
		
		dnode->leftC_ = new Node("^", dnode, TYPE_OPER);

		dnode->leftC_->leftC_ = copyTree(node->leftC_,
											dnode->leftC_);
		dnode->leftC_->rghtC_ = copyTree(node->rghtC_,
											dnode->leftC_);
		
		Node* power = new Node("*", NULL, TYPE_OPER);
		power->leftC_ = new Node("ln", power, TYPE_OPER);
		power->leftC_->leftC_ = copyTree(node->leftC_, 
											power->leftC_);
		power->rghtC_ = copyTree(node->rghtC_, power);
		dnode->rghtC_ = derivator(power, dnode);

		deleteTree(power);
		
		FUNC_BYE
		return dnode;
		,
		fprintf(texFile_, "{{");
		if(node->leftC_->type_ == TYPE_OPER)
		{
			fprintf(texFile_, "\\left(");
			dumpTex(node->leftC_);
			fprintf(texFile_, "\\right)");
		}
		else
			dumpTex(node->leftC_);
		fprintf(texFile_, "}^{");
		
		if(node->rghtC_->type_ == TYPE_OPER)
		{
			fprintf(texFile_, "\\left(");
			dumpTex(node->rghtC_);
			fprintf(texFile_, "\\right)");
		}
		else
			dumpTex(node->rghtC_);
		fprintf(texFile_, "}}");
	
		)
OP_DEF(ln,
		
		checkOnlyOneArg(node);

		if(node->leftC_)
			node = performLog(node->leftC_);

		if(node->rghtC_)
			node = performLog(node->rghtC_);
		
		FUNC_BYE
		return node;
		,

		checkOnlyOneArg(node);

		if(node->leftC_)
			DERIVATE_LOG(leftC_)	
		else //node->rghtC_
			DERIVATE_LOG(rghtC_)
		,

		DUMP_ONE_ARG_FUNC(ln)

		)
OP_DEF(cos,
		
		checkOnlyOneArg(node);

		if(node->leftC_)
			node = performcos(node->leftC_);

		if(node->rghtC_)
			node = performcos(node->rghtC_);

		FUNC_BYE
		return node;
		,

		if(node->leftC_)
			DERIVATE_COS(leftC_)
		else //node->rghtC_
			DERIVATE_COS(rghtC_)
		,

		DUMP_ONE_ARG_FUNC(cos)

		)
OP_DEF(sin, 
		
		checkOnlyOneArg(node);

		if(node->leftC_)
			node = performsin(node->leftC_);

		if(node->rghtC_)
			node = performsin(node->rghtC_);

		FUNC_BYE
		return node;
		,

		if(node->leftC_)
			DERIVATE_SIN(leftC_)
		else
			DERIVATE_SIN(rghtC_)
		,

		DUMP_ONE_ARG_FUNC(sin)

		)
OP_DEF(tg,  
		
		checkOnlyOneArg(node);

		if(node->rghtC_)
			node = performTan(node->rghtC_);

		if(node->leftC_)
			node = performTan(node->leftC_);
			
		FUNC_BYE
		return node;
		,

		if(node->leftC_)
			DERIVATE_TAN(leftC_)
		if(node->rghtC_)
			DERIVATE_TAN(rghtC_)
		,

		DUMP_ONE_ARG_FUNC(tg)

		)

#endif //INCLUDE_OP
