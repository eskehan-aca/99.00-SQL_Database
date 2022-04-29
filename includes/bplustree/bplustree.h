#ifndef BPLUSTREE_H
#define BPLUSTREE_H
#include "btree_array_funcs.h"
#include "../stl_utils/vector_utilities.h"

template <typename T>
class BPlusTree{
public:
    class Iterator{
    public:
        friend class BPlusTree;
        Iterator(BPlusTree<T>* it=nullptr, int key_ptr=0):_it(it), _key_ptr(key_ptr){
            if(_it){
                assert(_it->is_leaf());
                // assert(_key_ptr<=_it->_child_count);
            }
        }
        T operator *(){
            assert(_it);
            return _it->_data[_key_ptr];
        }
        Iterator operator++(int un_used){
            assert(_it);
            Iterator prev(_it,_key_ptr);
            _key_ptr++;
            if(_key_ptr>=_it->_data_count){
                _key_ptr=0;
                _it=_it->_next;
            }
            return prev;
        }
        Iterator operator++(){
            assert(_it);
            _key_ptr++;
            if(_key_ptr>=_it->_data_count){
                _key_ptr=0;
                _it=_it->_next;
            }
            return *this;
        }
        friend bool operator ==(const Iterator& lhs, const Iterator& rhs){
            return lhs._it==rhs._it && (!lhs._it || lhs._key_ptr==rhs._key_ptr);
        }
        friend bool operator !=(const Iterator& lhs, const Iterator& rhs){
            return !(lhs==rhs);
        }
        void print_Iterator(){
            if(_it){
                cout<<"iterator: ";
                print_array(_it->_data, _it->_data_count,_key_ptr);
            }
            else
                cout<<"iterator: NULL, key_ptr: "<<_key_ptr<<endl;
        }
        bool is_null(){return !_it;}
        void info(){
            cout<<endl<<"Iterator info:"<<endl;
            cout<<"key_ptr: "<<_key_ptr<<endl;
            cout<<"_it: "<<*_it<<endl;
        }

    private:
        BPlusTree<T>* _it;
        int _key_ptr;
    };

    BPlusTree(bool dups=false);
    BPlusTree(T *a, int size, bool dups=false);
    BPlusTree(const BPlusTree<T>& other);
    ~BPlusTree();
    BPlusTree<T>& operator =(const BPlusTree<T>& RHS);

    void clear_tree();             //clear this object (delete all nodes etc.)
    void copy_tree(const BPlusTree<T>& other);  //copy other into this object
    void copy_tree(const BPlusTree<T>& other, BPlusTree<T>* &last_leaf);

    void insert(const T& entry);         //insert entry into the tree
    void remove(const T& entry);         //remove entry from the tree

    T& get(const T& entry);              //return a reference to entry
    const T& get(const T& entry)const;   //return a reference to entry
    T& get_existing(const T& entry);     //return a reference to entry
    const T& get_existing(const T& entry)const;     //return a reference to entry
    
    int size() const;                    //count the number of elements
    bool empty() const;                  //true if the tree is empty
    bool contains(const T& entry) const; //true if entry can be found
    bool is_valid();

    Iterator find(const T& key);         //return an iterator to this key.  NULL if not there.
    Iterator lower_bound(const T& key);  //return first that goes NOT BEFORE key entry or next if does not exist: >= entry
    Iterator upper_bound(const T& key);  //return first that goes AFTER key exist or not, the next entry  >entry
    Iterator begin();
    Iterator end();

    string in_order();
    string pre_order();
    string post_order();
    void print_tree(int level=0, ostream &outs=cout) const;
    friend ostream& operator<<(ostream& outs, const BPlusTree<T>& print_me){
        if(&print_me)
            print_me.print_tree(0,outs);
        else
            outs<<"NULLPTR"<<endl;
        return outs;
    }
    ostream& list_keys(Iterator from=NULL, Iterator to=NULL){
        if(from==NULL) 
            from=begin();
        if(to==NULL)
            to=end();
        for(Iterator _it=from; _it!=to; _it++)
            cout<<*_it<<" ";
        return cout;
    }
private:
    static const int MINIMUM = 1;
    static const int MAXIMUM = 2*MINIMUM;

    bool _dups_ok;                      //true if duplicate keys are allowed
    int _data_count;                    //number of data elements
    int _child_count;                   //number of children
    T _data[MAXIMUM+1];                 //holds the keys
    BPlusTree* _subset[MAXIMUM+2];      //subtrees
    BPlusTree* _next;                   //ptr
    
