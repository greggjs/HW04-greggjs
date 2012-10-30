//
//  greggjsStarbucks.h
//  HW04
//
//  Created by Jake Gregg on 10/29/12.
//
//

#ifndef __HW04__greggjsStarbucks__
#define __HW04__greggjsStarbucks__

#include <iostream>
#include "Starbucks.h"
#include <fstream.h>
#include <vector.h>

#endif /* defined(__HW04__greggjsStarbucks__) */

class Node {
public:
    Entry* data_;
    Node* right_;
    Node* left_;
    
    Node () {this->data_=NULL;}
    ~Node() {
        delete this->data_;
        delete this->left_;
        delete this->right_;
    }
};

class greggjsStarbucks : public Starbucks {
public:
    Node* search_root_;
    greggjsStarbucks();
    ~greggjsStarbucks();
    
    int importData(Entry* import, string filename);
    virtual void build(Entry* c, int n);
    virtual Entry* getNearest(double x, double y);
    Entry* alwaysRightValue(Entry* c, int n, double x, double y);
    void printTree(Node* root);
    
    Node* insert(Entry* c, Node* root, bool xLevel);
    Node* search(double x, double y, Node* root, bool xLevel);
    static void shuffle(Entry* c, int n);
    
};