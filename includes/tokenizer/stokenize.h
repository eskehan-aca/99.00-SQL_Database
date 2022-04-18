#ifndef STOKENIZE_H
#define STOKENIZE_H
#include "state_machine_functions.h"
#include "constants.h"
#include "../tokens/token.h"

class STokenizer{
public:
    STokenizer();
    STokenizer(char str[]);
    
    bool done();            //true: there are no more tokens
    bool more();            //true: there are more tokens
    
    //CANVAS:
    //extract one token (very similar to the way cin >> works)
    friend STokenizer& operator >> (STokenizer& s, Token& t);

    //set a new string as the input string
    void set_string(char str[]);

private:
    //VAR
    char _buffer[MAX_BUFFER];       //input string
    int _pos;                       //current position in the string
    static int _table[MAX_ROWS][MAX_COLUMNS];
    
    
    //FUNCT---------------------------------
    
    //create table for all the tokens we will recognize (e.g. doubles, words...)
    void make_table(int _table[][MAX_COLUMNS]);

    //extract the longest string that match one of the acceptable token types
    bool get_token(int& start_state, string& token);
};

#endif // STOKENIZE_H
