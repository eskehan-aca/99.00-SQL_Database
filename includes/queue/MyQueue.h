#ifndef QUEUE_H
#define QUEUE_H
#include "../linked_list_functions/linkedlistfunctionsTemplated.h"

const bool queueDebug=false;

template <typename T>
class Queue{
public:
    class Iterator{
    public:
        friend class Queue;
        //CTOR
        Iterator(node<T>* p=NULL):_ptr(p){}    
        
        //dereference operator
        T &operator*(){
            assert(_ptr);
            return _ptr->_item;
        }
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
        node<T>* _ptr;
    };

    Queue();
    Queue(const Queue<T>& copyMe);
    ~Queue();
    Queue& operator=(const Queue<T>& RHS);


    T front();
    T back();
    T pop();
    void push(T item);

    Iterator begin() const;         //Iterator to the head node
    Iterator end() const;           //Iterator to NULL
    
    int size() const;
    bool empty();
    void print_pointers();
    
    template<typename TT>
    friend ostream& operator<<(ostream& outs, const Queue<TT>& printMe){
        outs<<"[SIZE "<<printMe._size<<"]\t";
        _print_list(printMe._front,outs);
        return outs;
    }

private:
    node<T>* _front;
    node<T>* _rear;
    int _size;
};

//----------------------------------------------------------------------------

template <typename T>
Queue<T>::Queue(){
    _front=nullptr;
    _rear=nullptr;
    _size=0;
}
template <typename T>
Queue<T>::Queue(const Queue<T>& copyMe){
    _front=nullptr;
    _rear=_copy_list(_front, copyMe._front);
    _size=copyMe._size;
}
template <typename T>
Queue<T>::~Queue(){
    _clear_list(_front);
    _size=0;
    delete _front;
    delete _rear;
}
template <typename T>
Queue<T>& Queue<T>::operator=(const Queue<T>& RHS){
    if(this==&RHS)
        return *this;
    _clear_list(_front);
    _rear=_copy_list(_front, RHS._front);
    _size=RHS._size;
    return *this;
}

template <typename T>
bool Queue<T>::empty(){
    return _front==nullptr;
}
template <typename T>
T Queue<T>::front(){
    assert(!empty() && _front!=nullptr);
    return _front->_item;
}
template <typename T>
T Queue<T>::back(){
    assert(!empty() && _rear!=nullptr);
    return _rear->_item;
}
template <typename T>
void Queue<T>::push(T item){
    _rear=_insert_after(_front, _rear, item);
    _size++;
}
template <typename T>
T Queue<T>::pop(){
    assert(!empty() && _front);
    _size--;
    if(_size==0){
        _rear=nullptr;
    }
    else if(_size==1){
        _rear=_front;
    }
    return _delete_node(_front,_front);
}
template <typename T>
typename Queue<T>::Iterator Queue<T>::begin() const{
    return Iterator(_front);
}            
template <typename T>
typename Queue<T>::Iterator Queue<T>::end() const{
    return Iterator();
}
template <typename T>
void Queue<T>::print_pointers(){
    cout<<"_front: "<<*_front<<"\t";
    cout<<"_rear: "<<*_rear<<endl;
}
template <typename T>
int Queue<T>::size() const{
    return _size;
}

//----------------------------------------------------------------------------

#endif //QUEUE_H