#ifndef MAP_H
#define MAP_H
#include "bplustree.h"

template <typename K, typename V>
struct Pair{
    K key;
    V value;

    Pair(const K& k=K(), const V& v=V()):key(k), value(v){}
    friend ostream& operator <<(ostream& outs, const Pair<K, V>& print_me){
        outs<<"["<<print_me.key<<": "<<print_me.value<<"]";
        return outs;
    }
    friend bool operator ==(const Pair<K, V>& lhs, const Pair<K, V>& rhs){
        return lhs.key==rhs.key;
    }
    friend bool operator <(const Pair<K, V>& lhs, const Pair<K, V>& rhs){
        return lhs.key<rhs.key;
    }
    friend bool operator >(const Pair<K, V>& lhs, const Pair<K, V>& rhs){
        return lhs.key>rhs.key;
    }
    friend bool operator <=(const Pair<K, V>& lhs, const Pair<K, V>& rhs){
        return lhs.key<=rhs.key;
    }
    friend bool operator >=(const Pair<K, V>& lhs, const Pair<K, V>& rhs){
        return lhs.key>=rhs.key;
    }
    friend Pair<K, V> operator +(const Pair<K, V>& lhs, const Pair<K, V>& rhs){
        assert(lhs.key==rhs.key);
        // assert(lhs==rhs);
        return Pair(lhs.key,lhs.value+rhs.value);
    }
};

template <typename K, typename V>
class Map{
public:
    typedef BPlusTree<Pair<K,V>> map_base;
    class Iterator{
    public:
        friend class Map;
        Iterator(typename map_base::Iterator it=nullptr):_it(it){}
        Iterator operator ++(int unused){
            assert(_it!=nullptr);
            _it++;
            return *this;
        }
        Iterator operator ++(){
            assert(_it);
            _it++;
            return *this;
        }
        Pair<K, V> operator *(){
            if(_it==nullptr)
                return Pair<K,V>();
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

    Map();

//  Element Access
    V& operator[](const K& key) const;    //assert if key not found
    V& operator[](const K& key);    //calls get, update+insertion if not found
                                    //return reference to value within the pair inside btree
    V& at(const K& key);
    const V& at(const K& key) const;

//  Modifiers
    void insert(const K& k, const V& v);
    void erase(const K& key);
    void clear();
    V get(const K& key);

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
    int count(const K& key);

    bool contains(const Pair<K, V>& target) const;

    bool is_valid(){return _map.is_valid();}
    friend ostream& operator<<(ostream& outs, const Map<K, V>& print_me){
        outs<<print_me._map<<endl;
        return outs;
    }
private:
    int _key_count;
    BPlusTree<Pair<K,V>> _map;  //T=Pair<K,V>
};

//-----------------------------------------------------------------------------

template <typename K, typename V>
Map<K,V>::Map(){
    _map=map_base();
    _key_count=0;
}

// ------------------------------------------------------------ Element Access
template <typename K, typename V>
V& Map<K,V>::operator[](const K& key) const{
    assert(_map.contains(Pair<K, V>(key)));
    return (_map.get_existing(Pair<K, V>(key))).value;
}    
template <typename K, typename V>
V& Map<K,V>::operator[](const K& key){
    if(!_map.contains(Pair<K, V>(key)))
        return (_map.get(Pair<K, V>(key))).value;
    return (_map.get_existing(Pair<K, V>(key))).value;
}    
template <typename K, typename V>
V& Map<K,V>::at(const K& key){
    if(!_map.contains(Pair<K, V>(key)))
        return (_map.get(Pair<K, V>(key))).value;
    return (_map.get_existing(Pair<K, V>(key))).value;
}
template <typename K, typename V>
const V& Map<K,V>::at(const K& key) const{
    assert(_map.contains(Pair<K, V>(key)));
    return (_map.get_existing(Pair<K, V>(key))).value;
}

// ---------------------------------------------------------------- Modifiers
template <typename K, typename V>
void Map<K,V>::insert(const K& k, const V& v){
    _map.insert(Pair<K, V>(k,v));
}
template <typename K, typename V>
void Map<K,V>::erase(const K& key){
    _map.remove(Pair<K, V>(key));
}
template <typename K, typename V>
void Map<K,V>::clear(){
    _map.clear_tree();
}
template <typename K, typename V>
V Map<K,V>::get(const K& key){
    return (_map.get(Pair<K, V>(key))).value;
}

// ---------------------------------------------------------------- Operations:
template <typename K, typename V>
typename Map<K,V>::Iterator Map<K,V>::find(const K& key){
    return _map.find(Pair<K, V>(key));
}
template <typename K, typename V>
typename Map<K,V>::Iterator Map<K,V>::lower_bound(const K& key){
//return first that goes NOT BEFORE key entry or next if does not exist: >= entry
    return _map.lower_bound(Pair<K, V>(key));
}  
template <typename K, typename V>
typename Map<K,V>::Iterator Map<K,V>::upper_bound(const K& key){
//return first that goes AFTER key exist or not, the next entry  >entry
    return _map.upper_bound(Pair<K, V>(key));
}  

// ---------------------------------------------------------------- Iterators
template <typename K, typename V>
typename Map<K,V>::Iterator Map<K,V>::begin(){
    return _map.begin();
}
template <typename K, typename V>
typename Map<K,V>::Iterator Map<K,V>::end(){
    return _map.end();
}

// ---------------------------------------------------------------- Capacity
template <typename K, typename V>
int Map<K,V>::size() const{
    return _map.size();
}
template <typename K, typename V>
bool Map<K,V>::empty() const{
    return _map.empty();
}
template <typename K, typename V>
bool Map<K,V>::contains(const Pair<K, V>& target) const{
    return _map.contains(target);
}
template <typename K, typename V>
int Map<K,V>::count(const K& key){
    int sum=0;
    for(Map<K,V>::Iterator it=_map.begin(); it!=_map.end(); it++)
        if((*it).key==key)
            sum++; 
    return sum;
}

#endif // MAP_H
