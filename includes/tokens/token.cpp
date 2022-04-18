#include "token.h"

Token::Token(){
    _token="";
    _type=TOKEN_UNKNOWN;
}
Token::Token(string str, int type){
    _token=str;
    _type=type;
    // cout<<"create token: ["<<_type<<"]\t["<<_token<<"]"<<endl;
}

ostream& operator <<(ostream& outs, const Token& t){
    outs<<"["<<t._token<<"]\ttype: "<<t._type;
    return outs;
}
int Token::type() const{
    return _type;
}
int Token::Type() const{
    return _type;
    //virtual??
}
int Token::Precedence() const{
    //virtual??
}
string Token::type_string() const{
    //use _type, compare to token types in const file
    switch(_type){
        // cout<<"switch top"<<endl;
        case TOKEN_NUMBER:  return "NUMBER";    break;
        case TOKEN_ALPHA:   return "ALPHA";     break;
        case TOKEN_SPACE:   return "SPACE";     break;
        case TOKEN_PUNC:    return "PUNCT";     break;
        case TOKEN_END:     return "END";       break;
        case TOKEN_OPERATOR:return "OPERATOR";  break;
        case TOKEN_UNKNOWN: return "UNKNOWN";   break;
        defaut: 
            // cout<<"ERR Token::type_string()invalid _type: "+to_string(_type)+"]"<<endl;
            return "TOKEN:TYPE_STRING() SWITCH ERROR: (_type "+to_string(_type)+")";
    }
    return "TOKEN:TYPE_STRING() ERROR";
}  
string Token::token_str() const{
    return _token;   
}