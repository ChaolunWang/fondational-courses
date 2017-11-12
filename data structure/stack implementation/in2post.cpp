//----------------------------in2post.cpp-------------------------------------------------
/*This is a program which can evaluate a string of infix equation and changed that into 
  postfix, which stores in a stack container. If the postfix equation do not contain letters
  (which means it is calculable) the program will then calculate the result of the postfix 
  equation.
  programmed by: Chaolun Wang
             at: 10/6/2015
*/

#include "stack.h"
#include <string>
#include <vector>
#include <iostream>
#include <cctype>
using namespace std;
using namespace cop4530;



void split(vector<string> &input, string &inputString);                                                                                         //split string based on ' ', store in a vector of string


int main()
{
		string inputString;	
	do
	{

		vector<string> input;
		Stack<string> operatorStack, postFixStack, posStackCopy;
		Stack<double> operandStack;
		int operandCounter=0;
		bool missingOperand = false;
		bool missingOperator = false;
		bool missingOperandEnd = false;
		int parensCounter = 0;
		cout<<"Enter infix expression (\"exit\" to quit): ";
		getline(cin, inputString);                                                                                                              //read in the input
		split(input, inputString);                                                                                                              //split the inputString into an array of string
		if (inputString!="exit")
		{
			//change infix into postfix formmat and store in stack
			for(auto itr=input.begin(); itr!=input.end(); ++itr)
			{
				if (*itr != "+" && *itr != "-" && *itr != "*" && *itr != "/" && *itr != "(" && *itr != ")")                                     //if meet operands, push operands into postFixStack
				{
					++operandCounter;
					if (operandCounter >= 2)
						missingOperator = true;
					postFixStack.push(*itr);
				}
					
				else                                                                                                                            //if meet operator:
				{
					if(*itr=="+" || *itr=="-")                                                                                                  //+ and -
					{
						--operandCounter;
						if (operandCounter <= -1)
							missingOperand = true;
						while((!operatorStack.empty()) && operatorStack.top()!="(")
						{
							postFixStack.push(operatorStack.top());
							operatorStack.pop();
						}
						operatorStack.push(*itr);
					}
					else if( *itr=="*" || *itr=="/" )                                                                                             //* and /
					{
						--operandCounter;
						if (operandCounter <= -1)
							missingOperand = true;
						while((!operatorStack.empty()) && operatorStack.top()!="+" && operatorStack.top()!="-"  && operatorStack.top()!="(" )
						{
							postFixStack.push(operatorStack.top());
							operatorStack.pop();
						}
						operatorStack.push(*itr);
					}
					else if (*itr == "(")                                                                                                           //(
					{
                        operatorStack.push(*itr);
						++parensCounter;
					}
						
					else if(*itr == ")")                                                                                                            //)
					{
						--parensCounter;
						while((!operatorStack.empty()) && operatorStack.top()!="(" )
						{
							postFixStack.push(operatorStack.top());
							operatorStack.pop();
						}
						if(!operatorStack.empty() && operatorStack.top()=="(")
						    operatorStack.pop();
					}
				}
				
			}
			while(!(operatorStack.empty()))                                                                                                         //put the remaining operand into postFixStack
			{
		    	postFixStack.push(operatorStack.top());
				operatorStack.pop();
			}
			if(operandCounter<1)
				missingOperandEnd = true;
			
			//evaluate if the postfix stack is valid to do the postfix evaluation
			posStackCopy=postFixStack;
			Stack<string> temp;
			bool ifValid=true;
			while(!(postFixStack.empty()))                                                                                                           //check if the postFixStack contain any letters
			{
				if(isalpha(postFixStack.top()[0]))
					ifValid=false;
				temp.push(postFixStack.top());
				postFixStack.pop();
			}
			if (missingOperand || missingOperator || missingOperandEnd || parensCounter != 0)                                                        //check if the postfix stack have any mismatched parens or missing operator/operands
				ifValid = false;
			postFixStack=std::move(temp);
			
			//Postfix evaluation
			while(!postFixStack.empty() && ifValid)
			{
				if(postFixStack.top()!="+" && postFixStack.top()!="-" && postFixStack.top()!="*" && postFixStack.top()!="/")
				{
					operandStack.push(stod(postFixStack.top(), nullptr));
					postFixStack.pop();
				}
				else
				{
					double result;
					double operand2=operandStack.top();
					operandStack.pop();
					double operand1=operandStack.top();
					operandStack.pop();
					if(postFixStack.top()=="+")
						result=operand1 + operand2;
					else if(postFixStack.top()=="-")
						result=operand1 - operand2;
					else if(postFixStack.top()=="*")
						result=operand1 * operand2;
					else
						result=operand1 / operand2;
					operandStack.push(result);
					postFixStack.pop();

				}
			}

	        //exception handeling block
		    if(parensCounter != 0)                                                                                                                   //mismatched parens
		    	cerr << "\nError: Infix expression: " << inputString << " has mismatched parens!\n";
		    else if (missingOperator)                                                                                                                //missing operator/operand
		    {
		    	cerr << "\nError: Missing operators in the expression\n";
	    	}
	        else if(missingOperand)
		    { 
		    	cerr<<"\nError: Missing operand in the expression\n";
		    }
		    else if (missingOperandEnd)                                                                                                               //missing operand in infix at the end
		    {
			    cout << "\nPostfix expression: " << posStackCopy << "\n";
			    cout << "Postfix evaluation: " << posStackCopy << " = ";
		    	cerr << "\nError: Missing operand in postfix string. Unable to evaluate postfix string!\n";
		    }
		    else if(ifValid)                                                                                                                         //contain letters
		    {
			    cout<<"\nPostfix expression: "<<posStackCopy<<"\n";
		        cout<<"Postfix evaluation: "<<posStackCopy<<" = "<<operandStack<<"\n";
		    }
		    else                                                                                                                                     //normal situation with no fault
		    {
			    cout<<"\nPostfix expression: "<<posStackCopy<<"\n";
		    	cout<<"Postfix evaluation: "<<posStackCopy<<" = "<<posStackCopy<<"\n";
		    }
		    operandStack.clear();
	     }
       
		    else
			    cout<<'\n';

	}while(inputString!="exit");
}









void split(vector<string> &input, string &inputString)                                                                                           //split string based on ' ', store in a vector of string                                                   
{
	string symble;
	bool isSpace=true;
	for(auto itr=inputString.begin(); itr!=inputString.end(); ++itr)
	{
		if(*itr!=' ')
		{
			symble+=*itr;
			isSpace=false;
		}
		else 
		{
			if(!isSpace)
				input.push_back(symble);
			symble.clear();
			isSpace=true;
		}
	}
	if(!symble.empty())
		input.push_back(symble);
}