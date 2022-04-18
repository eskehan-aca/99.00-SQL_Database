#ifndef NODE_H
#define NODE_H
#include <iomanip>
#include <iostream>
#include <assert.h>

using namespace std;
template <typename T>
struct node{
    //CTOR
    node(const T& item=T(), node<T>* next=NULL, node<T>* prev=NULL):_item(item), _next(next), _prev(prev){}
    
    //FUNCT
    friend ostream& operator << (ostream& outs, const node<T>& print_me){
        if(&print_me==nullptr)
            outs<<"[NULLPTR]";
        else
            outs<<"<-["<<print_me._item<<"]->";
        return outs;
    }
    
    //VARIABLES
    T _item;
    node<T>* _next;
    node<T>* _prev;
};

#endif //NODE_H