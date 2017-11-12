all: proj5.x sproj5.x

proj5.x :proj5.cpp passserver.cpp passserver.h hashtable.h hashtable.hpp
	g++ -std=c++11 -lcrypt -o proj5.x proj5.cpp passserver.cpp
sproj5.x :sproj5.cpp passserver.cpp passserver.h hashtable.h hashtable.hpp	
	g++ -std=c++11 -lcrypt -o sproj5.x sproj5.cpp passserver.cpp 
	
clean:
	rm proj5t.x sproj5t.x