/*
    load_and_search.cpp

    By Stephen Holiday 2011
    http://stephenholiday.com
    (Exception, Distance Algorithm by Anders Sewerin Johansen)

    The code is under the [Apache 2.0](http://www.apache.org/licenses/LICENSE-2.0) license.

    This is a utility for loading a BKTree from a serialized file.
    
    ./bkSearch [serialized tree] [word] [max edxit distance]
    ./bkSearch bkTree.dat word 3

*/

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <fstream>
#include <iostream>
#include <cstdlib> 
#include <cstring>

#include "BKTree.h"

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

int main(int argc, char* argv[]) { 
    BKTree<std::string> tree;
    {
        std::ifstream iff(argv[1]);
        boost::archive::text_iarchive iar(iff);
        
        iar >> tree;
    }
    
    
    std::vector<std::string> results;
    results=tree.find(std::string(argv[2]),atoi(argv[3]));
    

    std::vector<std::string>::iterator it;

    for ( it=results.begin() ; it < results.end(); it++ )
        std::cout << *it << std::endl;
        
    return 0;
}