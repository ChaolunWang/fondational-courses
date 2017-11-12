//---------------proj5.cpp------------------------------------
/*This is the driver program of PassServer class, in which
  a simple password server program is simulated. The main function
  can provide an interface that prompt the user for the next choice 
  from the menu and exit the program when the user selection the 
  exit "x" option.
  programmed by: Chaolun Wang
             at: 11/10/2015
*/

#include "passserver.h"
using namespace std;

void Menu()
{
  cout << "\n\n";
  cout << "l - Load From File" << endl;
  cout << "a - Add User" << endl;
  cout << "r - Remove User" << endl;
  cout << "c - Change User Password" << endl;
  cout << "f - Find User" << endl;
  cout << "d - Dump HashTable" << endl;
  cout << "s - HashTable Size" << endl;
  cout << "w - Write to Password File" << endl;
  cout << "x - Exit program" << endl;
  cout << "\nEnter choice : ";
}

int main()
{
	int capacity;
	cout<<"Enter preferred hash table capacity: ";
	cin>>capacity;
	PassServer myServer(capacity);
	cout<<myServer.capacity()<<'\n';
    string option;
    do 
    { 
        Menu();
        cin >> option;  
     
        if(option == "l")                                                      // Checking if user selected option l
        {
			string fileName;
            cout<<"Enter password file name to load from: ";
			cin>>fileName;
			if(!myServer.load(fileName.c_str()))
				cerr<<"Error: Cannot open file df\n";
			else
				cout<<'\n';
        }
        else if(option == "a")                                                 // Checking if user selected option a
        {
            string userName;
			string passWord;
			cout<<"Enter username: ";
			cin>>userName;
            cout<<"Enter password: ";
            cin>>passWord;
            if(myServer.addUser(make_pair(userName, passWord)))
				cout<<"\nUser "<<userName<<" added.\n";	
            else
                cout<<"*****Error: User already exists. Could not add user.\n";				
        }
        else if(option == "r")                                                 // Checking if user selected option r
        {
            string userName;
			cout<<"Enter username: ";
			cin>>userName;
            if(myServer.removeUser(userName))
				cout<<"User "<<userName<<" deleted.\n";	
            else
                cout<<"*****Error: User not found.  Could not delete user\n";		
        }
	    else if(option == "c")                                                 // Checking if user selected option c
        {
            string userName;
			string passWord;
			string newPassWord;
			cout<<"Enter username    : ";
			cin>>userName;
            cout<<"Enter password    : ";
            cin>>passWord;
            cout<<"Enter new password: ";
            cin>>newPassWord;
            if(myServer.changePassword(make_pair(userName, passWord), newPassWord))
				cout<<"\nPassword changed for user "<<userName<<"\n";	
            else
                cout<<"\n*****Error: Could not change user password\n";		
        }
        else if(option == "f")                                                 // Checking if user selected option f
        {
            string userName;
			cout<<"Enter username    : ";
			cin>>userName;
            if(myServer.find(userName))
				cout<<"\nUser '"<<userName<<"' found. \n";	
            else
                cout<<"\nUser '"<<userName<<"' not found. \n";	
        }	 
        else if(option == "d")                                                 // Checking if user selected option d
        {
            myServer.dump();
        }
        else if(option == "s")                                                 // Checking if user selected option s
        {
            cout<<"Size of hashtable: "<<myServer.size()<<'\n';
        }
        else if(option == "w")                                                 // Checking if user selected option w
        {
			string fileName;
            cout<<"Enter password file name to write to: ";
			cin>>fileName;
			if(!myServer.write_to_file(fileName.c_str()))
				cerr<<"Error: Cannot write to file df\n";           
        }
        else if(option == "x")                                                 // Checking if user selected option x
        {
        }
        else                                                                   //if user has entered invalid choice 
        {
          //Displaying error message
          cerr << "*****Error: Invalid entry.  Try again.\n" << endl;
        }
    }while(option != "x");
}
