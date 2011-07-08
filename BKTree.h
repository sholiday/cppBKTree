#ifndef BKTREE_H
#define BKTREE_H

/*
    BKTree.h

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
#include "BKNode.h"

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/map.hpp>

template <typename T>
class BKTree {
    private:
        friend class boost::serialization::access;
        template<class Archive>
        void serialize(Archive & ar, const unsigned int version);
    
    protected:
        BKNode<T> *root;
        int node_count;
    public:
        BKTree();
        BKTree(BKNode<T> *, int);
        ~BKTree();
    
        void insert(const T &);
        std::vector<T> find(const T &, const int) const;
        int size() const;
};

template <typename T>
template<class Archive>
void BKTree<T>::serialize(Archive & ar, const unsigned int version)
{
    ar & node_count;
    ar & root;
}

template <typename T>
BKTree<T>::BKTree(BKNode<T> * r, int nc) {
    root=r;
    node_count=nc;
}

template <typename T>
BKTree<T>::BKTree() {
    root=0;
    node_count=0;
}

template <typename T>
BKTree<T>::~BKTree() {
    delete root;
}

template <typename T>
void BKTree<T>::insert(const T &value) {
    node_count++;
    if (root==0) {
        root=new BKNode<T>(value);
    } else {
        root->insert(value);
    }
}

template <typename T>
std::vector<T> BKTree<T>::find(const T &rhs, const int threshold) const {
    if (root==0) {
        return std::vector<T>();
    } else {
        return root->find(rhs,threshold);
    }
}

template <typename T>
int BKTree<T>::size() const {
    return node_count;
}

#endif