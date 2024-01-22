//
//  AvlTree.hpp
//  hw2
//
//  Created by Osman Berk An on 14.11.2023.
//

#ifndef AvlTree_hpp
#define AvlTree_hpp

//#include <stdio.h>
#include <string>
using namespace std;

template <class Comparable>
class AvlTree;

template <class Comparable>
class AvlNode
{
    Comparable     element;
    AvlNode         *left;
    AvlNode         *right;
    int              height;

    AvlNode(const Comparable & theElement,
                AvlNode *lt, AvlNode *rt, int h = 0 )
            : element( theElement ), left( lt ), right( rt ), height( h ) { }

    friend class AvlTree<Comparable>;
};




template <class Comparable>
class AvlTree
{
public:
    
    
    explicit AvlTree(const Comparable & notFound );
    AvlTree( const AvlTree & rhs );
    ~AvlTree( );
    
    const Comparable & findMin( ) const;
    const Comparable & findMax( ) const;
    const Comparable & find(const Comparable & x ) const;
    bool isEmpty( ) const;
    
    void makeEmpty( );
    void insert(const Comparable & x );
    void remove(const Comparable & x );
    
    const AvlTree & operator=( const AvlTree & rhs );
    
   // bool operator<(const AvlTree<Comparable> &rhs) const;
    
   // bool operator==(const AvlTree<Comparable> &rhs) const;
   
    
    
    
private:
    AvlNode<Comparable> *root;
    const Comparable ITEM_NOT_FOUND;

    //bool isEqualNodes(AvlNode<Comparable> *lhs, AvlNode<Comparable> *rhs) const;
    const Comparable & elementAt( AvlNode<Comparable> *t ) const;

    void insert(const Comparable & x, AvlNode<Comparable> * & t ) const;
    void remove(const Comparable & x, AvlNode<Comparable> * & t ) const;

    AvlNode<Comparable> * findMin( AvlNode<Comparable> *t ) const;
    AvlNode<Comparable> * findMax( AvlNode<Comparable> *t ) const;
    AvlNode<Comparable> * find(const Comparable & x, AvlNode<Comparable> *t )const;

    void makeEmpty( AvlNode<Comparable> * & t ) const;
    
    AvlNode<Comparable> * clone( AvlNode<Comparable> *t ) const;

    // Avl manipulations
    int height( AvlNode<Comparable> *t ) const;
    int max( int lhs, int rhs ) const;
    void rotateWithLeftChild( AvlNode<Comparable> * & k2 ) const;
    void rotateWithRightChild( AvlNode<Comparable> * & k1 ) const;
    void doubleWithLeftChild( AvlNode<Comparable> * & k3 ) const;
    void doubleWithRightChild( AvlNode<Comparable> * & k1 ) const;

};


//MY CPP
//
//  AvlTree.cpp
//  hw2
//
//  Created by Osman Berk An on 14.11.2023.
//

#include <iostream>


//constructor
template <class Comparable>
AvlTree<Comparable>:: AvlTree(const Comparable & notFound ) :ITEM_NOT_FOUND( notFound ), root( nullptr ){}

//copy constructor
template <class Comparable>
AvlTree<Comparable>:: AvlTree(const  AvlTree<Comparable> & rhs ) :
        root( nullptr ), ITEM_NOT_FOUND( rhs.ITEM_NOT_FOUND )
{
    *this = rhs;
}

//destructor
template <class Comparable>
AvlTree<Comparable>::~AvlTree( )
{
    makeEmpty();
}

//Internal method to get element field in node t.
//Return the element field or ITEM_NOT_FOUND if t is NULL
template <class Comparable>
const Comparable & AvlTree<Comparable>:: elementAt( AvlNode<Comparable> *t ) const
{
    return t == nullptr ? ITEM_NOT_FOUND : t->element;
}


//public find
// Find item x in the tree.
// * Return the matching item or ITEM_NOT_FOUND if not found.
template <class Comparable>
const Comparable & AvlTree<Comparable>::find(const Comparable & x ) const
{
    return elementAt( find( x, root ) );
}

//private find
//Internal method to find an item in a subtree.
// x is item to search for.
//t is the node that roots the tree.
//Return node containing the matched item.
template <class Comparable>
AvlNode<Comparable> * AvlTree<Comparable>:: find(const Comparable & x, AvlNode<Comparable> * t ) const
{
    if ( t == NULL )
        return nullptr;
    else if( x < t->element )
        return find( x, t->left );
    else if( t->element < x )
        return find( x, t->right );
    else
        return t;    // Match
}


