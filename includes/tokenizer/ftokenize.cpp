#include "ftokenize.h"

FTokenizer::FTokenizer(char* fname){
    _f.open(fname);
    if(_f.fail()){
        cout<<"file opening fail?"<<endl;
        exit(1);
    }
    _pos=0;
    _blockPos=0;
    _more=true;
    bool retrieveFirstBlock=get_new_block();

    if(retrieveFirstBlock)
        cout<<"FTK CTOR success"<<endl;
    else 
        exit(2);
}

Token FTokenizer::next_token(){
    bool debug=true;
    Token t;
    _stk>>t;
    if(!_stk.more()){
        // cout<<"enter if to ";
        _more=get_new_block();
        // cout<<"get new block: "<<boolalpha<<hold<<endl;
        if(debug && !_more)
            cout<<"===================no more blocks to get"<<endl;    
    }
    return t;
    
    //     assert(_blockPos<MAX_BUFFER);
    //     _blockPos+=strlen(t.token_str().c_str());

    // _more=_stk.more();
    // while(_more){
    //     _stk>>t;
    //     if()
    // }
    // return t;
    
    // while(stk.more()){
    //     cout<<"WHILE TOP | token: ["<<t<<"]"<<"\ttype: "<<t.type_string()<<endl;
    //     t = Token();
    //     stk>>t;
    // }
}
    // _blockPos=_stk._pos; -> 
    // _more=(_stk.more() || get_new_block());

FTokenizer& operator >> (FTokenizer& f, Token& t){
    // t=Token();
    // f._stk>>t;
    // cout<<"not in while loop yet"<<endl;
    // while(_stk.more()){
    //     cout<<"WHILE TOP | token: ["<<t<<"]"<<"\ttype: "<<t.type_string()<<endl;
    //     cout<<"blockpos: "<<_blockPos<<endl;
    //     //update _blockPos
    // }
    t=f.next_token();
    return f;
}

bool FTokenizer::get_new_block(){
    cout<<"FTK::get_new_block() top"<<endl;
    //make sure that you are done with current block before replacing
    
    assert(_more);  
    // assert(_stk.done());    //doesnt work when getting another block
    // assert(!_stk.more());    //doesnt work when getting init block
    
    if(_f.eof()){
        cout<<"END OF FILE"<<endl;
        return false;   //no more blocks to get
    }
    //else, return true after successfully getting a block

    
    //reset _blockpos, add to blockpos
    _pos+=_blockPos;
    _blockPos=0;
    
    char temp_buffer[MAX_BUFFER];
    _f.read(temp_buffer,MAX_BUFFER-1);
    temp_buffer[_f.gcount()]='\0';
    // temp_buffer[MAX_BUFFER]='\0';
    
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