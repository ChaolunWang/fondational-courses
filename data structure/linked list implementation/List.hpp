//----------------------------------List.hpp--------------------------------------------------------
/*In this file, the definision of List template class was implemented. This class is a template class
  which implement the bluebrint of double linked list, it also have nested class const_iterator and 
  iterator.
  Programmed by: Chaolun Wang
             at: 09/22/2015
*/




// default zero parameter constructor
template <typename T>
List<T>::const_iterator::const_iterator(): current(nullptr)
{} 

// operator*() to return element
template <typename T>
const T & List<T>::const_iterator::operator*() const
{
	return retrieve();
}                       
						
// increment/decrement operators
//pre-increment
template <typename T>						
typename List<T>::const_iterator & List<T>::const_iterator::operator++()
{
	current=current->next;
	return *this;
}						

//post increment
template <typename T>
typename List<T>::const_iterator List<T>::const_iterator::operator++(int)
{
	const_iterator temp=*this;
	current=current->next;
	return temp;
}

//pre-decrement
template <typename T>
typename List<T>::const_iterator & List<T>::const_iterator::operator--()
{
	current=current->prev;
	return *this;
}

//post-decrement
template <typename T>
typename List<T>::const_iterator List<T>::const_iterator::operator--(int)
{
	const_iterator temp=*this;
	current=current->prev;
	return temp;
}

// comparison operators
//equal to
template <typename T>
bool List<T>::const_iterator::operator==(const const_iterator &rhs) const
{
	if (this->current==rhs.current)
		return true;
	else
		return false;
}

//not equal to
template <typename T>
bool List<T>::const_iterator::operator!=(const const_iterator &rhs) const
{
	return !(*this==rhs);
}
						
// retrieve the element refers to
template <typename T>
T & List<T>::const_iterator::retrieve() const
{
	return current->data;
}

// protected constructor for const_iterator						
template <typename T>
List<T>::const_iterator::const_iterator(Node *p): current{p}
{}
						
//constructor for iterator
template <typename T>
List<T>::iterator::iterator()
{}
	
//retreve the content
template <typename T>
T & List<T>::iterator::operator*()
{
	return const_iterator::retrieve();
}

//const version of retreve content
template <typename T>
const T & List<T>::iterator::operator*() const
{
	return const_iterator::operator*();
}

// increment/decrement operators
//pre-increment
template <typename T>
typename List<T>::iterator & List<T>::iterator::operator++()
{
	this->current=this->current->next;
	return *this;
}

//pos-increment
template <typename T>
typename List<T>::iterator List<T>::iterator::operator++(int)
{
	iterator temp=*this;
	this->current=this->current->next;
	return temp;
}

//pre-decrement
template <typename T>
typename List<T>::iterator & List<T>::iterator::operator--()
{
	this->current=this->current->prev;
	return *this;
}
	
//pos-decrement
template <typename T>
typename List<T>::iterator List<T>::iterator::operator--(int)
{
	iterator temp=*this;
    this->current=this->current->prev;
	return temp;
}

//constructor for iterator
template <typename T>
List<T>::iterator::iterator(Node *p): const_iterator{p}
{}
	
// constructor, desctructor, copy constructor
// default zero parameter constructor
template <typename T>
List<T>::List()
{
	init();
}
				
// copy constructor
template <typename T>
List<T>::List(const List &rhs)
{
	init();
	for(auto & i:rhs)
		push_back(i);
}
				
// move constructor
template <typename T>
List<T>::List(List && rhs): theSize{rhs.theSize}, head{rhs.head}, tail{rhs.tail}
{
	rhs.theSize=0;
	rhs.head=0;
	rhs.tail=0;
}
				
// num elements with value of val
template <typename T>
List<T>::List(int num, const T& val)
{
	init();
	for(int i=0; i<num; ++i)
		push_back(val);
}
				
// constructs with elements [start, end)
template <typename T>
List<T>::List(const_iterator start, const_iterator end)
{
	init();
	for(auto itr=start; itr!=end; ++itr)
		push_back(*itr);
}
				
// destructor
template <typename T>
List<T>::~List()
{
	clear();
	delete head;
	delete tail;
}

// copy assignment operator
template <typename T>
const List<T>& List<T>::operator=(const List &rhs)
{
	List temp=rhs;
	std::swap(*this, temp);
	return *this;
}
				
// move assignment operator
template <typename T>
List<T> & List<T>::operator=(List && rhs)
{
	std::swap(theSize, rhs.theSize);
	std::swap(head, rhs.head);
	std::swap(tail, rhs.tail);
	return *this;
}

// member functions
// number of elements
template <typename T>
int List<T>::size() const
{
	return theSize;
}
				
// check if list is empty
template <typename T>
bool List<T>::empty() const
{
	return theSize==0;
}

