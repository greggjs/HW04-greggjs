//
//  greggjsStarbucks.cpp
//  HW04
//
//  Created by Jake Gregg on 10/29/12.
//
//

#include "greggjsStarbucks.h"
#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"

using namespace std;
using namespace ci;
using namespace ci::app;

greggjsStarbucks::greggjsStarbucks() {
    this->search_root_ = new Node();
    this->search_root_->data_ = new Entry();
    this->search_root_->left_ = new Node();
    this->search_root_->right_ = new Node();
}

greggjsStarbucks::~greggjsStarbucks() {
    delete this->search_root_;
    search_root_ = NULL;
}

int greggjsStarbucks::importData(Entry* import, string filename) {
    try{
		ifstream fid (filename, ifstream::in);
        
		int index = 0;
		char* buffer = new char[256];
        
		while(!fid.eof()){
			Entry* temp = new Entry;
            
			fid.getline(buffer,256,',');
			temp->identifier = (string)buffer;
            
			fid.getline(buffer,256,',');
			temp->x  = atof(buffer); 
            
			fid.getline(buffer,256,'\r');
			temp->y  = atof(buffer);
            
			import[index] = *temp;
            
			//checking output for correctness:
			//console() << (locations->back())->identifier << ","<< (locations->back())->x << ","<< (locations->back())->y << std::endl ;
            
			index++;
		}
        
		fid.close();
        
		return index;
        
	}catch(ios_base::failure){
        
		cout << "Error Reading File\n";
		return NULL;
        
	}
}

void greggjsStarbucks::build(Entry *c, int n) {
    this->shuffle(c, n);
    this->search_root_->data_ = &c[0];
    for (int i = 1; i < n; i++) {
        Node* new_node = this->insert(&c[i], this->search_root_, true);
        new_node->data_ = &c[i];
        new_node->left_ = new Node();
        new_node->right_ = new Node();
    }
}

void greggjsStarbucks::shuffle(Entry *c, int n) {
    srand ( unsigned ( time (NULL) ) );
    int random;
    
    if (n>1) {
        for (int i = 0; i < n; ++i) {
            random = rand() % n;
            Entry temp = c[random];
            c[random] = c[i];
            c[i] = temp;
        }
    }
}

Node* greggjsStarbucks::insert(Entry *c, Node *root, bool xLevel) {
    if (root->data_ == NULL) return root;
    if (xLevel) {
        if (c->x > root->data_->x)
            return insert(c, root->right_, !xLevel);
        else
            return insert(c, root->left_, !xLevel);
    } else {
        if (c->y > root->data_->y)
            return insert(c, root->right_, !xLevel);
        else
            return insert(c, root->left_, !xLevel);
    }
}

void greggjsStarbucks::printTree(Node *root) {
    if (root->data_ == NULL) return;
    printTree(root->left_);
    console() << root->data_->identifier << "," << root->data_->x << "," << root->data_->y << endl;
    printTree(root->right_);
}

Entry* greggjsStarbucks::getNearest(double x, double y) {
    Node* nearest = search(x, y, this->search_root_, true);
    return nearest->data_;
}

Node* greggjsStarbucks::search(double x, double y, Node *root, bool xLevel) {
    if(root->data_ == NULL)
		return NULL;
    
	double parentDistance = sqrt((root->data_->x - x)*(root->data_->x - x) + (root->data_->y - y)*(root->data_->y - y));
    
	if(parentDistance < .0000001)
		return root;
    
	bool rightSide = false;
	Node* candidate = NULL;
	Node* otherPath = NULL;
    
    
	if(xLevel){
		if(x > root->data_->x){
			candidate = search(x, y, root->right_, !xLevel);
			otherPath = root->left_;
		}else{
			candidate = search(x, y, root->left_, !xLevel);
			otherPath = root->right_;
		}
	}else{
		if(y > root->data_->y){
			candidate = search(x, y, root->right_, !xLevel);
			otherPath = root->left_;
		}else{
			candidate = search(x, y, root->left_, !xLevel);
			otherPath = root->right_;
		}
	}
    
    /// if the candidate is NULL....
	if(candidate == NULL){
		candidate = root;
		Node* twinCandidate = search(x, y, otherPath, !xLevel);
        
		if(twinCandidate != NULL){
			double twinDistance = sqrt((twinCandidate->data_->x - x)*(twinCandidate->data_->x - x)
                                       + (twinCandidate->data_->y - y)*(twinCandidate->data_->y - y));
            
			if(twinDistance < parentDistance)
				candidate = twinCandidate;
		}
	}
    /// if the candidate is not NULL....
	else{
		double candidateDistance = sqrt((candidate->data_->x - x)*(candidate->data_->x - x)
                                        + (candidate->data_->y - y)*(candidate->data_->y - y));
        
        /// if the parent is better candidate than the child...
		if(parentDistance < candidateDistance){
			candidate = root;
			Node* twinCandidate = search(x, y, otherPath, !xLevel);
            
			if(twinCandidate != NULL){
				double twinDistance = sqrt((twinCandidate->data_->x - x)*(twinCandidate->data_->x - x)
                                           + (twinCandidate->data_->y - y)*(twinCandidate->data_->y - y));
                
				if(twinDistance < parentDistance)
					candidate = twinCandidate;
			}
		}
        
        /// if the candidate is greater than the x boundary...
		else if(xLevel && candidateDistance > abs(root->data_->x - x)){
			Node* twinCandidate = search(x, y, otherPath, !xLevel);
            
            /// checks to see if there is a twin...
			if(twinCandidate != NULL){
				double twinDistance = sqrt((twinCandidate->data_->x - x)*(twinCandidate->data_->x - x)
                                           + (twinCandidate->data_->y - y)*(twinCandidate->data_->y - y));
                
				if(twinDistance < candidateDistance)
					candidate = twinCandidate;
			}
		}
        
        /// candidate greater than y boundary...
		else if(!xLevel && candidateDistance > abs(root->data_->y - y)){
			Node* twinCandidate = search(x, y, otherPath, !xLevel);
            
            /// check to see if there is a twin...
			if(twinCandidate != NULL){//~~~~~~CHECK TWIN~~~~~~//
				double twinDistance = sqrt((twinCandidate->data_->x - x)*(twinCandidate->data_->x - x) 
                                           + (twinCandidate->data_->y - y)*(twinCandidate->data_->y - y));
                
				if(twinDistance < candidateDistance)
					candidate = twinCandidate;
			}
		}
        
	}
    
	return candidate;
}









