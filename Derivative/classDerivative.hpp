#ifndef DERIVATIVE
#define DERIVATIVE

#include "node.hpp"
#include <cstdio>

class Derivative
{
	public:
		Node*	root_;
		Node*	droot_;
		
		Derivative();
		Derivative(const char* fileName);
		~Derivative();
		void	loadInFile(Node* node);
		void	loadInFile(const char* fileName, Node* node);
		void	writeTree(Node* node);
		int		typeDefiner(const char* value);

		Node*	derivator(Node* node, Node* dnodeParent);
		Node*	copyTree(Node* node, Node* cnodeParent);

		Node*	computer(Node* node);
		void	checkTwoArgsNotNULL(Node* node);
		void	checkOnlyOneArg(Node* node);
		bool	TwoArgsAreConst(Node* node);
		Node*	curNodeReplacedByRght(Node* node);
		Node*	curNodeReplacedByLeft(Node* node);
		Node*	addSimplifications(Node* node);	
		Node*	mulSimplifications(Node* node);
		Node*	divSimplifications(Node* node);
		Node*	powSimplifications(Node* node);
		void	performPow(Node* node);
		Node*	performLog(Node* node);
		Node*	performsin(Node* node);
		Node*	performcos(Node* node);
		Node*	performTan(Node* node);

		Node*	buildTree(Node* parent);
		void	deleteTree(Node* node);
		void	dumpTree(Node* node);
		void	dump(Node* node);
		void	dumpTex(Node* root);
		void	mathDump();

		Node*	headBuildFunc();
		Node*	addBuildFunc();
		Node*	mulBuildFunc();
		Node*	powBuildFunc();
		Node*	mathBuildFunc();
		Node*	hookBuildFunc();
		Node*	workManBuildFunc();

	
	private:
		FILE*	inputFile_;
		FILE*	outputFile_;
		FILE*	dotFile_;
		char*	tree_buf_;
		size_t 	slider_;
		FILE*	texFile_;

};

#endif //DERIVATIVE
