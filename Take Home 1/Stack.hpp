//
//  Stack.hpp
//  cs300_homework1
//
//  Created by Osman Berk An on 24.10.2023.
//

#ifndef Stack_hpp
#define Stack_hpp

#include <iostream>


#include <stdio.h>

struct Cell{
    bool l,r,u,d;   //leftwall rightwall upwall downwall
    bool visited = false;  //cell is visited or not
    int coorX, coorY;   // coordinates of cell
};


template <class Object>
class Stack
{
public:
    Stack( );
    Stack( const Stack & rhs );
    ~Stack( );
    bool isEmpty( ) const;
    bool isFull( ) const;
    void makeEmpty( );
    void pop( );
    void push( const Object & x );
    Object topAndPop( );
    const Object & top( ) const;
    const Stack & operator=( const Stack & rhs );
    private:
    
        struct ListNode {
         Object element;
         ListNode *next;
            
         ListNode( const Object & theElement, ListNode * n = nullptr )
             : element( theElement ), next( n ) {}
        };
    
        ListNode *topOfStack;  // list itself is the stack
};


#endif /* Stack_hpp */


