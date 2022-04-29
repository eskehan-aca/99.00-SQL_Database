#ifndef TOKEN_H
#define TOKEN_H
#include "../tokenizer/constants.h"

class Token{
public:
    Token();
    Token(string str, int type);
    friend ostream& operator <<(ostream& outs, const Token& t);
    int type() const; 
    string type_string() const;
    string token_str() const;


    virtual int Type() const;
    virtual int Precedence() const;
private:
    string _token;
    int _type;
};

#endif // TOKEN_H
