#ifndef VECTOR_UTILITIES_H
#define VECTOR_UTILITIES_H

//-------------- Vector Extra operators: ------------------------

template <typename T>
ostream& operator <<(ostream& outs, const vector<T>& list); //print vector list
template <typename T>
vector<T>& operator +=(vector<T>& list, const T& addme); //list.push_back addme

//---------------------------------------------------------------

template <typename T>//print vector list
ostream& operator <<(ostream& outs, const vector<T>& list){
    outs<<"("<<list.size()<<")\t[";
    for(int i=0; i<list.size(); i++)
        outs<<"\t"<<list.at(i);
    outs<<"\t]\n";
    return outs;
}
template <typename T>//list.push_back addme
vector<T>& operator +=(vector<T>& list, const T& addme){
    list.push_back(addme);
    return list;
}

#endif // VECTOR_UTILITIES_H
