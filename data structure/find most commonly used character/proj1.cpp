//------------------------------proj1.cpp---------------------------
/* In this project, I build an analyer to find out the most commonly used character, words
   and numbers in a file. The program can read a character at a time and store the character 
   words or number into map container. After that, the top ten are selected based on the 
   given criteria.
   
   Programmer: Chaolun Wang */
   
   
#include <iostream>
#include <string>
#include <cctype>
#include <map>
#include <utility>
#include <vector>
#include <iomanip>

using namespace std;

void storeMap(map<string, pair<int, int> > &map, string &key, int creterial);                                //store the input character, word and number in map as strings
void topList(map<string, pair<int, int> > &map, vector<pair<string, int> > &list);                           //generate the list including top ten enries and store them in pair(name, number)
void printWord(string word);                                                                                 //print each string seperately
void print(const map<string, pair<int, int> > &map, const vector<pair<string, int> > &list, string ch);      //print out the enturies in top list         

int main()
{
    map<string, pair<int, int> > charMap, wordMap, numMap;
    vector<pair<string, int> > charVec, wordVec, numVec;
    int counter=0;
    string word="", number="", character="";
    word.reserve(25);
    number.reserve(25);
    character.reserve(1);
    
    for(char ch; cin.get(ch); )
    {
		++counter;	
		character+=ch;
		if(isdigit(ch))                                                                                      //this loop read a character at a time and then link them into seperate word and number, then store them
		{
			if(word.size())
			    storeMap(wordMap, word, counter);
			number+=ch;
		}
		else if(isalpha(ch))
		{
			if(number.size())
			    storeMap(numMap, number, counter);
			word+=tolower(ch);
		}
		else
		{
			if(word.size())
			    storeMap(wordMap, word, counter);
			else if(number.size())
			    storeMap(numMap, number, counter);
		}
		storeMap(charMap, character, ch);
	}
	if(word.size())
	    storeMap(wordMap, word, counter);
	else if(number.size())
	    storeMap(numMap, number, counter);			
    
    topList(charMap, charVec);
    topList(wordMap, wordVec);
    topList(numMap, numVec);
 
    print(charMap, charVec, "characters");
    print(wordMap, wordVec, "words");
    print(numMap, numVec, "numbers");
    
	return 0;
}

void storeMap(map<string, pair<int, int> > &map, string &key, int creterial)
{
	if(map.find(key)==map.end())
	    map[key]=make_pair(creterial, 1);
	else
	    ++map[key].second;
	key="";
}

void topList(map<string, pair<int, int> > &map, vector<pair<string, int> > &list)
{
    int n=0;
    int value, tempValue;
    std::map<string, pair<int, int> >::iterator temp;
    
    while(map.size() && n<10)
    {
		value=0;
		temp=map.begin();

		for(std::map<string, pair<int, int> >::iterator it=map.begin(); it!=map.end(); ++it)
		{
			tempValue=it->second.second;
		    if(tempValue>value || 
		       (tempValue==value && (it->second.first < temp->second.first)))
		    {
				value=tempValue;
				temp=it;
			}
		}
		list.push_back(make_pair(temp->first, value));
		map.erase(temp);	
		++n;
	}
}
	
void printWord(string word)
{
	if(word[0]==10)
	    cout<<setw(17)<<left<<"\\n";
	else if(word[0]==9)
	    cout<<setw(17)<<left<<"\\t";
	else if(word.size()>10)
	    cout<<setw(25)<<left<<word;
	else
	    cout<<setw(17)<<left<<word;
}
		
void print(const map<string, pair<int, int> > &map, const vector<pair<string, int> > &list, string ch)
{
	cout<<"Total "<<map.size()+list.size()<<" different "<<ch<<", "<<list.size()<<" most used "<<ch<<":\n";
	for(int i=0; i<list.size(); ++i)
	{
		cout<<"No. "<<i<<": ";
		printWord(list[i].first);
		cout<<list[i].second<<'\n';
	}
	cout<<'\n';
}				

//end of program
		
		
		
