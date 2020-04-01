#define DUMP_READ_TREE_FROM_FILE
char Akinator::readInBufFromFile()
{
	FUNC_HI
	assert(inputFile_);
	fseek(inputFile_, 0, SEEK_END);
	filesize_ = ftell(inputFile_) + 1;
	rewind(inputFile_);

	char* tree_buf_ = (char*) calloc(filesize_, sizeof(char));
	ALLOC(tree_buf_); //defined in myDefines
	fread(tree_buf_, sizeof(char), filesize_, inputFile_);
#ifdef DUMP_READ_TREE_FROM_FILE
	dumpString(tree_buf_, filesize_);
#endif
	assert(tree_buf_);
	FUNC_BYE
}
#undef DUMP_READ_TREE_FROM_FILE

Node::Node()//:
//		leftC_      (NULL),
//       ....
//      isDestructed(false)
{
	FUNC_HI //defined in myDefines
	value_	= 0;
	leftC_	= NULL;
	rghtC_	= NULL;
	parent_ = NULL;
	isDestructed = false;
	FUNC_BYE
}

Node::Node(data_t value)
{
	FUNC_HI
	value_	= value;
	leftC_	= NULL;
	rghtC_	= NULL;
	parent_ = NULL;
	isDestructed = false;
	FUNC_BYE
}

Node::Node(data_t value, Node* rghtC, Node* leftC)
{
	FUNC_HI
	value_	= value;
	leftC_	= rghtC;
	leftC_	= leftC;
	parent_ = NULL;
	isDestructed = false;
	FUNC_BYE
}

Node::Node(data_t value, Node* rghtC, Node* leftC, Node* parent)
{
	FUNC_HI
	value_	= value;
	leftC_	= rghtC;
	leftC_	= leftC;
	parent_ = parent;
	isDestructed = false;
	FUNC_BYE
}

void Akinator::printTree(Node* node)
{
	FUNC_HI
	if(!node)
		return;
	if(node)
	{

		printf("\n\t_node__%p__val__is__%s\n", node, node->buf_);
		if(node->leftC_)
			printTree(node->leftC_);


		if(node->rghtC_)
			printTree(node->rghtC_);
	}
	FUNC_BYE
}
