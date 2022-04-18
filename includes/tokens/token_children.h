#ifndef TOKEN_CHILDREN_H
#define TOKEN_CHILDREN_H
#include "token.h"
#include "../table/typedefs.h"

//need rpn & sy
class ResultSet:public Token{
public:
    ResultSet();
    ResultSet(vector<long> recnos):Token("",RESULTSET), _recnos(recnos){
        sort(_recnos.begin(),_recnos.end());
    }
    vector<long> recnos(){
        return _recnos;
    }
private:
    vector<long> _recnos;
};

class TokenStr:public Token{
public:
    TokenStr(string tok):Token(tok,STRING){}
private:
};

class Operator:public Token{
public:
    Operator(string tok):Token(tok,OPERATOR), _op(tok){
        if(_op=="&" || _op=="&&" || _op=="and")
            _precedence=2;
        else if(_op=="||" || _op=="or")
            _precedence=3;
        else 
            _precedence=1;
    }
    virtual int Type() const{
        return OPERATOR;
    }
    virtual int Precedence() const{
        return _precedence;
    }
    virtual ResultSet* evaluate(vector<MMap<string,long>*>& indices, Token* l, Token* r){}
    // virtual vector<long> evaluate(MMap<string,long>* mmap, Token* l, Token* r){}
    // virtual ResultSet* evaluate(MMap<string,long> indices, Token* l, Token* r);
    // vector<long> evaluate(MMap<string,long>* mmap, Token* l, Token* r){
        // cout<<"op: "<<_op<<endl;
        // string value=l->token_str();    //left=value
        // vector<long> recnos;
        // if(_op=="=="||_op=="="){
        //     recnos=mmap->at(value);
        // }
        // else if(_op==">"){
        //     MMap<string,long>::Iterator it_beg=mmap->upper_bound(value);
        //     MMap<string,long>::Iterator it_end=mmap->end();
        //     for(;it_beg!=it_end; it_beg++){
        //         for(int i=0; i<(*it_beg).value_list.size(); i++)
        //             recnos.push_back((*it_beg).value_list[i]);
        //     }
        // }
        // else if(_op==">="){
        //     MMap<string,long>::Iterator it_beg=mmap->find(value);
        //     MMap<string,long>::Iterator it_end=mmap->end();
        //     for(;it_beg!=it_end; it_beg++){
        //         for(int i=0; i<(*it_beg).value_list.size(); i++)
        //             recnos.push_back((*it_beg).value_list[i]);
        //     }
        // }
        // else if(_op=="<"){
        //     MMap<string,long>::Iterator it_beg=mmap->begin();
        //     MMap<string,long>::Iterator it_end=mmap->find(value);
        //     for(;it_beg!=it_end; it_beg++){
        //         for(int i=0; i<(*it_beg).value_list.size(); i++)
        //             recnos.push_back((*it_beg).value_list[i]);
        //     }
        // }
        // else if(_op=="<="){
        //     MMap<string,long>::Iterator it_beg=mmap->begin();
        //     MMap<string,long>::Iterator it_end=mmap->upper_bound(value);
        //     for(;it_beg!=it_end; it_beg++){
        //         for(int i=0; i<(*it_beg).value_list.size(); i++)
        //             recnos.push_back((*it_beg).value_list[i]);
        //     }
        // }
        // return recnos;
    // }
private:
    string _op;
    int _precedence;
};

class Logical:public Operator{
public:
    Logical(string tok):Operator(tok), _op(tok){}
    virtual int Type() const{
        return LOGICAL;
    }
    vector<long> evaluate(Token* l, Token* r){
        vector<long> left=static_cast<ResultSet*>(l)->recnos();
        vector<long> right=static_cast<ResultSet*>(r)->recnos();
        vector<long> result(left.size()+right.size());
        if(_op=="and"){
            vector<long>::iterator it=set_intersection(left.begin(), left.end(), right.begin(), right.end(), result.begin());
            result.erase(it,result.end());
        }
        else if(_op=="or"){
            vector<long>::iterator it=set_union(left.begin(), left.end(), right.begin(), right.end(), result.begin());
            result.erase(it,result.end());
        }
        return result;
    }
    virtual ResultSet* evaluate(vector<MMap<string,long>*>& indices, Token* l, Token* r){
        return new ResultSet(evaluate(l,r));
    }
private:
    // virtual ResultSet* evaluate(MMap<string,long> indices, Token* l, Token* r);
    string _op;
};

class Relational:public Operator{
public:
    Relational(string tok):Operator(tok), _op(tok){}
    virtual int Type() const{
        return RELATIONAL;
    }
    virtual vector<long> evaluate(MMap<string,long>* mmap, Token* key){
        string value=key->token_str();    //left=value
        vector<long> recnos;
        if(_op=="=="||_op=="="){
            recnos=mmap->at(value);
        }
        else if(_op==">"){
            MMap<string,long>::Iterator it_beg=mmap->upper_bound(value);
            MMap<string,long>::Iterator it_end=mmap->end();
            for(;it_beg!=it_end; it_beg++){
                for(int i=0; i<(*it_beg).value_list.size(); i++)
                    recnos.push_back((*it_beg).value_list[i]);
            }
        }
        else if(_op==">="){
            MMap<string,long>::Iterator it_beg=mmap->lower_bound(value);
            MMap<string,long>::Iterator it_end=mmap->end();
            for(;it_beg!=it_end; it_beg++){
                for(int i=0; i<(*it_beg).value_list.size(); i++)
                    recnos.push_back((*it_beg).value_list[i]);
            }
        }
        else if(_op=="<"){
            MMap<string,long>::Iterator it_beg=mmap->begin();
            MMap<string,long>::Iterator it_end=mmap->lower_bound(value);
            for(;it_beg!=it_end; it_beg++){
                for(int i=0; i<(*it_beg).value_list.size(); i++)
                    recnos.push_back((*it_beg).value_list[i]);
            }
        }
        else if(_op=="<="){
            MMap<string,long>::Iterator it_beg=mmap->begin();
            MMap<string,long>::Iterator it_end=mmap->upper_bound(value);
            for(;it_beg!=it_end; it_beg++){
                for(int i=0; i<(*it_beg).value_list.size(); i++)
                    recnos.push_back((*it_beg).value_list[i]);
            }
        }
        return recnos;
    }
    // virtual ResultSet* evaluate(vector<MMap<string,long>*>& indices, Token* l, Token* r){
    //     int k=_fields.get(l->token_str());//??????
    //     return new ResultSet(evaluate(indices[k],r));
    // }
private:
    // virtual ResultSet* evaluate(MMap<string,long> indices, Token* l, Token* r);
    string _op;
};

class LParen:public Token{
public:
    LParen():Token("(",LPAREN){}
private:
};
class RParen:public Token{
public:
    RParen():Token(")",RPAREN){}
private:
};

#endif // TOKEN_CHILDREN_H
