/*
    load_and_search.cpp

    By Stephen Holiday 2011
    http://stephenholiday.com
    (Exception, Distance Algorithm by Anders Sewerin Johansen)

    The code is under the [Apache 2.0](http://www.apache.org/licenses/LICENSE-2.0) license.

    This is a utility for creating a BKTree from a text file and saving to disk.
    The input files should have one entry per line.
    
    ./bkLoad [from] [to]
    ./bkLoad /usr/share/dict/words wordTree.dat

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
    
    BKTree<std::string> tree=BKTree<std::string>();
    
    std::ifstream words (argv[1]);
    std::string str;
    
    std::ofstream ofs(argv[2]);
    
    
    if ( !words.is_open() ) {
      // The file could not be opened
    }
    else {
        while(std::getline(words, str))
           {
               tree.insert(str);
           }
    }
    
    std::cout<<"Loaded "<<tree.size()<< " entries"<<std::endl;
    
    {
        boost::archive::text_oarchive oa(ofs);
        oa<<tree;
    }
        
    return 0;
}