    bool is_leaf() const {return _child_count==0;} //true if this is a leaf node

    T* find_ptr(const T& entry);         //return a pointer to this key. NULL if not there.

    //insert element functions
    void loose_insert(const T& entry);   //allows MAXIMUM+1 data elements in the root
    void fix_excess(int i);              //fix excess in child i

    //remove element functions:
    void loose_remove(const T& entry);  //allows MINIMUM-1 data elements in the root
    BPlusTree<T>* fix_shortage(int i);  //fix shortage in child i and return the smallest key in this subtree
    BPlusTree<T>* get_smallest_node();
    void get_smallest(T& entry);      //entry := leftmost leaf
    void get_biggest(T& entry);       //entry := rightmost leaf
    void remove_biggest(T& entry);    //remove the biggest child of tree->entry
    void transfer_left(int i);        //transfer one element LEFT from child i
    void transfer_right(int i);       //transfer one element RIGHT from child i
    BPlusTree<T> *merge_with_next_subset(int i); //merge subset i with  i+1

};

//---------------------------------------------------------------------
//            C T O R  /  B I G   T H R E E
//---------------------------------------------------------------------
template <typename T>
BPlusTree<T>::BPlusTree(bool dups):_dups_ok(dups),_data_count(0),_child_count(0),_next(NULL){
    for(int i=0;i<MAXIMUM+2;i++)
        _subset[i]=nullptr;
}
template <typename T>
BPlusTree<T>::BPlusTree(T *a, int size, bool dups):_dups_ok(dups),_data_count(0),_child_count(0),_next(NULL){
    for(int i=0;i<MAXIMUM+2;i++)
        _subset[i]=nullptr;
    for(int i=0; i<size; i++){
        insert(*(a+i));
    }
}
template <typename T>
BPlusTree<T>::BPlusTree(const BPlusTree<T>& other):_dups_ok(other._dups_ok),_data_count(0),_child_count(0),_next(NULL){
    copy_tree(other);
}
template <typename T>
BPlusTree<T>::~BPlusTree(){
    clear_tree();
}
template <typename T>
BPlusTree<T>& BPlusTree<T>::operator =(const BPlusTree<T>& rhs){
    if(this!=&rhs){
        clear_tree();
        copy_tree(rhs);
        assert(this->size()==rhs.size());
    }
    return *this;
}

//---------------------------------------------------------------
//      C L E A R   /  C O P Y
//---------------------------------------------------------------
template <typename T>
void BPlusTree<T>::clear_tree(){
    T data;
    while(_data_count>0){
        delete_item(_data,0,_data_count,data);
    }
    while(_child_count>0){
        _subset[--_child_count]->clear_tree();
        delete _subset[_child_count];
        _subset[_child_count]=nullptr;
    }
}
template <typename T>
void BPlusTree<T>::copy_tree(const BPlusTree<T>& other){
    //assumes this btree is empty. [memory leaks otherwise]
    assert(empty());
    BPlusTree<T>* last_leaf=nullptr;
    copy_tree(other,last_leaf);
}
template <typename T>
void BPlusTree<T>::copy_tree(const BPlusTree<T>& other, BPlusTree<T>* &last_leaf){
    copy_array(_data, other._data, _data_count, other._data_count);
    if(other.is_leaf()){
        if(last_leaf)
            last_leaf->_next=this;
        last_leaf=this;
    }
    while(_child_count<other._child_count){
        _subset[_child_count]=new BPlusTree<T>(_dups_ok);
        _subset[_child_count]->copy_tree(*(other._subset[_child_count]));
        _child_count++;
    }
}

