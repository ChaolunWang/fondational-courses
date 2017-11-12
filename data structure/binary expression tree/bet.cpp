//--------------------------------------bet.cpp----------------------------------
/*This is the implementation of the class BET. In this class, a binary expression 
  tree was implemented and can be used to convert postfix expressions into infix 
  expressions.A postfix expression contain 4 types of operators: multiplication 
  (*), division (/), plus (+), and minus (-).
  programmed by: Chaolun Wang
             at: 10/27/2015
*/

#include "bet.h"
#include <iostream>
#include <stack>
using namespace std;

BET::BET() : root{nullptr}                                                       // default zero-parameter constructor.
{	
}

BET::BET(const string & postfix) : root{nullptr}                                 // one-parameter constructor, where parameter "postfix" is string containing a postfix expression. The tree should be built based on the postfix expression. Tokens in the postfix expression are separated by space.
{
	buildFromPostfix(postfix);		
}

BET::BET(const BET& rhs) : root{clone(rhs.root)}                                 // copy constructor.
{
}

BET::~BET()                                                                      // destructor.
{
	makeEmpty(root);
}

bool BET::buildFromPostfix(const string & postfix)                               // parameter "postfix" is string containing a postfix expression. The tree should be built based on the postfix expression. Tokens in the postfix expression are separated by space. If the tree contains nodes before the function is called, you need to first delete the existing nodes. Return true if the new tree is built successfully. Return false if an error is encountered.
{
	makeEmpty(root);
	stack< BinaryNode * > nodeStack;
	string element;
	for(int i=0; i<postfix.size(); ++ i)
	{
		if(postfix[i]!=' ')
			element+=postfix[i];
		if(postfix[i]==' ' || i==postfix.size()-1)
		{
			if(element!="+" && element!="-" && element!="*" && element!="/")
			{
				nodeStack.push(new BinaryNode(element, nullptr, nullptr));
			}
			else
			{
				if(nodeStack.empty())
				{
					cerr<<"Wrong postfix expression\n";
					return false;
				}
				BinaryNode * tempRight=nodeStack.top();
				nodeStack.pop();
			    if(nodeStack.empty())
				{
					cerr<<"Wrong postfix expression\n";
					return false;
				}
				BinaryNode * tempLeft=nodeStack.top();
				nodeStack.pop();
				nodeStack.push(new BinaryNode(element, tempLeft, tempRight));
			}
			element.clear();
		}
	}
	if (nodeStack.size()!=1)
	{
		while(!nodeStack.empty())
		{
			makeEmpty(nodeStack.top());
			nodeStack.pop();
		}
		cerr<<"Wrong postfix expression\n";
		return false;
	}
	root=nodeStack.top();
	nodeStack.pop();
	return true;
}

const BET & BET::operator= (const BET & rhs)                                     // assignment operator.
{
	makeEmpty(root);
	root=clone(rhs.root);
}

void BET::printInfixExpression() const                                           // call the private version of the printInfixExpression function to print out the infix expression.
{
	printInfixExpression(root);
	cout<<'\n';
}

void BET::printPostfixExpression() const                                        // call the private version of the printPostfixExpression function to print out the postfix expression.
{
	printPostfixExpression(root);
	cout<<'\n';
}

size_t BET::size() const                                                        // call the  private version of the size function to return the number of nodes in the tree.
{
	return size(root);
}

size_t BET::leaf_nodes() const                                                   // call the private version of the leaf_nodes function to return the number of leaf nodes in the tree.
{
	return leaf_nodes(root);
}

bool BET::empty() const                                                         // return true if the tree is empty. Return false otherwise.
{
	if (size())
		return false;
	return true;
}

//private mumber functions 


void BET::printInfixExpression(BinaryNode *n) const                              // print to the standard output the corresponding infix expression. Note that you may need to add parentheses depending on the precedence of operators. You should not have unnecessary parentheses.
{
	if(n==nullptr)
		return;
	else
	{
		if((n->element=="*" || n->element=="/") && n->left!=nullptr && (n->left->element=="+" || n->left->element=="-") )
			cout<<"( ";
		printInfixExpression(n->left);
		if((n->element=="*" || n->element=="/") && n->left!=nullptr && (n->left->element=="+" || n->left->element=="-") )
			cout<<") ";
		cout<<(n->element)<<' ';
		if(((n->element=="*" || n->element=="/") && n->right!=nullptr && (n->right->element=="+" || n->right->element=="-" || n->right->element=="*" || n->right->element=="/") )|| 
			((n->element=="+" || n->element=="-") && n->right!=nullptr && (n->right->element=="+" || n->right->element=="-") ))
			cout<<"( ";
	    printInfixExpression(n->right);
		if(((n->element=="*" || n->element=="/") && n->right!=nullptr && (n->right->element=="+" || n->right->element=="-" || n->right->element=="*" || n->right->element=="/") )|| 
			((n->element=="+" || n->element=="-") && n->right!=nullptr && (n->right->element=="+" || n->right->element=="-") ))
			cout<<") ";
	}
}

void BET::makeEmpty(BinaryNode* &t)                                              // delete all nodes in the subtree pointed to by t. Called by functions such as the destructor.
{
	if(t!=nullptr)
	{
		makeEmpty(t->left);
		makeEmpty(t->right);
		delete t;
	}
	t=nullptr;
}

BET::BinaryNode * BET::clone(BinaryNode *t) const                            // clone all nodes in the subtree pointed to by t. Called by functions such as the assignment operator=.
{
	if(t==nullptr)
		return nullptr;
	else
		return new BinaryNode(t->element, clone(t->left), clone(t->right));
}

void BET::printPostfixExpression(BinaryNode *n) const                            // print to the standard output the corresponding postfix expression.
{
	if(n==nullptr)
		return;
	else
	{
		printPostfixExpression(n->left);
	    printPostfixExpression(n->right);
		cout<<(n->element)<<' ';
	}
}

size_t BET::size(BinaryNode *t) const                                            // return the number of nodes in the subtree pointed to by t.
{
	if(t==nullptr)
		return 0;
	else
		return 1+size(t->left)+size(t->right);
}

size_t BET::leaf_nodes(BinaryNode *t) const                                       // return the number of leaf nodes in the subtree pointed to by t.
{
	if(t==nullptr)
		return 0;
	else if(t->left==nullptr && t->right==nullptr)
		return 1;
	else
		return leaf_nodes(t->left)+leaf_nodes(t->right);
}
