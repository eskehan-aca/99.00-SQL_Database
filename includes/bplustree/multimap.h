#ifndef MULTIMAP_H
#define MULTIMAP_H
#include "bplustree.h"

template <typename K, typename V>
struct MPair{
    K key;
    vector<V> value_list;
    //--------------------------------------------------------------------------------
    //     MPair CTORs: (must have these CTORs)
    //   - default CTOR / CTOR with a key and no value: this will create an empty vector
    //   - CTOR with a key AND a value: pushes the value into the value_list
    //   - CTOR with a key and a vector of values (to replace value_list)
    //--------------------------------------------------------------------------------
    MPair(const K& k=K()):key(k){}
    MPair(const K& k, const V& v):key(k){
        value_list.push_back(v);
    }
    MPair(const K& k, const vector<V>& vlist):key(k), value_list(vlist){}
    //--------------------------------------------------------------------------------

    //You'll need to overload << for your vector:
    friend ostream& operator <<(ostream& outs, const MPair<K, V>& print_me){
            outs<<"["<<print_me.key<<": "<<print_me.value_list<<"]";
        return outs;
    }
    friend bool operator ==(const MPair<K, V>& lhs, const MPair<K, V>& rhs){
        return lhs.key==rhs.key;
    }
    friend bool operator <(const MPair<K, V>& lhs, const MPair<K, V>& rhs){
        return lhs.key<rhs.key;
    }
    friend bool operator <=(const MPair<K, V>& lhs, const MPair<K, V>& rhs){
        return lhs.key>=rhs.key;
    }
    friend bool operator >(const MPair<K, V>& lhs, const MPair<K, V>& rhs){
        return lhs.key>rhs.key;
    }
    friend bool operator >=(const MPair<K, V>& lhs, const MPair<K, V>& rhs){
        return lhs.key>=rhs.key;
    }
    friend MPair<K, V> operator +(const MPair<K, V>& lhs, const MPair<K, V>& rhs){
        assert(lhs.key==rhs.key);
        // assert(lhs==rhs);
        vector<V> sum=lhs.value_list;
        while(!rhs.value_list.empty())
            sum.push_back(rhs.value_list.pop_back());
        return MPair(lhs.key,sum);
    }
};

template <typename K, typename V>
class MMap{
public:
    typedef BPlusTree<MPair<K,V>> map_base;
    class Iterator{
    public:
        friend class MMap;
        Iterator(typename map_base::Iterator it=nullptr):_it(it){}
        Iterator operator ++(int unused){
            _it++;
            return *this;
        }
        Iterator operator ++(){
            assert(_it);
            _it++;
            return *this;
        }
        MPair<K, V> operator *(){
            if(_it==nullptr)
                return MPair<K,V>();
            return *_it;
        }
        friend bool operator ==(const Iterator& lhs, const Iterator& rhs){
            return lhs._it==rhs._it;
        }
        friend bool operator !=(const Iterator& lhs, const Iterator& rhs){
            return !(lhs==rhs);
        }
    private:
        typename map_base::Iterator _it;
    };

    MMap();

//  Element Access
    const vector<V>& operator[](const K& key) const;
    vector<V>& operator[](const K& key);
    vector<V>& get(const K& key);
    vector<V>& at(const K& key);
    vector<V>& at(const K& key) const;

//  Modifiers
    void insert(const K& k, const V& v);
    void erase(const K& key);
    void clear();

//  Operations:
    Iterator find(const K& key);
    Iterator lower_bound(const K& key);  //return first that goes NOT BEFORE key entry or next if does not exist: >= entry
    Iterator upper_bound(const K& key);  //return first that goes AFTER key exist or not, the next entry  >entry

//  Iterators
    Iterator begin();
    Iterator end();

//  Capacity
    int size() const;
    bool empty() const;
    bool contains(const K& key) const;
    int count(const K& key);

    void print_lookup(){
        // cout<<"MMAP PRINT LOOKUP: INC FUNCT MM.H, L"<<__LINE__<<endl;
        cout<<"sizE: "<<size()<<endl;
        for(MMap<K,V>::Iterator it=begin(); it!=end(); it++){
            cout<<setw(10)<<(*it).key<<" :\t"<<(*it).value_list<<endl;
        }
        cout<<"=============="<<endl<<*this<<endl;
    }

