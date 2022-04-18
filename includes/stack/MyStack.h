#ifndef STACK_H
#define STACK_H
#include "../linked_list_functions/linkedlistfunctionsTemplated.h"

template <typename T>
class Stack{
public:
    class Iterator{
    public:
        friend class Stack;                 

        //CTOR
        Iterator(node<T>* p=NULL):_ptr(p){}

        //REVISIT RETURN TYPE (no &)? 
        //dereference operator
        T &operator*(){
            assert(_ptr);
            return _ptr->_item;
        }
        //REMOVED?
        // //member access operator
        // T *operator->(){
        //     assert(_ptr);
        //     return &(_ptr->_item);
        // }


        //true if _ptr is NULL
        bool is_null(){
            return _ptr==nullptr;
        }
        //true if left != right
        friend bool operator!=(const Iterator &left, const Iterator &right){
            return left._ptr!=right._ptr;
        }
        //true if left == right
        friend bool operator==(const Iterator &left, const Iterator &right){
            return left._ptr==right._ptr;
        }
        //member operator:      ++it; or  ++it = new_value
        Iterator &operator++(){
            _ptr=_ptr->_next;
            return *this;
        }
        //friend operator: it++
        friend Iterator operator++(Iterator &it, int unused){
            assert(it._ptr);
            Iterator temp=it;
            it._ptr=(it._ptr->_next);
            return temp;
        }

    private:
        node<T>* _ptr;                     //pointer being encapsulated
    };

    Stack();
    Stack(const Stack<T>& copyMe);
    ~Stack();
    Stack<T>& operator=(const Stack<T>& RHS);
    
    int size() const;
    bool empty();
    
    T top();
    T pop();
    void push(T item);

    Iterator begin() const;                //Iterator to the head node
    Iterator end() const;                  //Iterator to NULL

    template<typename TT>
    friend ostream& operator<<(ostream& outs, const Stack<TT>& printMe){
        outs<<"[SIZE "<<printMe._size<<"]\t";
        _print_list(printMe._top,outs);
        return outs;
    }

private:
    node<T>* _top;
    int _size;
};

//----------------------------------------------------------------------------

template <typename T>
Stack<T>::Stack(){
    _top=nullptr;
    _size=0;
}
template <typename T>
Stack<T>::Stack(const Stack<T>& copyMe){
    _top=_copy_list(copyMe._top);
    _size=copyMe._size;
}
template <typename T>
Stack<T>::~Stack(){
    _clear_list(_top);
    delete _top;
}
template <typename T>
Stack<T>& Stack<T>::operator=(const Stack<T> &RHS){
    if(this==&RHS)
        return *this;
    _clear_list(_top);
    delete _top;
    _top=_copy_list(RHS._top);
    _size=RHS._size;
    return *this;
}

template <typename T>
T Stack<T>::top(){
    assert(_size>0);
    return _top->_item;
}
template <typename T>
T Stack<T>::pop(){
    assert(_size>0);
    _size--;
    return _delete_node(_top,_top);
}
template <typename T>
bool Stack<T>::empty(){
    return _size==0;    //or _top==nullptr;
}
template <typename T>
void Stack<T>::push(T item){
    node<T>* ignore=_insert_head(_top,item);
    _size++;
}

template <typename T>
typename Stack<T>::Iterator Stack<T>::begin() const{
    return Iterator(_top);
}
template <typename T>
typename Stack<T>::Iterator Stack<T>::end() const{
    return Iterator();
}

template <typename T>
int Stack<T>::size() const{
    return _size;
}

//----------------------------------------------------------------------------

#endif //STACK_H