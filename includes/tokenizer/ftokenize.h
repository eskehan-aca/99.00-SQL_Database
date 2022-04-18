#ifndef FTOKENIZE_H
#define FTOKENIZE_H
#include "stokenize.h"
#include "constants.h"

class FTokenizer{
public:
    FTokenizer(char* fname);

    Token next_token();
    bool more();        //returns the current value of _more
    int pos();          //returns the value of _pos
    int block_pos();     //returns the value of _blockPos

    friend FTokenizer& operator >> (FTokenizer& f, Token& t);

    const int MAX_BLOCK = MAX_BUFFER;

private:
    //VARS
    std::ifstream _f;       //file being tokenized
    STokenizer _stk;        //The STokenizer object to tokenize current block
    int _pos;               //Current position in the file
    int _blockPos;          //Current position in the current block ==> stks._pos
    bool _more;             //false if last token of the last block has been processed and now we are at the end of the last block.
    
    //FUNCT
    bool get_new_block();   //gets the new block from the file
};

#endif // FTOKENIZE_H
