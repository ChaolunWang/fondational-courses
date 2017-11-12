//----------------------------------------hashtable.h-------------------------------------
/*This file is a declearation part of the class template HashTable, which stored a hash table
  by the chaining with separate list. 
  Programmed by: Chaolun Wang
             at: 11/10/2015
*/

#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <utility>
#include <vector>
#include <iostream>
#include <list>
#include <fstream>
#include <algorithm>
#include <string>

namespace cop4530 
{
	static const unsigned int max_prime = 1301081;                   // max_prime is used by the helpful functions provided
    static const unsigned int default_capacity = 11;                 // the default_capacity is used if the initial capacity of the underlying vector of the hash table is zero. 
	
	template <typename K, typename V>
	class HashTable
	{
	public:
		explicit HashTable(size_t size = 101);                      // constructor. Create a hash table, where the size of the vector is set to prime_below(size) (where size is default  to 101), where prime_below() is a private member function of the HashTable and provided to you.
        ~HashTable();                                               // destructor. Delete all elements in hash table.
        bool contains(const K & k) const;                           // check if key k is in the hash table.
        bool insert(const std::pair<K, V> & kv);                    // add  the key-value pair kv into the hash table. If the key is the hash table but with a different value, the value should be updated to the new one with kv. Return true if kv is inserted or the value is updated,false otherwise.
        bool insert (std::pair<K,  V> && kv);                       // move version of insert.
        bool remove(const K & k);                                   // delete the key k and the corresponding value if it is in the hash table. Return true if k is deleted, return false otherwise (i.e., if key k is not in the hash table).
        void clear();                                               // delete all elements in the hash table
        bool load(const char *filename);                            // load the content of the file with name filename into the hash table. In the file, each line contains a single pair of key and value, separated by a white space.
        void dump() const;                                          //display all entries in the hash table. If an entry contains multiple key-value pairs, separate them by a semicolon character (:) 
        bool write_to_file(const char *filename) const;             //write all elements in the hash table into a file with name filename.
	    size_t getSize() const;                                     //get the size of Hash table
		size_t getCapacity() const;                                 //get the capacity of Hash table
		bool match(const std::pair<K, V> &kv) const;                // check if key-value pair is in the hash table

	private:
	    std::vector< std::list< std::pair<K,V> > > mList;           // memberdata container
		size_t mSize;                                               // number of elements in hash table
	    void makeEmpty();                                           // delete all elements in the hash table. 
        void rehash();                                              // Rehash function. Called when the number of elements in the hash table is greater than the size of the vector.
        size_t myhash(const K &x) const;                            // return the index of the vector entry where x should be stored.
        unsigned long prime_below (unsigned long) const;            //  function to determine the proper prime numbers used in setting up the vector size
		void setPrimes(std::vector<unsigned long>&) const;          //  function to determine the proper prime numbers used in setting up the vector size
	};
	
	
	#include "hashtable.hpp"
}//end of namespace cop4530

#endif

//end of hashtable.h
