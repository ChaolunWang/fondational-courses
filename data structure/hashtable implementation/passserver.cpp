//----------------------passserver.cpp---------------------------
/*This is the implementation part of the class PassServer, in which
  a simple password server program is simulated. The server used
  the hash table I developed in HashTable class.
  programmed by: Chaolun Wang
             at: 11/10/2015
*/

#include "passserver.h"

PassServer::PassServer(size_t size): mHash{size<2 ? 11 : size}                                  // constructor, create a hash table of the specified size. 
{
    if(size<2)
		std::cout<<"** input too small\nset to default capacity\n";	
}

PassServer::~PassServer()                                                                        // destructor. 
{	
}

bool PassServer::load(const char *filename)                                                     // load a password file into the HashTable object. Each line contains a pair of username and encrypted password.
{
	return mHash.load(filename);
}

bool PassServer::addUser(std::pair<string,  string> & kv)                                        // add a new username and password.  The password passed in is in plaintext, it should be encrypted before insertion.
{
	auto tempPair=kv;
	tempPair.second=encrypt(tempPair.second);
	return mHash.insert(std::move(tempPair));
}

bool PassServer::addUser(std::pair<string, string> && kv)                                        // move version of addUser.
{
	kv.second=encrypt(kv.second);
	return mHash.insert(std::move(kv));
}

bool PassServer::removeUser(const string & k)                                                     // delete an existing user with username k.
{
	return mHash.remove(k);
}

bool PassServer::changePassword(const pair<string, string> &p, const string & newpassword)        // change an existing user's password. Note that both passwords passed in are in plaintext.
{
	if(!mHash.match(make_pair(p.first, encrypt(p.second))))
		return false;
	return addUser(make_pair(p.first, newpassword));
}

bool PassServer::find(const string & user) const                                                   // check if a user exists (if user is in the hash table).
{
	return mHash.contains(user);
}

void PassServer::dump() const                                                                      // show the structure and contents of the HashTable object to the screen. Same format as the dump() function in the HashTable class template.
{
	mHash.dump();
}

size_t PassServer::size() const                                                                    // return the size of the HashTable (the number of username/password pairs in the table).
{
	return mHash.getSize();
}

size_t PassServer::capacity() const                                                               // return the capacity of the HashTable 
{
	return mHash.getCapacity();
}

bool PassServer::write_to_file(const char *filename) const                                         // save the username and password combination into a file. Same format as the write_to_file() function in the HashTable class template.
{
	return mHash.write_to_file(filename);
}

string PassServer::encrypt(const string & str) const                                              //encrypt the parameter str and return the encrypted string.
{
	char salt[] = "$1$########";
	char password [100];
    strcpy ( password, crypt(str.c_str(), salt));
	char truncate [50];
	for(int i=0; i<23; ++i)
		truncate[i]=password[i+12];
	return truncate;
}


//end of file passserver.cpp