//---------------------------------------------------------------------
//           G E T
//---------------------------------------------------------------------
template <typename T>
T& BPlusTree<T>::get(const T& entry){
    //If entry is not in the tree, add it to the tree
    if(!contains(entry))
        insert(entry);
    assert(contains(entry));
    return get_existing(entry);
}
template <typename T>
const T& BPlusTree<T>::get(const T& entry) const{
    assert(contains(entry));
    return get_existing(entry);
}
template <typename T>
T& BPlusTree<T>::get_existing(const T& entry) {
/* ---------------------------------------------------------------
 * i = first_ge
 *       |      found         |    !found         |
 * ------|--------------------|-------------------|
 * leaf  |  you're done       | it's not there    |
 *       |                    |                   |
 * ------|--------------------|-------------------|
 * !leaf | subset[i+1]->get() |  subset[i]->get() |
 *       | (inner node)       |                   |
 * ------|--------------------|-------------------|
 *       |                    |                   |
 * --------------------------------------------------------------- */
    const bool debug=false;
    int i=first_ge(_data, _data_count, entry);
    bool found=(i<_data_count && _data[i]==entry);
    if(is_leaf())
        if(found){
            return _data[i];
        }
        else{
            //or -> assert(contains(entry));
            if (debug) cout<<"get_existing was called with nonexistent entry"<<endl;
            assert(found);
        }
    if(found)                               //inner node
        return _subset[i+1]->get(entry);    //or just return true?
    else                                    //not found yet
        return _subset[i]->get(entry);
}
template <typename T>
const T& BPlusTree<T>::get_existing(const T& entry) const{
/* ---------------------------------------------------------------
 * i = first_ge
 *       |      found         |    !found         |
 * ------|--------------------|-------------------|
 * leaf  |  you're done       | it's not there    |
 *       |                    |                   |
 * ------|--------------------|-------------------|
 * !leaf | subset[i+1]->get() |  subset[i]->get() |
 *       | (inner node)       |                   |
 * ------|--------------------|-------------------|
 *       |                    |                   |
 * --------------------------------------------------------------- */
    const bool debug=false;
    int i=first_ge(_data, _data_count, entry);
    bool found=(i<_data_count && _data[i]==entry);
    if(is_leaf())
        if(found){
            return _data[i];
        }
        else{
            if (debug) cout<<"get_existing was called with nonexistent entry"<<endl;
            assert(found);
        }
    if(found)                               //inner node
        return _subset[i+1]->get(entry);    //or just return true?
    else                                    //not found yet
        return _subset[i]->get(entry);
}

//---------------------------------------------------------------------
//           E M P T Y / S I Z E / V A L I D A T I O N
//---------------------------------------------------------------------
template <typename T>
int BPlusTree<T>::size() const{
    int sum=_data_count;
    for(int i=0;i<_child_count; i++){
        sum+=_subset[i]->size();
    }
    return sum;
}
template <typename T>
bool BPlusTree<T>::empty() const{
    return _data_count==0 && _child_count==0;
}
template <typename T>
bool BPlusTree<T>::is_valid(){
    return true;
}

//---------------------------------------------------------------------
//           C O N T A I N S / F I N D 
//---------------------------------------------------------------------
template <typename T>
bool BPlusTree<T>::contains(const T& entry) const{
    int i=first_ge(_data, _data_count, entry);
    bool found=(i<_data_count && _data[i]==entry);
    if(found)
        return true;
    else if(i<_child_count)                          
        return _subset[i]->contains(entry);
    return false;
}
template <typename T>
T* BPlusTree<T>::find_ptr(const T& entry){
    int i=first_ge(_data, _data_count, entry);
    bool found=(i<_data_count && _data[i]==entry);
    if(is_leaf())
        if(found){
            return &_data[i];
        }
        else{
            cout<<"get_existing was called with nonexistent entry"<<endl;
            assert(found);
        }
    if(found)                               
        return _subset[i+1]->find_ptr(entry);
    else                                    
        return _subset[i]->find_ptr(entry);
}

