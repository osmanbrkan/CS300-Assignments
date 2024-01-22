//
// Created by Osman Berk An on 9.12.2023.
//

#ifndef CS300_HOMEWORK3_HASHTABLE_H
#define CS300_HOMEWORK3_HASHTABLE_H

#include <iostream>
#include <string>
#include <cmath>
using namespace std;

struct DocumentItem {
    string documentName;
    int count;

    bool operator == (const DocumentItem& rhs)const{
        return documentName == rhs.documentName;

    }

};

struct WordItem {
    string word;
    // List of DocumentItem’s. In order to keep the documents
    //you can again use the BST that you are going to implement.
    vector<DocumentItem> details;

    // Comparison operators for WordItem
    bool operator<(const WordItem& other) const {
        return word < other.word;
    }

    bool operator == (const WordItem& other) const{
        return word == other.word;
    }

    //need to add != for insert method in hash table
    bool operator != (const WordItem&other) const{
        return word != other.word;
    }
    bool operator>(const WordItem& other) const {  // for binary search
        return word >other.word;
    }
    bool operator<=(const WordItem& other) const {
        return word <= other.word;
    }
    bool operator>=(const WordItem& other) const {  // for binary search
        return word >=other.word;
    }

    void incrementDocCount(string docName)  // incrementing document count of existing word for that document
    {
        bool foundInList = false;
        for (int i = 0; i < details.size(); i++)
        {
            if(details[i].documentName == docName)
            {
                details[i].count++;
                foundInList = true;
            }
        }
        if(!foundInList)
        {
            DocumentItem newDocItem;
            newDocItem.count = 1;
            newDocItem.documentName = docName;
            details.push_back(newDocItem);
        }
    }


};



template <class HashedObj>
class HashTable {
public:
    explicit HashTable(const HashedObj &notFound, int size);

    HashTable(const HashTable &rhs)
            : ITEM_NOT_FOUND(rhs.ITEM_NOT_FOUND),
              array(rhs.array), currentSize(rhs.currentSize) {}

    const HashedObj &find(const HashedObj &x) const;

    void makeEmpty();

    void insert(const HashedObj &x);

    void remove(const HashedObj &x);

    const HashTable & operator=(const HashTable &rhs);

    enum EntryType {
        ACTIVE, EMPTY, DELETED
    };


    void afterPreprocessing();




private:
    struct HashEntry {
        HashedObj element;
        EntryType info;

        HashEntry(const HashedObj &e = HashedObj(),
                  EntryType i = EMPTY)
                : element(e), info(i) {}
    };
    vector<HashEntry> array;
    int currentSize;
    const HashedObj ITEM_NOT_FOUND;

    bool isActive(int currentPos) const;

    int findPos(const HashedObj &x) const;

    void rehash();

    int hashF(const WordItem&) const;  // need to write hash function for WordItems

};

//additional methods used in other methods-> isPrime, nextPrime

//isPrime
bool isPrime( int n )
{
    if ( n == 2 || n == 3 )
        return true;

    if ( n == 1 || n % 2 == 0 )
        return false;

    for ( int i = 3; i * i <= n; i += 2 )
        if ( n % i == 0 )
            return false;

    return true;
}


//nextPrime
int nextPrime( int n )
{
    if ( n % 2 == 0 )
        n++;

    for ( ; ! isPrime( n ); n += 2 )
        ;

    return n;
}

//CONSTRUCTOR
template <class HashedObj>
HashTable<HashedObj>::HashTable( const HashedObj & notFound, int size ) : ITEM_NOT_FOUND( notFound ), array( nextPrime( size ))
{
    //makeEmpty( );
}



//implementations of class

//hash function  //takes prime number
template <class HashedObj>
int HashTable<HashedObj>::hashF(const WordItem& item) const {   //example hash function from slide
    int hashVal = 0;
    string key = item.word;

    for (int i = 0; i < key.length();i++)
        hashVal = 37 * hashVal + key[ i ];

    hashVal = hashVal % array.size();

    if (hashVal < 0)
        hashVal += array.size();

    return hashVal;
}


