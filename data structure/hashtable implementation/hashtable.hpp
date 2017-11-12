//----------------------------------------hashtable.h-------------------------------------
/*This file is the implementation part of the class template HashTable, which stored a hash table
  by the chaining with separate list. 
  Programmed by: Chaolun Wang
             at: 11/10/2015
*/


template <typename K, typename V>
HashTable<K, V>::HashTable(size_t size) : mList{prime_below(size)}, mSize{0}                  // constructor. Create a hash table, where the size of the vector is set to prime_below(size) (where size is default  to 101), where prime_below() is a private member function of the HashTable and provided to you.
{	

}

template <typename K, typename V>
HashTable<K, V>::~HashTable()                                                   // destructor. Delete all elements in hash table.
{
    clear();	
}

template <typename K, typename V>
bool HashTable<K, V>::contains(const K & k) const                                 // check if key k is in the hash table.
{
	auto tempList=mList[myhash(k)];
	for(auto& tempPair: tempList)
	{
	    if(tempPair.first==k)
			return true;
	}
	return false;
}

template <typename K, typename V>
bool HashTable<K, V>::match(const std::pair<K, V> &kv) const
{
	auto tempList=mList[myhash(kv.first)];
	for(auto& tempPair: tempList)
	{
	    if(tempPair==kv)
			return true;
	}
	return false;
}

template <typename K, typename V>
bool HashTable<K, V>::insert(const std::pair<K, V> & kv)                        // add  the key-value pair kv into the hash table. If the key is the hash table but with a different value, the value should be updated to the new one with kv. Return true if kv is inserted or the value is updated,false otherwise.
{
	auto& tempList=mList[myhash(kv.first)];
	for(auto& tempPair: tempList)
	{
		if(tempPair.first==kv.first && tempPair.second==kv.second)
			return false;
		else if(tempPair.first==kv.first)
		{
			tempPair.second=kv.second;
			return true;
		}
	}
	tempList.push_back(kv);
	if(++mSize>mList.size())
		rehash();
	return true;
}

template <typename K, typename V>
bool HashTable<K, V>::insert (std::pair<K,  V> && kv)                           // move version of insert.
{
	auto& tempList=mList[myhash(kv.first)];
	for(auto& tempPair: tempList)
	{
		if(tempPair.first==kv.first && tempPair.second==kv.second)
			return false;
		else if(tempPair.first==kv.first)
		{
			std::swap(tempPair, kv);
			return true;
		}
	}
	tempList.push_back(std::move(kv));
	if(++mSize>mList.size())
		rehash();
	return true;
}

template <typename K, typename V>
bool HashTable<K, V>::remove(const K & k)                                       // delete the key k and the corresponding value if it is in the hash table. Return true if k is deleted, return false otherwise (i.e., if key k is not in the hash table).
{
	auto& tempList=mList[myhash(k)];
	for(auto itr=tempList.begin(); itr!=tempList.end(); ++itr)
	{
	    if(itr->first==k)
		{
			tempList.erase(itr);
			--mSize;
			return true;
		}
	}
	return false;
}

template <typename K, typename V>
void HashTable<K, V>::clear()                                                   // delete all elements in the hash table
{
    makeEmpty();
}

template <typename K, typename V>
bool HashTable<K, V>::load(const char *filename)                                // load the content of the file with name filename into the hash table. In the file, each line contains a single pair of key and value, separated by a white space.
{
	std::ifstream input;
	input.open(filename);
	if(!input)
		return false;
	std::string name, passWord;
	while(input>>name>>passWord)
	{
		insert(make_pair(name, passWord));
	}
	input.close();
	return true;
}

template <typename K, typename V>
void HashTable<K, V>::dump() const                                                //display all entries in the hash table. If an entry contains multiple key-value pairs, separate them by a semicolon character (:) 
{
	int counter=0;
	for(auto& tempList: mList)
	{
		std::cout<<"v["<<counter<<"]: ";
		for(auto& tempPair: tempList)
	    {
			std::cout<<tempPair.first<<' '<<tempPair.second;
			if(tempPair!=tempList.back())
				std::cout<<':';
		}
		++counter;
		std::cout<<'\n';
	}
}

template <typename K, typename V>
bool HashTable<K, V>::write_to_file(const char *filename) const                   //write all elements in the hash table into a file with name filename.
{
	std::ofstream output;
	output.open(filename);
	if(!output)
		return false;
	for(auto& tempList: mList)
	{
		for(auto& tempPair: tempList)
		    output<<tempPair.first<<' '<<tempPair.second<<'\n';
	}
	output.close();
	return true;
}

template <typename K, typename V>
size_t HashTable<K, V>::getSize() const                                        //get the size of hash table
{
	return mSize;
} 

template <typename K, typename V>
size_t HashTable<K, V>::getCapacity() const                                     //get the capacity of Hash table
{
	return mList.size();
}

template <typename K, typename V>
void HashTable<K, V>::makeEmpty()                                               // delete all elements in the hash table. 
{
	mSize=0;
	for(auto& tempList: mList)
	{
		tempList.clear();
	}
}

template <typename K, typename V>
void HashTable<K, V>::rehash()                                                  // Rehash function. Called when the number of elements in the hash table is greater than the size of the vector.
{
	auto oldList=mList;
	makeEmpty();
	mList.resize(prime_below(2*mList.size()));
	mSize=0;
	for(auto& tempList: oldList)
		for(auto& tempPair: tempList)
			insert(std::move(tempPair));
}

template <typename K, typename V>
size_t HashTable<K, V>::myhash(const K &x) const                                  // return the index of the vector entry where x should be stored.
{
	static std::hash<K> hf;
	return hf(x)%mList.size();
}

template <typename K, typename V>
unsigned long HashTable<K, V>::prime_below (unsigned long n) const               //  function to determine the proper prime numbers used in setting up the vector size
{
  if (n > max_prime)
    {
      std::cerr << "** input too large for prime_below()\n";
      return 0;
    }
  if (n == max_prime)
    {
      return max_prime;
    }
  if (n <= 1)
    {
		std::cerr << "** input too small \n";
      return 0;
    }

  // now: 2 <= n < max_prime
  std::vector <unsigned long> v (n+1);
  setPrimes(v);
  while (n > 2)
    {
      if (v[n] == 1)
	return n;
      --n;
    }

  return 2;
}

 
template <typename K, typename V>
void HashTable<K, V>::setPrimes(std::vector<unsigned long>& vprimes) const              //  function to determine the proper prime numbers used in setting up the vector size
{
  int i = 0;
  int j = 0;

  vprimes[0] = 0;
  vprimes[1] = 0;
  int n = vprimes.capacity();

  for (i = 2; i < n; ++i)
    vprimes[i] = 1;

  for( i = 2; i*i < n; ++i)
    {
      if (vprimes[i] == 1)
        for(j = i + i ; j < n; j += i)
          vprimes[j] = 0;
    }
}

