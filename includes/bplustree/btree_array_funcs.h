#ifndef BTREE_ARRAY_FUNCS_H
#define BTREE_ARRAY_FUNCS_H
#include <assert.h> 
#include <iostream>
#include <iomanip> 
#include <string> 
#include <vector>
#include <stack>
#include <cmath>
#include <set>

using namespace std;

template <typename T>
T maximal(const T& a, const T& b);                     //return the larger of the two items

template <typename T>
void swap(T& a, T& b);                                 //swap the two items

template <typename T>
int index_of_maximal(T data[], int n);                 //return index of the largest item in data

template <typename T>
void ordered_insert(T data[], int& n, T entry);        //insert entry into the sorted array data with length n

template <typename T>
int first_ge(const T data[], int n, const T& entry);   //return the first element in data that is not less than entry

template <typename T>
void attach_item(T data[], int& n, const T& entry);    //append entry to the right of data

template <typename T>
void insert_item(T data[], int i, int& n, T entry);    //insert entry at index i in data

template <typename T>
void detach_item(T data[], int& n, T& entry);          //remove the last element in data and place it in entry

template <typename T>
void delete_item(T data[], int i, int& n, T& entry);   //delete item at index i and place it in entry

template <typename T>
void merge(T data1[], int& n1, T data2[], int& n2);   //append data2 to the right of data1

template <typename T>
void split(T data1[], int& n1, T data2[], int& n2);   //move n/2 elements from the right of data1 and move to data2

template <typename T>
void copy_array(T dest[], const T src[], int& dest_size, int src_size);//copy src[] into dest[]

template <typename T>
void print_array(const T data[], int n, int pos = -1);  //print array data

template <typename T>
bool is_gt(const T data[], int n, const T& item);       //item > all data[i]

template <typename T>
bool is_le(const T data[], int n, const T& item);       //item <= all data[i]

//-------------- Additional array functions: ---------------------

template <typename T>
void shift_right(T *a, int &size, int shift_here);
template <typename T>
void shift_left(T* a, int& size, int shift_here);

//=============================================================================


template <typename T>//return the larger of the two items
T maximal(const T& a, const T& b){
    if(!b || a>b)
        return a;
    else return b;
}

template <typename T>//swap the two items
void swap(T& a, T& b){
    T temp=a;
    a=b;
    b=temp;
}

template <typename T>//return index of the largest item in data
int index_of_maximal(T data[], int n){
    print_array(data,n);
    int r=0;
    for(int i=1; i<n; i++){
        if(data[i]>data[r])
            r=i;
    }
    return r;
}

template <typename T>//insert entry into the sorted array data with length n
void ordered_insert(T data[], int& n, T entry){
    if(n==0){
        data[n++]=entry;
        return;
    }
    int i=first_ge(data,n,entry);
    return insert_item(data,i,n,entry);
} 

template <typename T>//return the first element in data that is not less than entry
int first_ge(const T data[], int n, const T& entry){
    for(int i=0; i<n; i++){
        if(data[i]>=entry)
            return i;
    }
    return n;
}

template <typename T>//append entry to the right of data
void attach_item(T data[], int& n, const T& entry){
    data[n]=entry;
    n++;
} 

template <typename T>//insert entry at index i in data
void insert_item(T data[], int i, int& n, T entry){
    shift_right(data,n,i);
    data[i]=entry;
}

template <typename T>//remove the last element in data and place it in entry
void detach_item(T data[], int& n, T& entry){
    n--;
    entry=data[n];
}  

template <typename T>//delete item at index i and place it in entry
void delete_item(T data[], int i, int& n, T& entry){
    entry=data[i];
    shift_left(data,n,i);
}

template <typename T>//append data2 to the right of data1
void merge(T data1[], int& n1, T data2[], int& n2){
    for(int i=0; i<n2; i++){
        attach_item(data1,n1,data2[i]);
    }
    n2=0;
}

template <typename T>//move n/2 elements from the right of data1 and move to data2
void split(T data1[], int& n1, T data2[], int& n2){
    int many=n1/2;
    n1-=many;
    n2+=many;
    for(int i=0; i<many; i++){
        data2[i]=data1[n1+i];
    }
}

template <typename T> //copy src[] into dest[]
void copy_array(T dest[], const T src[], int& dest_size, int src_size){
    while(dest_size!=0){
        T item;
        delete_item(dest,0,dest_size,item);
    }
        
    assert(dest_size==0);
    T *destWalker=dest;
    for(int i=0; i<src_size; i++){
        const T *srcWalker=new T(src[i]);   
        dest_size++;
        *destWalker=*srcWalker;
        destWalker++;
    }
}        

template <typename T> //print array data
void print_array(const T data[], int n, int pos){
    cout<<"size "<<n<<" pos "<<pos<<"   [";
    for(int i=0; i<n; i++){
        if(pos!=-1)
            i=pos;
        cout<<data[i]<<", ";
    }
    cout<<"]"<<endl;
}

template <typename T>//item > all data[i]
bool is_gt(const T data[], int n, const T& item){
    for(int i=0; i<n; i++){
        if(item<=data[i])
            return false;
    }
    return true;
}

template <typename T>//item <= all data[i]
bool is_le(const T data[], int n, const T& item){
    return !is_gt(data,n,item);
}

//========ARR FUNCTS 
template <typename T>
void shift_right(T *a, int &size, int shift_here){
    if(shift_here>size || shift_here<0)
        return; //out of bounds
    T* pointerShiftHere=a+shift_here;
    if(pointerShiftHere==nullptr)
        return; //out of bounds
    for (T *walker=a+size; walker>pointerShiftHere; walker--){
        //creating a pointer to copy the value to the left
        T *left_one = walker-1;
        *walker=*left_one;
    }
    size++;
}
template <typename T>
void shift_left(T* a, int& size, int shift_here){
    if(shift_here>size || shift_here<0||size==0)
        return; //out of bounds
    T* pointerShiftHere = a+shift_here;
    if(pointerShiftHere==nullptr)
        return; //out of bounds
    T *rightOne = pointerShiftHere+1;
    while(rightOne < a+size){
        //shift elements of block to the left starting at position shift_here
        *pointerShiftHere = *rightOne;
        //increment/decrement changes
        pointerShiftHere++;
        rightOne++;
    }
    size--;
}

#endif // BTREE_ARRAY_FUNCS_H
