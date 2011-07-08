# compiler to use
CC = c++
CFLAGS=-O2

INC = -I/usr/local/include/boost
LIB=-L/usr/local/lib/
BOOSTLIBS=/usr/local/lib/libboost_serialization.a

all: clean tools
	
tools:
	$(CC) $(CFLAGS) $(INC) load_from_dictionary.cpp -o bkLoad $(LIB) $(BOOSTLIBS)
	$(CC) $(CFLAGS) $(INC) load_and_search.cpp -o bkSearch $(LIB) $(BOOSTLIBS)
	
clean:
	rm -f bkLoad bkSearch
	rm -Rf bkSearch.dSym bkLoad.dSYM
