#include "ftokenize.h"

const bool ftkDebug=false;
FTokenizer::FTokenizer(char* fname){
    _f.open(fname);
    if(_f.fail()){
        throw("file failed to open.");
        exit(1);
    }
    _pos=0;
    _blockPos=0;
    _more=true;
    bool retrieveFirstBlock=get_new_block();

    if(!retrieveFirstBlock)
        assert(false);
}

Token FTokenizer::next_token(){
    Token t;
    _stk>>t;
    if(!_stk.more()){
        _more=get_new_block();
        if(ftkDebug && !_more)
            cout<<"===================no more blocks to get"<<endl;    
    }
    return t;
}

FTokenizer& operator >> (FTokenizer& f, Token& t){
    t=f.next_token();
    return f;
}

bool FTokenizer::get_new_block(){
    if(ftkDebug)
    cout<<"FTK::get_new_block() top"<<endl;
    //make sure that you are done with current block before replacing
    
    assert(_more);  
    if(_f.eof()){
        if(ftkDebug)
            cout<<"END OF FILE"<<endl;
        return false;   //no more blocks to get
    }
    
    //reset _blockpos, add to blockpos
    _pos+=_blockPos;
    _blockPos=0;
    
    char temp_buffer[MAX_BUFFER];
    _f.read(temp_buffer,MAX_BUFFER-1);
    temp_buffer[_f.gcount()]='\0';
    
    int blockLen=_f.gcount(); //the last block may end early => junk after
    _stk.set_string(temp_buffer);
    return true;
}

bool FTokenizer::more(){
    //done when:
    // - end of file
    // - get_new_block false => no more blocks
    //   - finished reading current block
    if(_more==false)
        return _more;
    return _stk.more() || !_f.eof();
}
int FTokenizer::pos(){
    return _pos;
}
int FTokenizer::block_pos(){
    return _blockPos;
}