//public findMin
// Find the smallest item in the tree.
//Return smallest item or ITEM_NOT_FOUND if empty.
template <class Comparable>
const Comparable & AvlTree<Comparable>::findMin( ) const
{
    return elementAt( findMin( root ) );
}


//private findMin
//Internal method to find the smallest item in a subtree t.
// Return node containing the smallest item.
template <class Comparable>
AvlNode<Comparable> * AvlTree<Comparable>::findMin( AvlNode<Comparable> *t )const
{
    if( t == NULL )
        return nullptr;
    if( t->left == NULL )
        return t;
    return findMin( t->left );
}

//public findMax
//find the largest item in tree
template <class Comparable>
const Comparable & AvlTree<Comparable>::findMax( ) const
{
    return elementAt( findMax( root ) );
}

//private findMax
//Internal method to find the largest item in a subtree t.
// Return node containing the largest item.
template <class Comparable>
AvlNode<Comparable> *AvlTree<Comparable>::findMax( AvlNode<Comparable> *t )const
{
    if( t != NULL )
        while( t->right != NULL )
            t = t->right;
    return t;
}

//public makeEmpty
template <class Comparable>
void AvlTree<Comparable>::makeEmpty( )
{
    makeEmpty( root );
}

//private makeEmpty
template <class Comparable>
void AvlTree<Comparable>::makeEmpty( AvlNode<Comparable> * & t ) const
{
    if( t != NULL )
    {
        makeEmpty( t->left );
        makeEmpty( t->right );
        delete t;
    }
    t = NULL;
}

//height
//Return the height of node t, or -1, if NULL.
template <class Comparable>
int AvlTree<Comparable>::height( AvlNode<Comparable> *t )const
{
    if (t == NULL)
        return -1;

    return t->height;
}

//max
//return maximum of lhs and rhs
template <class Comparable>
int AvlTree<Comparable>::max( int lhs, int rhs ) const
{
    if (lhs > rhs)
        return lhs;

    return rhs;
}


//rotations

//Rotate binary tree node with left child.
//  * For AVL trees, this is a single rotation for case 1.
//* Update heights, then set new root.
template <class Comparable>
void AvlTree<Comparable>::rotateWithLeftChild(AvlNode<Comparable> * & k2 ) const
{
    AvlNode<Comparable> *k1 = k2->left;
    k2->left = k1->right;
    k1->right = k2;
    k2->height = max( height( k2->left ), height( k2->right ) ) + 1;
    k1->height = max( height( k1->left ), k2->height ) + 1;
    k2 = k1;
}

template <class Comparable>
void AvlTree<Comparable>::rotateWithRightChild( AvlNode<Comparable> * & k1 ) const
{
    AvlNode<Comparable> *k2 = k1->right;
    k1->right = k2->left;
    k2->left = k1;
    k1->height = max( height( k1->left ), height( k1->right ) ) + 1;
    k2->height = max( height( k2->right ), k1->height ) + 1;
    k1 = k2;
}


//Double rotate binary tree node: first left child.
//* with its right child; then node k3 with new left child.
//* For AVL trees, this is a double rotation for case 2.
//* Update heights, then set new root.
template <class Comparable>
void AvlTree<Comparable>::doubleWithLeftChild( AvlNode<Comparable> * & k3 ) const
{
    rotateWithRightChild( k3->left );
    rotateWithLeftChild( k3 );
}

template <class Comparable>
void AvlTree<Comparable>::doubleWithRightChild( AvlNode<Comparable> * & k1 ) const
{
    rotateWithLeftChild( k1->right );
    rotateWithRightChild( k1 );
}


//public isEmpty function   returns true if root is null
template <class Comparable>
bool AvlTree<Comparable>::isEmpty() const {
    return root ==  NULL;
}

//node clone
template <class Comparable>
AvlNode<Comparable>* AvlTree<Comparable>::clone(AvlNode<Comparable>* t) const {
    if (t == nullptr) {
        return NULL;
    }
    AvlNode<Comparable>* newNode = new AvlNode<Comparable>(t->element);
    newNode->left = clone(t->left);
    newNode->right = clone(t->right);
    newNode->height = t->height;
    //clone everything of node structure
    return newNode;
}

