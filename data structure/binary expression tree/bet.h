//--------------------------------------bet.h----------------------------------
/*This is the declearation of the class BET. In this class, a binary expression 
  tree was implemented and can be used to convert postfix expressions into infix 
  expressions.A postfix expression contain 4 types of operators: multiplication 
  (*), division (/), plus (+), and minus (-).
  programmed by: Chaolun Wang
             at: 10/27/2015
*/

#ifndef BET_H
#define BET_H

#include <string>
using namespace std;

class BET
{
public:
    BET();                                                                       // default zero-parameter constructor.
    BET(const string & postfix);                                                 // one-parameter constructor, where parameter "postfix" is string containing a postfix expression. The tree should be built based on the postfix expression. Tokens in the postfix expression are separated by space.
    BET(const BET&);                                                             // copy constructor.
    ~BET();                                                                      // destructor.
    bool buildFromPostfix(const string & postfix);                               // parameter "postfix" is string containing a postfix expression. The tree should be built based on the postfix expression. Tokens in the postfix expression are separated by space. If the tree contains nodes before the function is called, you need to first delete the existing nodes. Return true if the new tree is built successfully. Return false if an error is encountered.
    const BET & operator= (const BET &);                                         // assignment operator.
    void printInfixExpression() const;                                           // call the private version of the printInfixExpression function to print out the infix expression.
    void printPostfixExpression() const;                                         // call the private version of the printPostfixExpression function to print out the postfix expression.
    size_t size() const;                                                         // call the  private version of the size function to return the number of nodes in the tree.
    size_t leaf_nodes() const;                                                   // call the private version of the leaf_nodes function to return the number of leaf nodes in the tree.
    bool empty() const;                                                          // return true if the tree is empty. Return false otherwise.

private:
	struct BinaryNode {                                                          // nested struct which represent the node of binary expression tree
	    string element;
	    BinaryNode *left;
	    BinaryNode *right;

	    BinaryNode(const string &theElement, BinaryNode *lt, BinaryNode *rt)
		: element{theElement}, left{lt}, right{rt} {}
	    BinaryNode(string && theElement, BinaryNode *lt, BinaryNode *rt)
		: element{std::move(theElement)}, left{lt}, right{rt} {}
	};
	
	BinaryNode *root;

    void printInfixExpression(BinaryNode *n) const;                              // print to the standard output the corresponding infix expression. Note that you may need to add parentheses depending on the precedence of operators. You should not have unnecessary parentheses.
    void makeEmpty(BinaryNode* &t);                                              // delete all nodes in the subtree pointed to by t. Called by functions such as the destructor.
    BinaryNode * clone(BinaryNode *t) const;                                     // clone all nodes in the subtree pointed to by t. Called by functions such as the assignment operator=.
    void printPostfixExpression(BinaryNode *n) const;                            // print to the standard output the corresponding postfix expression.
    size_t size(BinaryNode *t) const;                                            // return the number of nodes in the subtree pointed to by t.
    size_t leaf_nodes(BinaryNode *t) const;                                      // return the number of leaf nodes in the subtree pointed to by t.

};

#endif

