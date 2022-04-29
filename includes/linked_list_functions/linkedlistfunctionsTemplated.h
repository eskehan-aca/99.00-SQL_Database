#ifndef LINKED_LIST_FUNCTIONS_TEMPLATED_H
#define LINKED_LIST_FUNCTIONS_TEMPLATED_H
#include "../node/node.h"

const bool linkedDebug=false;

//Linked List General Functions:
template <typename T>
ostream& _print_list(node<T>* head, ostream& outs=cout);                        //varies from given def

//recursive fun! :)
template <typename T>
ostream& _print_list_backwards(node<T> *head, ostream& outs=cout);              //varies from given def

//return ptr to key or NULL
template <typename T>
node<T>* _search_list(node<T>* head, T key);

template <typename T>
node<T>* _insert_head(node<T> *&head, T insert_this);

//insert after ptr: insert head if marker null                                  //modified
template <typename T>
node<T>* _insert_after(node<T>*& head, node<T> *after_this, T insert_this);

//insert before ptr: insert head if marker null                                 //modified
template <typename T>
node<T>* _insert_before(node<T>*& head, node<T>* before_this, T insert_this);

//ptr to previous node                                                          //modified
template <typename T>
node<T>* _previous_node(node<T>* prev_to_this);

//delete, return item
template <typename T>
T _delete_node(node<T>*&head, node<T>* delete_this);

//duplicate the list...
template <typename T>
node<T>* _copy_list(node<T>* head);

//duplicate list and return ptr to the last node of the copy/dst
//Add the following _copy_list function to your linked list library. This function will take the source and destination head pointers. Copies the source to destination list and returns the address of the last node in destination. 
//You will use this function in your Queue's copy constructor and assignment operator (big3). You do not want to travel the entire length of the queue to find the rear. In fact, at no point in your project will you travel the length of the queue to find the rear.
template <typename T>
node<T>* _copy_list(node<T>* &dest, node<T> *src);

//delete all the nodes
template <typename T>
void _clear_list(node<T>*& head);

//_item at this position
template <typename T>
T& _at(node<T>* head, int pos);

//----------------------------------------------------------------------------

template <typename T>
ostream& _print_list(node<T>* head, ostream& outs){
    outs<<"H->";
    for(node<T> *walker=head; walker!=nullptr; walker=walker->_next)
        outs<<*walker;
    outs<<"|||"<<endl;
    return outs;
}
template <typename T>
ostream& _print_list_backwards(node<T> *head, ostream& outs){
    // outs<<"|||->";
    // for(node<T> *walker=_last_node(head); walker!=head; walker=_previous_node(head,walker))
    //     outs<<*walker;
    // outs<<*head<<"H"<<endl;
    return outs;
}
template <typename T>
node<T>* _search_list(node<T>* head, T key){
    for(node<T> *walker=head; walker!=nullptr; walker=walker->_next){
        if(walker->_item==key)
            return walker;
    }
    return nullptr;
}
template <typename T>
node<T>* _insert_head(node<T>*& head, T insert_this){
    node<T> *temp=new node<T>(insert_this, head, nullptr);  //create new node
    if(head)                                                //list was not empty
        head->_prev=temp;
    head=temp;                                              //head to new node
    return temp;
}
template <typename T>
node<T>* _insert_after(node<T>*& head, node<T> *after_this, T insert_this){
    if(head==nullptr||after_this==nullptr)
       return _insert_head(head, insert_this);
    node<T> *temp=new node<T>(insert_this, after_this->_next, after_this);
    if(after_this->_next)
        (after_this->_next)->_prev=temp;
    after_this->_next=temp;
    return temp;
}
template <typename T>
node<T>* _insert_before(node<T>*& head, node<T>* before_this, T insert_this){
    return _insert_after(head, before_this->_prev, insert_this);
}
template <typename T>
node<T>* _previous_node(node<T>* prev_to_this){
    if(prev_to_this->_prev==nullptr && linkedDebug)
        cout<<"prev node is null (given head)"<<endl;
    return prev_to_this->_prev;
}
//delete, return item
template <typename T>
T _delete_node(node<T>*&head, node<T>* delete_this){
    assert(head && delete_this);  //assert not null
    T pop=delete_this->_item;
    
    //deal with following node - if there is a next node, make _prev point to _prev of what is being deleted
    if(delete_this->_next){
        (delete_this->_next)->_prev=_previous_node(delete_this);
    }
    //deal with previous node - if there is a prev node (not head), set the prev node's _next to the _next of node being del
    if(_previous_node(delete_this)){
        _previous_node(delete_this)->_next=delete_this->_next;
    }         
    //deleting head      
    else{ 
        head=delete_this->_next;
    }
    delete_this=nullptr;
    delete delete_this;
    return pop;
}
//duplicate the list...
template <typename T>
node<T>* _copy_list(node<T>* head){
    node<T> *newHead=nullptr;
    node<T> *dstWalker=newHead;
    for(node<T> *srcWalker=head; srcWalker!=nullptr; srcWalker=srcWalker->_next){
        dstWalker=_insert_after(newHead, dstWalker, srcWalker->_item);
    }
    return newHead;
}
//duplicate list and return the last node of the copy
template <typename T>
node<T> *_copy_list(node<T>* &dest, node<T> *src){
    assert(dest==nullptr);
    node<T> *dstWalker=dest;
    for(node<T> *srcWalker=src; srcWalker!=nullptr; srcWalker=srcWalker->_next){
        dstWalker=_insert_after(dest, dstWalker, srcWalker->_item);
    }
    return dstWalker;
}
//delete all the nodes
template <typename T>
void _clear_list(node<T>*& head){
    while(head){ //returns false if head==nullptr
        T item=_delete_node(head,head);
    }
    head=nullptr;
}
//_item at this position
template <typename T>
T& _at(node<T>* head, int pos){
    assert(pos>=0 && head);
    int i=0;
    for(node<T> *walker=head; walker!=nullptr; walker=walker->_next){
        if(i==pos)
            return walker->_item;
        i++;
    }
    return nullptr;
}

//----------------------------------------------------------------------------

#endif // LINKED_LIST_FUNCTIONS_TEMPLATED_H