//------------------------------------------------
//          I N S E R T
//------------------------------------------------
template <typename T>
void BPlusTree<T>::insert(const T& entry){
    const bool debug=false;
    if(debug)   cout<<"inserting "<<entry<<endl;
    loose_insert(entry);
    if(_data_count>MAXIMUM){
        BPlusTree<T>* temp=new BPlusTree<T>();
        copy_array(temp->_data, _data, temp->_data_count,_data_count);
        copy_array(temp->_subset,_subset,temp->_child_count,_child_count);
        _data_count=0;
        _child_count=0;
        _subset[_child_count++]=temp;
        assert(_child_count==1);
    }
    fix_excess(0);
    if(debug)   cout<<"bt:"<<endl<<*this<<endl;
}
template <typename T>
void BPlusTree<T>::loose_insert(const T& entry){
    /*      |   found          |   !found         |
      ------|------------------|------------------|-------
      leaf  | a. dups? +       | c: insert entry  |
            |                  |    at data[i]    |
      ------|------------------|------------------|-------
            | b.               | d.               |
            |subset[i+1]->     | subset[i]->      |
      !leaf | loose_insert(i+1)|  loose_insert(i) |
            |fix_excess(i+1)   | fix_excess(i)    |
            |                  |                  |
      ------|------------------|------------------|-------
    */
    const bool debug=false;
    int i=first_ge(_data, _data_count, entry);
    bool found=(i<_data_count && i>=0 && _data[i]==entry);
    if(debug)   cout<<"i: "<<i<<endl<<"found: "<<boolalpha<<found<<endl;
    if(found){
        if(is_leaf()){
            if(_dups_ok)
                insert_item(_data,i,_data_count,entry);
        }
        else{
            _subset[i+1]->loose_insert(entry);
            fix_excess(i+1);
        }
    }
    else{
        if(is_leaf()){
            insert_item(_data,i,_data_count,entry);
        }
        else{
            _subset[i]->loose_insert(entry);
            fix_excess(i);
        }
    }
}
template <typename T>
void BPlusTree<T>::fix_excess(int i){
    //this node's child i has one too many items: 3 steps:
    //1. add a new subset at location i+1 of this node
    //2. split subset[i] (both the subset array and the data array) and move half into
    //      subset[i+1] (this is the subset we created in step 1.)
    //3. detach the last data item of subset[i] and bring it and insert
    //      it into this node's data[]
    //Note that this last step may cause this node to have too many items.
    //  This is OK. This will be dealt with at the higher recursive level. (my parent will fix it!)
    if(i>=_child_count)
        return;
    const bool debug=false;
    assert(i<_child_count);
    assert(i>=0);
    if(_subset[i]->_data_count>MAXIMUM){
        if(debug){
            cout<<"\tin if, i="<<i<<"{"<<endl;
            cout<<"_datacount:"<<_data_count<<endl;
            cout<<"_childcount:"<<_child_count<<endl;
            cout<<*this<<endl;
            cout<<"}"<<endl;
        }
        T item;
        insert_item(_subset, i+1, _child_count, new BPlusTree<T>());
        split(_subset[i]->_data, _subset[i]->_data_count, _subset[i+1]->_data, _subset[i+1]->_data_count);
        split(_subset[i]->_subset, _subset[i]->_child_count, _subset[i+1]->_subset, _subset[i+1]->_child_count);
        detach_item(_subset[i]->_data, _subset[i]->_data_count, item);
        insert_item(_data, i,_data_count, item);
        if(_subset[i]->is_leaf()){
            _subset[i+1]->_next=_subset[i]->_next;
            _subset[i]->_next=_subset[i+1];
            insert_item(_subset[i+1]->_data,0,_subset[i+1]->_data_count,item);
        }
    }
}

