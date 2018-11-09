#ifndef NODE_CLASS
#define NODE_CLASS


enum node_types
{
	TYPE_CONST,
	TYPE_OPER,
	TYPE_VAR,
	TYPE_DEFAULT,
};

class Node
{
	public:
		char*	buf_;
		Node*	leftC_;
		Node*	rghtC_;
		Node*	parent_;
		int		type_;
		int		visit_count_qt_;

		Node();
		Node(const char* value);
		Node(const char* value, int type);
		Node(const char* value, Node* parent, int type);
		Node(const char* value, Node* rghtC, Node* leftC);
		Node(const char* value, Node* rghtC, Node* leftC, 
								  Node* parent);
		~Node();
	//private:
		
};

#endif ///NODE_CLASS