// assignment operator
template <class Comparable>
const AvlTree<Comparable>& AvlTree<Comparable>::operator=(const AvlTree& rhs) {
    if (this != &rhs) {
        makeEmpty();
        //first clear all the tree, then clone it from rhs

        root = clone(rhs.root);
    }
    return *this;
}

//public insert
//Insert x into the tree; duplicates are ignored.
template <class Comparable>
void AvlTree<Comparable>::insert(const Comparable & x )
{
    insert( x, root );
}

//private insert
template <class Comparable>
void AvlTree<Comparable>::insert (const Comparable & x, AvlNode<Comparable> * & t ) const
{
    if ( t == nullptr )
        t = new AvlNode<Comparable>( x, nullptr, nullptr );

    else if ( x < t->element ) {
        // X should be inserted to the left tree!
        insert( x, t->left );

        // Check if the left tree is out of balance (left subtree grew in height!)
        if ( height( t->left ) - height( t->right ) == 2 ){
            if ( x < t->left->element )  // X was inserted to the left-left subtree!
                rotateWithLeftChild( t );
            else                 // X was inserted to the left-right subtree!
                doubleWithLeftChild( t );
        }

    }
    else if( t->element < x ){   // Otherwise X is inserted to the right subtree
        insert( x, t->right );
        if ( height( t->right ) - height( t->left ) == 2 ){
            if ( t->right->element < x )  // X was inserted to right-right subtree
                rotateWithRightChild( t );
            else // X was inserted to right-left subtree
                doubleWithRightChild( t );
        }

    }
    else
        ;  // Duplicate; do nothing

}

//my added functions removes and < operator overload


//public remove
template<class Comparable>
void AvlTree<Comparable>::remove(const Comparable & x )
{
    remove( x, root );
}

//private remove


template<class Comparable>
void AvlTree<Comparable>::remove (const Comparable & x, AvlNode<Comparable> * & t ) const
{


    if(t == nullptr){
        return;
    }
    if (x < t->element) { // Go to the left subtree
        remove(x, t->left);
    }
    else if (t->element < x) { // Go to the right subtree
        remove(x, t->right);
    }

    else if (t->left != nullptr && t->right != nullptr) {  // Node with two children
        //replace with largest child from left or smallest from right (and delete that grandchild (which has at most 1 child)

        t->element = findMin(t->right)->element;
        remove(t->element, t->right);
    }
    else { // Node with one child or no child
        AvlNode<Comparable> *oldNode = t;
        t = (t->left != nullptr) ? t->left : t->right;  //checking which side is null
        delete oldNode; // and delete
    }

    //BALANCE
    //maybe change the rotate wtih lft child and right child casres.

    if (t != nullptr) {
        t->height = max(height(t->left), height(t->right)) + 1;
        int balance = height(t->left) - height(t->right);

        // Check and fix balance factor
        if (balance > 1) {  //when height difference is 2
            if (height(t->left->left) >= height(t->left->right)) {
                rotateWithLeftChild(t);
            }
            else {
                doubleWithLeftChild(t);
            }
        }
        else if (balance < -1) {  //when height difference is 2
            if (height(t->right->right) >= height(t->right->left)) {
                rotateWithRightChild(t);
            }
            else {
                doubleWithRightChild(t);
            }
        }
    }

}

/*
template <class Comparable>
bool AvlTree<Comparable>::operator<(const AvlTree<Comparable> &rhs) const {
    return this->root && rhs.root ? (*this->root < *rhs.root) : false;
}
*/
/*
template <class Comparable>
bool AvlTree<Comparable>::isEqualNodes(AvlNode<Comparable> *lhs, AvlNode<Comparable> *rhs) const {
    if (!lhs && !rhs) {
        return true;  // Both are empty, consider them equal
    }

    if (lhs && rhs) {
        return (*lhs == *rhs) && isEqualNodes(lhs->left, rhs->left) && isEqualNodes(lhs->right, rhs->right);
    }

    return false;  // One is empty while the other is not
}*/

/*
template <class Comparable>
bool AvlTree<Comparable>::operator==(const AvlTree<Comparable> &rhs) const {
    return isEqualNodes(this->root, rhs.root);
}
*/










#endif /* AvlTree_hpp */