//------------------------------------------------
//          R E M O V E
//------------------------------------------------
template <typename T>
void BPlusTree<T>::remove(const T& entry){
    if(empty()) return;
    loose_remove(entry);
    if(_data_count==0 && _child_count>0){
        BPlusTree* temp=_subset[0];
        copy_array(_data,temp->_data,_data_count,temp->_data_count);
        copy_array(_subset,temp->_subset,_child_count,temp->_child_count);
        temp->_child_count=0;
        temp->_data_count=0;
        delete temp;
    }
}
template <typename T>
void BPlusTree<T>::loose_remove(const T& entry){
     /*      |   !found               |   found                 |
       ------|------------------------|-------------------------|
       leaf  |  a: nothing            | b: delete               |
             |     to do              |    target               |
       ------|------------------------|-------------------------|
       !leaf | c:                     | d: B_PLUS_TREE          |
             |  [i]->  loose_remove   |   [i+1]-> loose_remove  |
             |  fix_shortage(i)       | fix_shortage(i+1) [...] |
       ------|------------------------|-------------------------|
     */
    const bool debug=false;
    T item, smallest;
    int i=first_ge(_data, _data_count, entry);
    bool found=(i<_data_count && i>=0 && _data[i]==entry);
    if(is_leaf()&&!found){      //a
        return;
    }
    if(is_leaf()&&found){       //b
        delete_item(_data, i, _data_count, item);
    }
    if(!is_leaf()&&!found){     //c
        _subset[i]->loose_remove(entry);
        fix_shortage(i);
    }
    if(!is_leaf()&&found){      //d
        assert(i<_child_count-1);
        /*  After fix_shortage(i+1):
        *   Search for entry in data[ ] and if found, replace it with smallest.
        *   otherwise, search for it in subset[i]->data[ ] and replace it. 
        *   that's where the key (entry) will end up after fix order returns */
        _subset[i+1]->loose_remove(entry);
        BPlusTree* rtrn=fix_shortage(i+1);
        i=first_ge(_data, _data_count, entry);
        found=(i<_data_count && i>=0 && _data[i]==entry);
        if(debug){
            cout<<*this<<endl;
            cout<<"i: "<<i<<endl;
            cout<<"childcount: "<<_child_count<<endl;
            cout<<"found: "<<boolalpha<<found<<endl;
        }
        if(found){
            if(i+1<_child_count){
                _subset[i+1]->get_smallest(smallest);
                if(debug)   cout<<"replacing data "<<_data[i]<<" with "<<smallest<<endl;
                _data[i]=smallest;
            }
        }
        else if(rtrn){ //entry not in data
            i=first_ge(rtrn->_data, rtrn->_data_count, entry);
            found=(i<rtrn->_data_count && i>=0 && rtrn->_data[i]==entry);
            if(found && i+1<rtrn->_child_count){
                rtrn->_subset[i+1]->get_smallest(smallest);
                rtrn->_data[i]=smallest;
                if(debug)    cout<<"replacing subset data"<<rtrn->_data[i]<<" with "<<smallest<<endl;
            }
        }
        //key was removed from subset[i+1]:
        //  1. shortage: find entry in data or subset[i+1]->data and replace it with subset[i+1]->smallest
        //  2. no shortage: replace data[i] with subset[i+1]->smallest
    }
}
template <typename T>
BPlusTree<T>* BPlusTree<T>::fix_shortage(int i){
    /* fix shortage in subtree i:
     * if child i+1 has more than MINIMUM, (borrow from right) transfer / rotate left(i+1)
     * elif child i-1 has more than MINIMUM, (borrow from left) transfer /rotate right(i-1)
     * elif there is a left sibling, merge with prev child: merge(i-1)
     * else merge with next (right) child: merge(i) returns a pointer to the fixed_subset
     */  
    const bool debug=false;
    if(i<0 || i>=_child_count){
        if(debug)    cout<<"subset "<<i<<" not in shortage|L"<<__LINE__<<endl;
        return nullptr;
    }
    else if(_subset[i]->_data_count>=MINIMUM)
        return _subset[i];
    else if(debug){
        cout<<"fix shortage on subset[i="<<i<<"] in data[0]="<<_data[0]<<", childcount="<<_child_count<<endl;
        cout<<"subset["<<i<<"] data_count="<<_subset[i]->_data_count<<endl;
        cout<<"subset["<<i<<"] child_count="<<_subset[i]->_child_count<<endl;
        cout<<*this<<endl;
    }
    if(i+1<_child_count && _subset[i+1]->_data_count>MINIMUM){
        if(debug) cout<<"pull from right subset (transfer_left)"<<endl;
        transfer_left(i);
    }
    else if(i>0 && _subset[i-1]->_data_count>MINIMUM){
        if(debug) cout<<"pull from left subset (transfer_right)"<<endl;
        transfer_right(i);
    }
    else if(i-1>=0 && _child_count>i){
        if(debug) cout<<"merging with prev child (left)"<<endl;
        merge_with_next_subset(i-1);
        return _subset[i-1];
    }
    else if(_child_count>i+1){
        if(debug) cout<<"merging with next child (right)"<<endl;
        merge_with_next_subset(i);
    }
    else{
        if(debug){
            cout<<"fix_shortage, L|"<<__LINE__<<endl;
            cout<<*this<<endl;
            cout<<"i="<<i<<endl;
            cout<<"subset["<<i<<"] data_count="<<_subset[i]->_data_count<<endl;
            cout<<"subset["<<i<<"] child_count="<<_subset[i]->_child_count<<endl;
            cout<<"subset["<<i<<"] decrementing child_count"<<endl;
        }
        if(_subset[i]->_data_count==0 && _subset[i]->_child_count==0)
            _child_count--;
    }
    return _subset[i];
}
template <typename T>
BPlusTree<T>* BPlusTree<T>::get_smallest_node(){
    if(is_leaf())
        return this;
    else
        return _subset[0]->get_smallest_node();
}
template <typename T>
void BPlusTree<T>::get_biggest(T& entry){
    if(is_leaf())
        entry=_data[_data_count-1];
    else
        return _subset[_child_count]->get_biggest();
}
template <typename T>
void BPlusTree<T>::get_smallest(T& entry){
    if(is_leaf()){
        if(_data_count>0)
            entry=_data[_data_count-1];
    }
    else
        _subset[0]->get_smallest(entry);
}
template <typename T>
void BPlusTree<T>::remove_biggest(T& entry){
    //remove the biggest child of tree->entry
    if(is_leaf()){
        if(_data_count>0)
            detach_item(_data,_data_count,entry);
    }
    else{
        _subset[_child_count-1]->remove_biggest(entry);
        fix_shortage(_child_count-1);
    }
}
template <typename T>
BPlusTree<T>* BPlusTree<T>::merge_with_next_subset(int i){
    /* ----------------------------------------------------------------------
     *  Merge subset[i] with subset [i+1] REMOVE data[i];
     *  non leaf: same as BTree
     *  leaves  : delete but do not bring down data[i]
     * ----------------------------------------------------------------------
     *
     *   1. remove data[i] from this object
     *   2. if not a leaf, append it to child[i]->data:
     *   3. Move all data items from subset[i+1]->data to right of subset[i]->data
     *   4. Move all subset pointers from subset[i+1]->subset to right of subset[i]->subset
     *   5. delete subset[i+1] (store in a temp ptr)
     *   6. if a leaf, point subset[i]->next to temp_ptr->next
     *   6. delete temp ptr
     *
     *
     * non-leaf nodes: (same as BTree)
     * ------------------
     *  i = 0:
     *             [50  100]
     *          /      |     \
     *  [  ]         [75]       ....
     *    |         /    \
     *   [a]      [b]    [c]
     *
     *  bring down data[i], merge it with subset[i] and subset[i+1]:
     *      then, get rid of subset[i+1]
     *             [100]
     *               |
     *            [50|75]       ....
     *            /  |  \
     *         [a]  [b]  [c]
     *
     *
     *
     * leaf node:
     * ----------------
     * Exactly the same, but do not attach the deleted data[i] to subset[i]->data[ ]
     *
     *  i = 0 : merge 5 and [()]
     *        [7 | 10]
     *      /    |     \
     *  [5]->   [()]->  [10]
     *
     *  Delete data[i] (7), merge subset[i] and subset[i+1]
     *      WITHOUT bringing down data[i]
     *
     *        [10]
     *      /      \
     *  [5]->      [10]
     *
     *
     * i = 1 merge 7 and [()]
     *        [7 | 10]
     *      /    |     \
     *  [5]->   [7]->  [()]
     *
     *  Delete data[i] (10), merge subset[i] and subset[i+1]
     *      WITHOUT bringing down data[i]
     *
     *        [7]
     *      /     \
     *  [5]->      [7]
     */
    const bool debug=false;
    T item=T();
    BPlusTree* child=nullptr;
    delete_item(_data, i, _data_count, item);
    if(!_subset[i]->is_leaf()){
        attach_item(_subset[i]->_data, _subset[i]->_data_count, item);
    }
    merge(_subset[i]->_data, _subset[i]->_data_count, _subset[i+1]->_data, _subset[i+1]->_data_count);
    merge(_subset[i]->_subset, _subset[i]->_child_count, _subset[i+1]->_subset, _subset[i+1]->_child_count);
    delete_item(_subset, i+1,_child_count, child);
    if(_subset[i]->is_leaf())
        _subset[i]->_next=child->_next; 
    delete child;
    return _subset[i];

}
template <typename T>
void BPlusTree<T>::transfer_left(int i){
    /* --------------------------------------------------------------
     * transfer_left: from subset[i] to subset[i-1]
     *      non-leaf: same as BTree
     *      leaves  : rotate and leave and update data[i-1] with subset[i]->data[0]
     *                  leftmost key after you borrowed goes to replace data[i-1]
     * --------------------------------------------------------------
     *
     * non-leaf keys: BTree's rotate_left
     * ---------------
     *
     * (0 < i < _child_count) and (subset[i]->_data_count > MINIMUM)
     * subset[i-1] has only MINIMUM - 1 entries.
     *
     * item transfers from child[i] to child [i-1]
     *
     * FIRST item of subset[i]->data moves up to data to replace data[i-1],
     * data[i-1] moves down to the RIGHT of subset[i-1]->data
     *
     *  i = 1:
     *              [50  100]
     *  [  ]         [65 75]       ....
     *   [a]      [b]  [c]  [d]
     *
     *  65 move up to replace 50 (data[i])
     *  65's child (its child 0) moves over to be the future child of 50
     *  50 moves down to the right of subset[i]->data
     *
     *               [65 100]
     *   [50]         [ 75 ]       ....
     * [a]  [b]      [c]  [d]
     *
     * ****This does not belong here:
     * last item of subset[i-1]->data moves up to data to replace data[i],
     * data[i] moves down to the RIGHT of subset[i]->data
     *
     * leaf keys:
     * -------------
     *  fix_shortage(0) calls trnasfer_left(1) so, i = 1
     *          [7 | 10]
     *        /    |     \
     * [( )]<=[(7) | 8]  [10]
     *
     * 1. transfer subset[i(1)]->data[0] to end of subset[i-1]
     *        [7 | 10]
     *      /    |     \
     *   [7]    [8]   [10]
     *
     * 2. update parent of the transfered subset: data[i-1 (0)] = subset[i(1)]->data[0]
     *
     *        [(8) | 10]
     *      /      |     \
     *   [7]     [(8)]   [10]
     */
    assert(0<=i && i<=_child_count);
    assert(_subset[i+1]->_data_count>MINIMUM);
    assert(_subset[i]->_data_count==MINIMUM-1);
    //move item from _data to child
    insert_item(_subset[i]->_data,_subset[i]->_data_count,_subset[i]->_data_count,_data[i]);
    //replace moved item
    T item=T();
    delete_item(_subset[i+1]->_data,0,_subset[i+1]->_data_count,item);
    _data[i]=item;
    //if leaf
    if(_subset[i+1]->_child_count<=0){
        _data[i]=_subset[i+1]->_data[0];
    }
    else{
        BPlusTree<T>* child;
        delete_item(_subset[i+1]->_subset,0,_subset[i+1]->_child_count,child);
        attach_item(_subset[i]->_subset,_subset[i]->_child_count,child);
    }
}
template <typename T>
void BPlusTree<T>::transfer_right(int i){
    /* --------------------------------------------------------------
     * transfer right from subset[i] to subset[i+1]
     *      non-leaf: same as BTree
     *      leaves  : rotate and leave a 'trace':
     *              data[i] is replaced with subset[i+1]->data[0]
     *              after you borrowed the key,
     * --------------------------------------------------------------
     *
     * non-leaf keys: BTree's rotate_right
     * ---------------
     * (i < _child_count - 1) and (subset[i-1]->_data_count > MINIMUM)
     * subset[i+ 1] has only MINIMUM - 1 entries.
     *
     * item transfers from child[i] to child [i+1]
     *
     * LAST item of subset[i]->data moves up to data to replace data[i],
     * data[i] moves down to the LEFT of subset[i+1]->data
     *
     * i = 1
     *                     [50 100]
     *      [20 30]        [65 75]          [ ]
     *  [..] [..] [..]   [a] [b] [c]        [..]
     *
     *  75 moves up to replace 100 (data[i])
     *  75's child (its last child) moves over to be the (child 0) child of 100
     *  100 moves down to subset[i]->data
     *
     *                     [50 75]
     *      [20 30]          [65]          [100]
     *  [..] [..] [..]     [a] [b]        [c] [..]
     *
     * last item of subset[i-1]->data moves up to data,
     * data[i] moves down to the RIGHT of subset[i]->data
     *
     * leaf keys:
     * -------------
     *  fix_shortage(2) calls trnasfer_right(1) so, i = 1 subset[i] is ([7|8])
     *  subset[i+1] is the one being fixed.
     *        [7 | 10]
     *      /    |     \
     * [5|6]->[7 | (8)]=>[()]
     *
     * 1. transfer subset[i(1)]->data[last item] to left of subset[i+1]->data
     *        [7 | (10)]
     *      /    |     \
     *   [5|6]->[7]-> [8]
     *
     * 2. update parent of the transfered subset: data[i (1)] = subset[i+1(2)]->data[0]
     *
     *        [7 | 8]
     *      /    |    \
     *   [5|6]->[7]-> [8]
     */

    assert(i<_child_count);
    assert(_subset[i-1]->_data_count>MINIMUM);
    assert(_subset[i]->_data_count==MINIMUM-1);
    //move item from _data to child
    insert_item(_subset[i]->_data,0,_subset[i]->_data_count,_data[i-1]);
    //replace moved item
    T item=T(), undo;
    detach_item(_subset[i-1]->_data,_subset[i-1]->_data_count,item);
    _data[i-1]=item;
    //shift subset
    if(_subset[i]->_child_count>0){
        BPlusTree<T>* child;
        detach_item(_subset[i-1]->_subset,_subset[i-1]->_child_count,child);
        insert_item(_subset[i]->_subset,0,_subset[i]->_child_count,child);
    }
    //if leaf (_subset[i]->_child_count<0)
    else{
        detach_item(_subset[i]->_data,_subset[i]->_data_count,undo); //undo first insert
        insert_item(_subset[i]->_data,0,_subset[i]->_data_count,item);
        _data[i-1]=_subset[i]->_data[0];
    }
}

