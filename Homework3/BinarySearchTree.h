//
// Created by Osman Berk An on 9.12.2023.
//

#ifndef CS300_HOMEWORK3_BINARYSEARCHTREE_H
#define CS300_HOMEWORK3_BINARYSEARCHTREE_H

#include <string>
#include <iostream>
using namespace std;



template <class Comparable>
class BinarySearchTree;

template <class Comparable>
class BinaryNode
{
    Comparable element;
    BinaryNode  *left;
    BinaryNode  *right;

    BinaryNode( const Comparable & theElement,
                BinaryNode *lt, BinaryNode *rt )
            : element( theElement ), left( lt ), right( rt ) { }

    friend class BinarySearchTree<Comparable>;
};

template <class Comparable>
class BinarySearchTree
{
public:
    explicit BinarySearchTree( const Comparable & notFound );
    BinarySearchTree( const BinarySearchTree & rhs );
    ~BinarySearchTree( );

    const Comparable & findMin( ) const;
    const Comparable & findMax( ) const;
    const Comparable & find( const Comparable & x ) const;
    bool isEmpty( ) const;

    //void printTree( ) const;

    void makeEmpty( );
    void insert( const Comparable & x );
    void remove( const Comparable & x );

    const BinarySearchTree & operator=( const BinarySearchTree & rhs );


private:
    BinaryNode<Comparable> *root;
    const Comparable ITEM_NOT_FOUND;

    const Comparable & elementAt( BinaryNode<Comparable> *t ) const;

    void insert( const Comparable & x, BinaryNode<Comparable> * & t ) const;
    void remove( const Comparable & x, BinaryNode<Comparable> * & t ) const;
    BinaryNode<Comparable> * findMin( BinaryNode<Comparable> *t ) const;
    BinaryNode<Comparable> * findMax( BinaryNode<Comparable> *t ) const;
    BinaryNode<Comparable> * find( const Comparable & x, BinaryNode<Comparable> *t ) const;
    void makeEmpty( BinaryNode<Comparable> * & t ) const;
    //void printTree( BinaryNode<Comparable> *t ) const;
    BinaryNode<Comparable> * clone( BinaryNode<Comparable> *t ) const;

    /*
    // Avl manipulations
    int height( AvlNode<Comparable> *t ) const;
    int max( int lhs, int rhs ) const;
    void rotateWithLeftChild( AvlNode<Comparable> * & k2 ) const;
    void rotateWithRightChild( AvlNode<Comparable> * & k1 ) const;
    void doubleWithLeftChild( AvlNode<Comparable> * & k3 ) const;
    void doubleWithRightChild( AvlNode<Comparable> * & k1 ) const;*/

};

//implementations of methods

//constructor
template <class Comparable>
BinarySearchTree<Comparable>::BinarySearchTree(const Comparable &notFound) :ITEM_NOT_FOUND( notFound ), root( nullptr ){}

//copy constructor
template <class Comparable>
BinarySearchTree<Comparable>::BinarySearchTree(const  BinarySearchTree<Comparable> & rhs ) :
        root( nullptr ), ITEM_NOT_FOUND( rhs.ITEM_NOT_FOUND )
{
    *this = rhs;
}

//destructor
template <class Comparable>
BinarySearchTree<Comparable>::~BinarySearchTree( )
{
    makeEmpty();
}

//Internal method to get element field in node t.
//Return the element field or ITEM_NOT_FOUND if t is NULL
template <class Comparable>
const Comparable & BinarySearchTree<Comparable>:: elementAt( BinaryNode<Comparable> *t ) const
{
    return t == nullptr ? ITEM_NOT_FOUND : t->element;
}


//public find
// find item x in the tree.
// * Return the matching item or ITEM_NOT_FOUND if not found.
template <class Comparable>
const Comparable & BinarySearchTree<Comparable>::find(const Comparable & x ) const
{
    return elementAt( find( x, root ) );
}

//private find
//Internal method to find an item in a subtree.
// x is item to search for.
//t is the node that roots the tree.
//Return node containing the matched item.
template <class Comparable>
BinaryNode<Comparable> * BinarySearchTree<Comparable>:: find(const Comparable & x, BinaryNode<Comparable> * t ) const
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
const Comparable & BinarySearchTree<Comparable>::findMin( ) const
{
    return elementAt( findMin( root ) );
}


//private findMin
//Internal method to find the smallest item in a subtree t.
// Return node containing the smallest item.
template <class Comparable>
BinaryNode<Comparable> * BinarySearchTree<Comparable>::findMin( BinaryNode<Comparable> *t )const
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
const Comparable & BinarySearchTree<Comparable>::findMax( ) const
{
    return elementAt( findMax( root ) );
}

//private findMax
//Internal method to find the largest item in a subtree t.
// Return node containing the largest item.
template <class Comparable>
BinaryNode<Comparable> *BinarySearchTree<Comparable>::findMax( BinaryNode<Comparable> *t )const
{
    if( t != NULL )
        while( t->right != NULL )
            t = t->right;
    return t;
}

//public makeEmpty
template <class Comparable>
void BinarySearchTree<Comparable>::makeEmpty( )
{
    makeEmpty( root );
}

//private makeEmpty
template <class Comparable>
void BinarySearchTree<Comparable>::makeEmpty( BinaryNode<Comparable> * & t ) const
{
    if( t != NULL )
    {
        makeEmpty( t->left );
        makeEmpty( t->right );
        delete t;
    }
    t = NULL;
}


//public isEmpty function   returns true if root is null
template <class Comparable>
bool BinarySearchTree<Comparable>::isEmpty() const {
    return root ==  NULL;
}

//node clone
template <class Comparable>
BinaryNode<Comparable>* BinarySearchTree<Comparable>::clone(BinaryNode<Comparable>* t) const {
    if (t == nullptr) {
        return NULL;
    }
    BinaryNode<Comparable>* newNode = new BinaryNode<Comparable>(t->element);

    newNode->left = clone(t->left);
    newNode->right = clone(t->right);
    //clone everything of node structure

    return newNode;
}

// assignment operator
template <class Comparable>
const BinarySearchTree<Comparable>& BinarySearchTree<Comparable>::operator=(const BinarySearchTree& rhs) {
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
void BinarySearchTree<Comparable>::insert(const Comparable & x )
{
    insert( x, root );
}

//private insert
template <class Comparable>
void BinarySearchTree<Comparable>::insert (const Comparable & x, BinaryNode<Comparable> * & t ) const
{
    if( t == NULL ) //  create a new node at the right place
        t = new BinaryNode<Comparable>( x, NULL, NULL );
    else if( x < t->element )
        insert( x, t->left );  // insert at the left or
    else if( t->element < x )
        insert( x, t->right );  // right subtree
    else
        ;
}

//public remove
template<class Comparable>
void BinarySearchTree<Comparable>::remove(const Comparable & x )
{
    remove( x, root );
}

//private remove
template<class Comparable>
void BinarySearchTree<Comparable>::remove (const Comparable & x, BinaryNode<Comparable> * & t ) const
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
        BinaryNode<Comparable> *oldNode = t;
        t = (t->left != nullptr) ? t->left : t->right;  //checking which side is null
        delete oldNode; // and delete
    }

}


#endif //CS300_HOMEWORK3_BINARYSEARCHTREE_H