// delete all elements
template <typename T>
void List<T>::clear()
{
	while(theSize)
		pop_back();
}
				
// reverse the order of the elements
template <typename T>
void List<T>::reverse() 
{
	List temp = *this;
	clear();
	for (auto i : temp)
		push_front(i);
}
				
// reference to the first element
template <typename T>
T & List<T>::front()
{
	return *begin();
}
	
// reference to the first element const version
template <typename T>
const T& List<T>::front() const
{
	return *begin();
}
				
// reference to the last element
template <typename T>
T & List<T>::back()
{
	return *--end();
}
			
// reference to the last element const version
template <typename T>
const T & List<T>::back() const
{
	return *--end();
}

//add element at front of the list
template <typename T>
void List<T>::push_front(const T & val)
{
	insert(begin(), val);
}
				
// move version of insert
template <typename T>
void List<T>::push_front(T && val) 
{
	insert(begin(), std::move(val));
}

// insert to the end
template <typename T>
void List<T>::push_back(const T & val)
{
	insert(end(), val);
}
				
// move version of insert
template <typename T>
void List<T>::push_back(T && val)
{
	insert(end(), std::move(val));
}
				
// delete first element
template <typename T>
void List<T>::pop_front()
{
	erase(begin());
}
				
// delete last element
template <typename T>
void List<T>::pop_back()
{
	erase(--end());
}
				
// remove all elements with value = val
template <typename T>
void List<T>::remove(const T &val)
{
	auto itr=begin();
	while (itr!=end())
	{
		if(*itr==val)
			itr=erase(itr);
		else
			++itr;
	}
}

// print out all elements. ofc is deliminitor
template <typename T>
void List<T>::print(std::ostream& os, char ofc) const
{
	for(auto itr=begin(); itr!=--end(); ++itr)
		os<<*itr<<' ';
	os<<*--end();
}
				
// iterator to first element
template <typename T>
typename List<T>::iterator List<T>::begin()
{
	return iterator(head->next);
}

//iterator to first element const version
template <typename T>
typename List<T>::const_iterator List<T>::begin() const
{
	return const_iterator(head->next);
}
				
// end marker iterator
template <typename T>
typename List<T>::iterator List<T>::end()
{
	return iterator(tail);
}

// end marker iterator const version
template <typename T>
typename List<T>::const_iterator List<T>::end() const
{
	return const_iterator(tail);
}
				
// insert val ahead of itr
template <typename T>
typename List<T>::iterator List<T>::insert(iterator itr, const T& val)
{
	++theSize;
	Node *itrNode=itr.current;
	Node *tempNode=new Node;
	tempNode->data=val;
	tempNode->prev=itrNode->prev;
	tempNode->next=itrNode;
	itrNode->prev->next=tempNode;
	itrNode->prev=tempNode;
	return iterator(itrNode->prev);
}
				
// move version of insert
template <typename T>
typename List<T>::iterator List<T>::insert(iterator itr, T && val)
{
	++theSize;
	Node *itrNode=itr.current;
	Node *tempNode=new Node;
	tempNode->data=std::move(val);
	tempNode->prev=itrNode->prev;
	tempNode->next=itrNode;
	itrNode->prev->next=tempNode;
	itrNode->prev=tempNode;
	return iterator(itrNode->prev);
}
				
// erase one element
template <typename T>
typename List<T>::iterator List<T>::erase(iterator itr)
{
	--theSize;
	Node *itrNode=itr.current;
	iterator ret(itrNode->next);
	itrNode->prev->next=itrNode->next;
	itrNode->next->prev=itrNode->prev;
	delete itrNode;
	return ret;
}
				
// erase [start, end)
template <typename T>
typename List<T>::iterator List<T>::erase(iterator start, iterator end)
{
	for(auto itr=start; itr!=end; )
		itr=erase(itr);
	return end;
}
				
// initialization
template <typename T>
void List<T>::init()
{
	theSize=0;
	head= new Node;
	tail= new Node;
	head->next=tail;
	tail->prev=head;
}


// overloading comparison operators
//equal to
template <typename T>
bool operator==(const List<T> & lhs, const List<T> &rhs)
{
    if(lhs.size()!=rhs.size())
		return false;
    auto ritr=rhs.begin();
	for(auto litr=lhs.begin(); litr!=lhs.end(); ++litr)
	{
		if(*litr!=*ritr)
			return false;
		++ritr;
	}
    return true;
}

//not equal to
template <typename T>
bool operator!=(const List<T> & lhs, const List<T> &rhs)
{
	return !(lhs==rhs);
}

// overloading output operator
template <typename T>
std::ostream & operator<<(std::ostream &os, const List<T> &l)
{
	l.print(os);
	return os;
}


//-------------------end of file List.hpp----------------------