//---------------------------------------------------------------------
//            I T E R A T O R S
//---------------------------------------------------------------------
template <typename T>
typename BPlusTree<T>::Iterator BPlusTree<T>::find(const T& entry){
    int i=first_ge(_data, _data_count, entry);
    bool found=(i<_data_count && _data[i]==entry);
    if(is_leaf())
        if(found){
            return Iterator(this,i);
        }
        else{
            cout<<"get_existing was called with nonexistent entry"<<endl;
            return Iterator();
        }
    if(found)                               
        return _subset[i+1]->find(entry);
    else                                    
        return _subset[i]->find(entry);
}
template <typename T>
typename BPlusTree<T>::Iterator BPlusTree<T>::lower_bound(const T& key){
    for(Iterator it=begin(); it!=end(); it++){
        if(*it>=key)
            return it;
    }
    return Iterator();
}
template <typename T>
typename BPlusTree<T>::Iterator BPlusTree<T>::upper_bound(const T& key){
    for(Iterator it=begin(); it!=end(); it++){
        if(*it>key)
            return it;
    }
    return Iterator();
}
template <typename T>
typename BPlusTree<T>::Iterator BPlusTree<T>::begin(){
    return Iterator(get_smallest_node());
}
template <typename T>
typename BPlusTree<T>::Iterator BPlusTree<T>::end(){
    return Iterator();
}

