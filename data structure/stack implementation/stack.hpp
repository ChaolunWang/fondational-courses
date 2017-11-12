//-----------------------------------------------stack.hpp-------------------------


		
template <typename T>
Stack<T>::Stack()                                     // zero-argument constructor.   
{
}

template <typename T>
Stack<T>::~Stack ()
// destructor.  
{
}

template <typename T>
Stack<T>::Stack (const Stack<T>& stk)
// copy constructor.   
{
	data=stk.data;
}

template <typename T>
Stack<T>::Stack(Stack<T> && stk)
// move constructor.
{
	data=std::move(stk.data);
}

template <typename T>
Stack<T>& Stack<T>::operator= (const Stack <T>& stk )
// copy assignment operator=.
{
	if(&stk==this)
		return *this;
	data=stk.data;
	return *this;
}

template <typename T>
Stack<T> & Stack<T>::operator=(Stack<T> && stk)
// move assignment operator=
{
	if(&stk==this)
		return *this;
	data=std::move(stk.data);
	return *this;
}
 
template <typename T>
bool Stack<T>::empty() const
// returns true if the Stack contains no elements, and false otherwise.  
{
	return data.empty();
}

template <typename T>
void Stack<T>::clear()
// delete all elements from the stack.
{
	data.clear();
}

template <typename T>
void Stack<T>::push(const T& x)
// adds  x  to the Stack.   copy version.
{
	data.push_back(x);

}

template <typename T>
void Stack<T>::push(T && x)
// adds x to the Stack. move version.
{
	data.push_back(std::move(x));
}

template <typename T>
void Stack<T>::pop()
// removes and discards the most recently added element of the Stack.  
{
	data.pop_back();
}

template <typename T>
T& Stack<T>::top()
// mutator that returns a reference to the most recently added element of the Stack.  
{
	return data[data.size()-1];
}

template <typename T>
const T& Stack<T>::top() const
// accessor that returns the most recently added element of the Stack.  
{
	return data[data.size()-1];
}

template <typename T>
int Stack<T>::size() const
// returns the number of elements stored in the Stack.
{
	return data.size();
}

template <typename T>
void Stack<T>::print(std::ostream& os, char ofc) const
// print elements of Stack to ostream os. ofc is the separator between elements in the stack when they are printed out. Note that print() prints elements in the opposite order of the Stack (that is, the oldest element should be printed first).
{
	typename vector<T>::const_iterator itr;
	itr=data.begin();
	while(itr!=--data.end())
	{
		os<<*itr<<ofc;
		++itr;
	}
	os<<*itr;
}



template <typename T>
std::ostream& operator<< (std::ostream& os, const Stack<T>& a)
//invokes the print() method to print the Stack<T> a in the specified ostream    
{
	a.print(os);
	return os;
}

template <typename T>
bool operator== (const Stack<T>& stl, const Stack <T>& str)
// returns true if the two compared Stacks have the same elements, in the same order.  
{
	if(stl.size()!= str.size())
		return false;
	Stack<T> templ=stl;
	Stack<T> tempr=str;
	while(!templ.empty())
	{
		if(templ.top()!=tempr.top())
			return false;
		templ.pop();
		tempr.pop();
	}
	return true;
}

template <typename T>
bool operator!= (const Stack<T>& stl, const Stack <T>& str)
// returns true if the two compared Stacks do not have the same elements, in the same order. 
{
	return !(stl==str);
}

template <typename T>
bool operator< (const Stack<T>& a, const Stack <T>& b)
// returns true if every element in Stack a is smaller or equal to corresponding elements of Statck b.
{
	if(a.size() > b.size())
		return false;
	Stack<T> templ=a;
	Stack<T> tempr=b;
	while(!templ.empty())
	{
		if(templ.top()>tempr.top())
			return false;
		templ.pop();
		tempr.pop();
	}
	return true;
}

