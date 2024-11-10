#ifndef BINTREE_H
#define BINTREE_H

template <typename T>
class BinTree {
public:
    T data;
    BinTree* left;
    BinTree* right;

    BinTree(T val) : data(val), left(nullptr), right(nullptr) {}
    ~BinTree() {
        delete left;
        delete right;
    }

    bool isLeaf() const {
        return !left && !right;
    }
};

#endif
