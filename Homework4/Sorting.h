//
// Created by Osman Berk An on 29.12.2023.
//



#ifndef CS300_HOMEWORK4_SORTING_H
#define CS300_HOMEWORK4_SORTING_H

#endif //CS300_HOMEWORK4_SORTING_H

#include <iostream>
#include <string>
#include <vector>
#include <random>


using namespace std;


// INSERTION SORT
//just an insertion sort function
template <class Comparable>
void insertionSort (vector <Comparable> & a)
{
    int j;
    // loop over the passes
    for (int p = 1;  p < a.size(); p++)
    {
        Comparable tmp = a[p];
        // loop over the elements
        for (j = p; j > 0 &&  tmp < a[j-1]; j--)
            a[j] = a[j-1];
        a[j] = tmp;
    }
}

//HEAP SORT
//percDown, swapFunc, leftChild and  heap sort functions

inline int leftChild( int i ){
    return 2*i+1;
}

template <class Comparable>
inline void swapFunc( Comparable & obj1,
                      Comparable & obj2 ){   //Basic swapping func
    Comparable tmp = obj1;
    obj1 = obj2;
    obj2 = tmp;
}

template <class Comparable>  // for deleteMax
// a is the array, i is the position to percolate down from// n is the logical size of the array
void percDown( vector<Comparable> & a, int i, int n )
{
    int child;
    Comparable tmp;

    for (tmp=a[i] ; leftChild(i) < n; i = child )
    {
        child = leftChild(i);
        if ( child != n-1 && a[ child  ] < a[ child+1 ] )
            child++;
        if ( a[child ] > tmp )
            a[i] = a[ child ];
        else
            break;
    }
    a[ i ] = tmp;
}



template <class Comparable>
void heapsort(vector<Comparable> & a)
{
    // buildHeap
    for (int i = a.size()/2; i >=0; i--)
        percDown(a, i, a.size());

    // sort
    for (int j = a.size() -1; j >0; j--)
    {
        swapFunc(a[0], a[j]);    // swap max to the last pos.
        percDown(a, 0, j); // re-form the heap
    }
}

//MERGE SORT


//Mergesort algorithm (driver).
//recursive
template <class Comparable>
void mergeSort( vector<Comparable> & a )
{
    vector<Comparable> tmpArray( a.size( ) );

    mergeSort( a, tmpArray, 0, a.size( ) - 1 );
}

//internal merge method, Internal method that merges two sorted halves of a subarray.
//a is an array of Comparable items.
//tmpArray is an array to place the merged result.
// leftPos is the left-most index of the subarray.
// rightPos is the index of the start of the second half.
// rightEnd is the right-most index of the subarray.
template <class Comparable>
void merge(  vector<Comparable> & a,
             vector<Comparable> & tmpArray,
             int leftPos, int rightPos, int rightEnd ){
    int leftEnd = rightPos - 1;
    int tmpPos = leftPos;
    int numElements = rightEnd - leftPos + 1;

    //Main loop
    while ( leftPos <= leftEnd && rightPos <= rightEnd ) {

        if (a[leftPos] <= a[rightPos])
            tmpArray[tmpPos++] = a[leftPos++];
        else
            tmpArray[tmpPos++] = a[rightPos++];
    }
    while ( leftPos <= leftEnd )    // Copy rest of first half
        tmpArray[ tmpPos++ ] = a[ leftPos++ ];

    while ( rightPos <= rightEnd )  // Copy rest of right half
        tmpArray[ tmpPos++ ] = a[ rightPos++ ];

    // Copy tmpArray back
    for ( int i = 0; i < numElements; i++, rightEnd-- )
        a[ rightEnd ] = tmpArray[ rightEnd ];
}



 // Internal method that makes recursive calls.
 // a is an array of Comparable items.
 // tmpArray is an array to place the merged result.
 // left is the left-most index of the subarray.
 // right is the right-most index of the subarray.
 template <class Comparable>
 void mergeSort( vector<Comparable> & a,
                    vector<Comparable> & tmpArray, int left, int right ){
        if ( left < right )
        {
            int center = ( left + right ) / 2;
            mergeSort( a, tmpArray, left, center );
            mergeSort( a, tmpArray, center + 1, right );
            merge( a, tmpArray, left, center + 1, right );
        }
 }





//QUICK SORT


//check these:
//quicksort first
//quicksort median
//quicksort random

//driver
template <class Comparable>
void quicksort( vector<Comparable> & a )
{
    quicksort( a, 0, a.size( ) - 1 );
}


/** Return median of left, center, and right.
  *   Order these and hide the pivot.
 */