    bool is_valid(){return _mmap.is_valid();}
    friend ostream& operator<<(ostream& outs, const MMap<K, V>& print_me){
        outs<<print_me._mmap<<endl;
        return outs;
    }

private:
    BPlusTree<MPair<K, V>> _mmap;
};

template <typename K, typename V>
MMap<K,V>::MMap(){
    _mmap=map_base();
}
template <typename K, typename V>
const vector<V>& MMap<K,V>::operator[](const K& key) const{
    assert(_mmap.contains(MPair<K, V>(key)));
    return (_mmap.get_existing(MPair<K, V>(key))).value;
}    
template <typename K, typename V>
vector<V>& MMap<K,V>::operator[](const K& key){
    if(!_mmap.contains(MPair<K, V>(key)))
        return (_mmap.get(MPair<K, V>(key))).value_list;
    return (_mmap.get_existing(MPair<K, V>(key))).value_list;
}    
template <typename K, typename V>
vector<V>& MMap<K,V>::at(const K& key){
    if(!_mmap.contains(MPair<K, V>(key)))
        return (_mmap.get(MPair<K, V>(key))).value_list;
    return (_mmap.get_existing(MPair<K, V>(key))).value_list;
}
template <typename K, typename V>
vector<V>& MMap<K,V>::at(const K& key) const{
    assert(_mmap.contains(MPair<K, V>(key)));
    return (_mmap.get_existing(MPair<K, V>(key))).value_list;
}
template <typename K, typename V>
vector<V>& MMap<K,V>::get(const K& key){
    if(!_mmap.contains(MPair<K, V>(key)))
        return (_mmap.get(MPair<K, V>(key))).value_list;
    return (_mmap.get_existing(MPair<K, V>(key))).value_list;
}

// ---------------------------------------------------------------- Modifiers
template <typename K, typename V>
void MMap<K,V>::insert(const K& k, const V& v){
    _mmap.insert(MPair<K, V>(k,v));
}
template <typename K, typename V>
void MMap<K,V>::erase(const K& key){
    _mmap.remove(MPair<K, V>(key));
}
template <typename K, typename V>
void MMap<K,V>::clear(){
    _mmap.clear_tree();
}

// ---------------------------------------------------------------- Operations:
template <typename K, typename V>
typename MMap<K,V>::Iterator MMap<K,V>::find(const K& key){
    return _mmap.find(MPair<K, V>(key));
}
template <typename K, typename V>
typename MMap<K,V>::Iterator MMap<K,V>::lower_bound(const K& key){
//return first that goes NOT BEFORE key entry or next if does not exist: >= entry
    return _mmap.lower_bound(MPair<K, V>(key));
}  
template <typename K, typename V>
typename MMap<K,V>::Iterator MMap<K,V>::upper_bound(const K& key){
//return first that goes AFTER key exist or not, the next entry  >entry
    return _mmap.upper_bound(MPair<K, V>(key));
}  

// ---------------------------------------------------------------- Iterators
template <typename K, typename V>
typename MMap<K,V>::Iterator MMap<K,V>::begin(){
    return _mmap.begin();
}
template <typename K, typename V>
typename MMap<K,V>::Iterator MMap<K,V>::end(){
    return _mmap.end();
}

// ---------------------------------------------------------------- Capacity
template <typename K, typename V>
int MMap<K,V>::size() const{
    return _mmap.size();
}
template <typename K, typename V>
bool MMap<K,V>::empty() const{
    return _mmap.empty();
}
template <typename K, typename V>
bool MMap<K,V>::contains(const K& key) const{
    return _mmap.contains(key);
}
template <typename K, typename V>
int MMap<K,V>::count(const K& key){
    int sum=0;
    for(MMap<K,V>::Iterator it=_mmap.begin(); it!=_mmap.end(); it++)
        if((*it).key==key)
            sum++; 
    return sum;
}

#endif // MULTIMAP_H