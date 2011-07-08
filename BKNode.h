#ifndef BKNODE_H
#define BKNODE_H
/*
    BKNode.h

    By Stephen Holiday 2011
    http://stephenholiday.com
    (Exception, Distance Algorithm by Anders Sewerin Johansen)

    The code is under the [Apache 2.0](http://www.apache.org/licenses/LICENSE-2.0) license.

    This a C++ implementation of a BKTree (Burkhard-Keller).
    Essentially it allows searching for an object in a metric space.

    One common use is for fuzzy string matching, like in a spell checker.
    The search is performed by looking at the distance of the test with the current
    node and moving according to the distance of the test to the children.

    This technique is faster than brute force as it does not need to look at every
    possible node in the space.

    Implemented according to this post:
    http://blog.notdot.net/2007/4/Damn-Cool-Algorithms-Part-1-BK-Trees

*/

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <fstream>
#include <iostream>

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/map.hpp>

// Functor for deleting pointers in map.
template<class A, class B>
struct DeleteMapFntor
{
    // Overloaded () operator.
    // This will be called by for_each() function.
    bool operator()(std::pair<A,B> x) const
    {
        // Assuming the second item of map is to be
        // deleted. Change as you wish.
        delete x.second;
        return true;
    }
};

template <typename T>
class BKNode {
    private:
        friend class boost::serialization::access;
        template<class Archive>
        void serialize(Archive & ar, const unsigned int version);
        
    protected:
        T value;
        std::map<int,BKNode<T>*> children;
    public:
        BKNode(const T &);
        BKNode();
        BKNode(T, std::map<int,BKNode<T> >);
        ~BKNode();
        
        // Accessors
        T get() const;

        int distance(const BKNode<std::string> &) const;
        std::vector<T> find(const T &, const int);
        
        // Mutators
        void insert(const T &);
        
    template <typename TT>
    friend std::ostream &operator << ( std::ostream &, const BKNode<TT> & );
    
};

template <typename T>
template<class Archive>
void BKNode<T>::serialize(Archive & ar, const unsigned int version)
{
    ar & value;
    ar & children;
}

template <typename T>
BKNode<T>::BKNode() {
}

template <typename T>
BKNode<T>::BKNode(const T &newValue) {
    value=newValue;
    children=std::map<int,BKNode<T>*>();
}

template <typename T>
BKNode<T>::BKNode(T v, std::map<int,BKNode<T> > c) {
    value=v;
    children=c;
}

template <typename T>
BKNode<T>::~BKNode() {
    for_each( children.begin(),children.end(),DeleteMapFntor<int,BKNode<T>*>());
}

template <typename T>
T BKNode<T>::get() const{
    return value;
}

template <typename T>
void BKNode<T>::insert(const T &rhs) {
    int dist = distance(rhs);
    
    // If the distance is zero, they're the same objects so don't insert
    if (dist!=0) {
        
        // If there is already a child with the same distance, insert it on that child
        if (children.count(dist)) {
            children[dist]->insert(rhs);
        } else {
            BKNode<T> *new_node = new BKNode<T>(rhs);
            children[dist]=new_node;
        }
    }
}


template <typename T>
std::vector<T> BKNode<T>::find(const T &rhs, const int threshold) {
    int dist = distance(rhs);
    
    std::vector<T> results=std::vector<T>();
    // If the current node is in the threshold, return it
    if (dist<=threshold)
        results.push_back(value);
    
    int dmin=dist-threshold;
    int dmax=dist+threshold;
    
    for (int i=dmin; i<=dmax; i++) {
        if (children.count(i)) {
            std::vector<T> partial= std::vector<T>(children[i]->find(rhs,threshold));
            
            results.insert(results.end(), partial.begin(), partial.end());
        }
    }
    
    return results;
}

template <>
int BKNode<std::string>::distance(const BKNode<std::string>& rhs) const{
    std::string source=value;
    std::string target=rhs.value;
  
    /*
        From http://www.merriampark.com/ldcpp.htm
        By Anders Sewerin Johansen
    */
      // Step 1

    const int n = source.length();
    const int m = target.length();
    if (n == 0) {
        return m;
    }
    if (m == 0) {
        return n;
    }

    // Good form to declare a TYPEDEF

    typedef std::vector< std::vector<int> > Tmatrix; 

    Tmatrix matrix(n+1);

    // Size the vectors in the 2.nd dimension. Unfortunately C++ doesn't
    // allow for allocation on declaration of 2.nd dimension of vec of vec

    for (int i = 0; i <= n; i++) {
        matrix[i].resize(m+1);
    }

    // Step 2

    for (int i = 0; i <= n; i++) {
        matrix[i][0]=i;
    }

    for (int j = 0; j <= m; j++) {
        matrix[0][j]=j;
    }

    // Step 3

    for (int i = 1; i <= n; i++) {

        const char s_i = source[i-1];

        // Step 4

        for (int j = 1; j <= m; j++) {

            const char t_j = target[j-1];

            // Step 5

            int cost;
            if (s_i == t_j) {
                cost = 0;
            } else {
                cost = 1;
            }

            // Step 6

            const int above = matrix[i-1][j];
            const int left = matrix[i][j-1];
            const int diag = matrix[i-1][j-1];
            int cell = std::min( above + 1, std::min(left + 1, diag + cost));

            // Step 6A: Cover transposition, in addition to deletion,
            // insertion and substitution. This step is taken from:
            // Berghel, Hal ; Roach, David : "An Extension of Ukkonen's 
            // Enhanced Dynamic Programming ASM Algorithm"
            // (http://www.acm.org/~hlb/publications/asm/asm.html)

            if (i>2 && j>2) {
                int trans=matrix[i-2][j-2]+1;
                if (source[i-2]!=t_j) trans++;
                if (s_i!=target[j-2]) trans++;
                if (cell>trans) cell=trans;
            }

            matrix[i][j]=cell;
        }
    }

    // Step 7

    return matrix[n][m];
}

template <typename TT>
std::ostream &operator << ( std::ostream &out, const BKNode<TT> &node ) {
    out << "BKNode<this: " << node.value << " >";
    
    return out;
}

#endif