//private findPos
// method that performs quadratic probing resolution, return the position where the search for x terminates
template <class HashedObj>
int HashTable<HashedObj>::findPos( const HashedObj & x ) const
{
    int collisionNum = 0;

    //int currentPos = hash( x, array.size( ) );
    int currentPos = hashF(x);

    while ( array[ currentPos ].info != EMPTY &&
            array[ currentPos ].element != x )
    {
        currentPos += pow(++collisionNum, 2) ;  //add the difference
        if ( currentPos >= array.size( ) ) // perform the mod
            currentPos -= array.size(); // if necessary
    }
    return currentPos;
}

//private isActive
template <class HashedObj>
bool HashTable<HashedObj>::isActive( int currentPos ) const
{
    return array[ currentPos ].info == ACTIVE;
}


//expand the size of hash table
template <class HashedObj>
void HashTable<HashedObj>::rehash( )
{

    vector<HashEntry> oldArray = array;

    // Create new double-sized, empty table
    array.resize( nextPrime( 2 * oldArray.size( ) ) );
    for ( int j = 0; j < array.size( ); j++ )
        array[ j ].info = EMPTY;

    // Copy table over
    currentSize = 0;
    for ( int i = 0; i < oldArray.size( ); i++ )
        if ( oldArray[ i ].info == ACTIVE ){
            insert( oldArray[ i ].element );

        }

    //after all rehashing, need to output information about hash table
    /*  //NO NEED TO OUTPUT THIS INFO THIS TIME!
    cout << "rehashed..." << endl;
    cout << "previous table size: " << oldArray.size()
         << ", new table size: " << array.size()
         << ", current unique word count " << currentSize

         //The load will be calculated as (item count / capacity).
         << ", current load factor: " << double(currentSize) / array.size()
         << endl;
    */
}


//find method
//find item x in the hash table
template <class HashedObj>
const HashedObj & HashTable<HashedObj>::find( const HashedObj & x ) const
{
    int currentPos = findPos( x );
    if (isActive( currentPos ))
        return array[ currentPos ].element;

    return   ITEM_NOT_FOUND;
}

//insert method
//insert item x into hash table
template <class HashedObj>
void HashTable<HashedObj>::insert( const HashedObj & x )
{
    // Insert x as active
    int currentPos = findPos( x );
    if ( isActive( currentPos ) )
        return;
    array[ currentPos ] = HashEntry( x, ACTIVE );

    // enlarge the hash table if necessary
    if (++currentSize >= array.size( ) / 1.5)
        rehash( );
}

//remove method
//remove item x from the hash table
//x has to be in the hash table
template <class HashedObj>
void HashTable<HashedObj>::remove( const HashedObj & x )
{
    int currentPos = findPos( x );
    if ( isActive( currentPos ) )
        array[ currentPos ].info = DELETED;
}

//make empty
template <class HashedObj>
void HashTable<HashedObj>::makeEmpty( )
{
    for( int i = 0; i < array.size( ); i++ )
        array[ i ].makeEmpty( );
    // destroy the lists but not the vector!
}

//assignment op
template <class HashedObj>
const HashTable<HashedObj> &HashTable<HashedObj>::operator=(const HashTable<HashedObj> &rhs) {
    if (this != &rhs) {
        // Clear the existing elements
        makeEmpty();

        // Copy elements from rhs
        array = rhs.array;
        currentSize = rhs.currentSize;
        // No need to copy ITEM_NOT_FOUND, as it's constant
    }
    return *this;
}

//print after preprocess file
template <class HashedObj>
void HashTable<HashedObj>::afterPreprocessing() {
    cout << "After preprocessing, the unique word count is " << currentSize << ". Current load ratio is " << double(currentSize)/array.size() << endl;
}




#endif //CS300_HOMEWORK3_HASHTABLE_H