template <class Comparable>
const Comparable & median3( vector<Comparable> & a,
                                int left, int right ){
        int center = ( left + right ) / 2;
        if ( a[ center ] < a[ left ] )
            swapFunc( a[ left ], a[ center ] );
        if ( a[ right ] < a[ left ] )
            swapFunc( a[ left ], a[ right ] );
        if ( a[ right ] < a[ center ] )
            swapFunc( a[ center ], a[ right ] );

        // Place pivot at position right - 1
        swapFunc( a[ center ], a[ right - 1 ] );
        return a[ right - 1 ];
}


//internal insertionSort for quicksort
template <class Comparable>
void insertionSort( vector<Comparable> & a, int left, int right )
{
        for ( int p = left + 1; p <= right; p++ )
        {
            Comparable tmp = a[ p ];
            int j;

            for ( j = p; j > left && tmp < a[ j - 1 ]; j-- )
                a[ j ] = a[ j - 1 ];
            a[ j ] = tmp;
        }
}


/**
 * Internal quicksort method that makes recursive calls.
  * Uses median-of-three partitioning and a cutoff of 10.
  * a is an array of Comparable items.
 * left is the left-most index of the subarray.
 * right is the right-most index of the subarray.
 */
//quicksort with selecting pivot with median3 method
template <class Comparable>
void quicksort( vector<Comparable> & a, int left, int right ) {

        if (left + 10 <= right) {
            Comparable pivot = median3(a, left, right);
            //begin partitioning
            int i = left, j = right - 1;
            for ( ; ; )
            {
                while ( a[ ++i ] < pivot ) { }

                while ( pivot < a[ --j ] ) { }

                if ( i < j )
                    swapFunc( a[ i ], a[ j ] );
                else
                    break;
            }
            swapFunc( a[ i ], a[ right - 1 ] );   // Restore pivot

            quicksort( a, left, i - 1 );       // Sort small elements
            quicksort( a, i + 1, right );    // Sort large elements
        }
        else{  // Do an insertion sort on the subarray
            insertionSort( a, left, right );

        }
}



//quicksort with selecting first element as pivot
template <class Comparable>
void quicksortFirst( vector<Comparable> & a )
{
    quicksortFirst( a, 0, a.size( ) - 1 );
}



//partitionFirst
template <class Comparable>
int partitionFirst(std::vector<Comparable> & a, int left, int right) {
    Comparable pivot = a[left];

    int i = left + 1;
    int j = right;

    while (true) {
        while (i <= j && a[i] < pivot) ++i;
        while (i <= j && pivot < a[j]) --j;

        if (i <= j)
            swapFunc(a[i++], a[j--]);
        else
            break;
    }

    swapFunc(a[left], a[j]);  // Restore pivot

    return j;
}


//quicksort with selecting pivot as first element of vector
template <class Comparable>
void quicksortFirst(vector<Comparable> & a, int left, int right) {

    if (left + 10 <= right) {
        int pivotIndex = partitionFirst(a, left, right);

        quicksortFirst(a, left, pivotIndex - 1);    // Sort small elements
        quicksortFirst(a, pivotIndex + 1, right);   // Sort large elements
    } else {
        // Do an insertion sort on the subarray
        insertionSort(a, left, right);
    }


}


// quicksort with selecting random element as pivot
template <class Comparable>
void quicksortRandom( vector<Comparable> & a )
{
    quicksortRandom( a, 0, a.size( ) - 1 );
}

// Return a random index between left and right (inclusive)
//for quicksortRandom
inline int randomIndex(int left, int right) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distribution(left, right);
    return distribution(gen);
}


template <class Comparable>
const Comparable & randomPivot(vector<Comparable> & a, int left, int right) {
    return a[randomIndex(left, right)];
}

//quicksort with selecting random element as pivot
template <class Comparable>
void quicksortRandom(vector<Comparable> & a, int left, int right) {
    if (left + 10 <= right) {
        Comparable pivot = randomPivot(a, left, right);

        // Begin partitioning
        int i = left, j = right - 1;
        for (; ;) {
            while (a[++i] < pivot) { }
            while (pivot < a[--j]) { }
            if (i < j)
                swapFunc(a[i], a[j]);
            else
                break;
        }
        swapFunc(a[i], a[right - 1]);   // Restore pivot

        quicksortRandom(a, left, i - 1);      // Sort small elements
        quicksortRandom(a, i + 1, right);     // Sort large elements
    } else {
        // Do an insertion sort on the subarray
        insertionSort(a, left, right);
    }
}