//---------------------------------------------------------------------
//           P R I N T I N G
//---------------------------------------------------------------------
template <typename T>
string BPlusTree<T>::in_order(){
    string s="";    
    for(int i=0; i<max(_data_count,_child_count);i++){
        if(i<_child_count)
            s+=_subset[i]->in_order();
        if(i<_data_count)
            s+=to_string(_data[i])+"|";
    }
    return s;
}
template <typename T>
string BPlusTree<T>::pre_order(){
    string s="";
    for(int i=0; i<max(_data_count,_child_count);i++){
        if(i<_data_count)
            s+=to_string(_data[i])+"|";
        if(i<_child_count)
            s+=_subset[i]->pre_order();
    }
    return s;
}
template <typename T>
string BPlusTree<T>::post_order(){
    string s="";
    if(_child_count>0)   
        s+=_subset[0]->post_order();
    for(int i=0; i<_data_count;i++){
        if(i+1<_child_count)
            s+=_subset[i+1]->post_order();
        s+=to_string(_data[i])+"|";
    }
    return s;
}
template <typename T>
void BPlusTree<T>::print_tree(int level, ostream &outs) const{
    if(_child_count>0)
        _subset[_child_count-1]->print_tree(level+1,outs);
    if(_next && level!=0)
        outs<<setw(6*level)<<"^"<<endl;
    outs<<setw(6*level)<<"=="<<endl;
    if(_data_count==0){
        outs<<setw(6*level+5)<<"no data"<<endl;
        outs<<setw(6*level)<<"=="<<endl;
        for(int i=_child_count;i>0; i--){
                _subset[i-1]->print_tree(level+1,outs);        
            if(i==0)
                outs<<setw(6*level)<<"=="<<endl;
        }
    }
    for(int i=_data_count-1;i>=0; i--){
        outs<<setw(6*level)<<_data[i]<<endl;
        if(i==0)
            outs<<setw(6*level)<<"=="<<endl;
        if(i<_child_count)
            _subset[i]->print_tree(level+1,outs);        
    }
}

#endif // BPLUSTREE_H