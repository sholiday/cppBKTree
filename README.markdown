## cppBKTree ##

By [Stephen Holiday](http://stephenholiday.com) 2011

(Exception, Distance Algorithm by Anders Sewerin Johansen)

The code is under the [Apache 2.0](http://www.apache.org/licenses/LICENSE-2.0) license.

This a C++ implementation of a BKTree (Burkhard-Keller).
Essentially it allows searching for an object in a metric space.

One common use is for fuzzy string matching, like in a spell checker.
The search is performed by looking at the distance of the test with the current
node and moving according to the distance of the test to the children.

This technique is faster than brute force as it does not need to look at every
possible node in the space.

Implemented according to [this post](http://blog.notdot.net/2007/4/Damn-Cool-Algorithms-Part-1-BK-Trees).

I also wrote a [Python implementation](https://github.com/sholiday/pyBKTree).

### Tools ###

Aside from the data structure itself, I wrote two tools to play with BKTrees.

Too make the tools just run:
    make tools
    
This assumes that you have the boost serialization libraries [installed and compiled]((http://www.boost.org/doc/libs/release/more/getting_started/index.html).


#### bkLoad ####
This is a utility for creating a BKTree from a text file and saving to disk.
The input files should have one entry per line.

    ./bkLoad [from] [to]
    ./bkLoad /usr/share/dict/words wordTree.dat

#### bkSearch ####
This is a utility for loading a BKTree from a serialized file.

    ./bkSearch [serialized tree] [word] [max edxit distance]
    ./bkSearch bkTree.dat word 3
    
### Comments/Pull Requests ###
If you think something could be improved, I'm happy to accept pull requests.