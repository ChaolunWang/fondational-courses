//----------------------passserver.h---------------------------
/*This is the declearation part of the class PassServer, in which
  a simple password server program is simulated. The server used
  the hash table I developed in HashTable class.
  programmed by: Chaolun Wang
             at: 11/10/2015
*/

#ifndef PASSSERVER_H
#define PASSSERVER_H

#include "hashtable.h"
#include <unistd.h>
#include <crypt.h>
#include <cstring>

using namespace std;

class PassServer
{
public:
    PassServer(size_t size = 101);                                                   // constructor, create a hash table of the specified size. 
    ~PassServer();                                                                   // destructor. 
    bool load(const char *filename);                                                 // load a password file into the HashTable object. Each line contains a pair of username and encrypted password.
    bool addUser(std::pair<string,  string> & kv);                                   // add a new username and password.  The password passed in is in plaintext, it should be encrypted before insertion.
    bool addUser(std::pair<string, string> && kv);                                   // move version of addUser.
    bool removeUser(const string & k);                                               // delete an existing user with username k.
    bool changePassword(const pair<string, string> &p, const string & newpassword);  // change an existing user's password. Note that both passwords passed in are in plaintext.
    bool find(const string & user) const;                                            // check if a user exists (if user is in the hash table).
    void dump() const;                                                               // show the structure and contents of the HashTable object to the screen. Same format as the dump() function in the HashTable class template.
    size_t size() const;                                                             // return the size of the HashTable (the number of username/password pairs in the table).
	size_t capacity() const;                                                         // return the capacity of the HashTable
    bool write_to_file(const char *filename) const;                                  // save the username and password combination into a file. Same format as the write_to_file() function in the HashTable class template.
private:
    cop4530::HashTable<string, string> mHash;                                        //member data stored in hash table
	string encrypt(const string & str) const;                                        //encrypt the parameter str and return the encrypted string.
};

#endif

//end of file passserver.h