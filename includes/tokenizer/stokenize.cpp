#include "stokenize.h"
int STokenizer::_table[MAX_ROWS][MAX_COLUMNS];

STokenizer::STokenizer(){
    strncpy(_buffer,"DEFAULT",MAX_BUFFER); 
    _pos=0;
    make_table(_table);
}
STokenizer::STokenizer(char str[]){
    strncpy(_buffer,str,MAX_BUFFER); 
    _pos=0;
    make_table(_table);
}
bool STokenizer::done(){
    return _buffer[_pos]=='\0';
}            
bool STokenizer::more(){
    return _pos<=strlen(_buffer);
}

//extract one token (very similar to the way cin >> works)
STokenizer& operator >> (STokenizer& s, Token& t){
    int type=TOKEN_UNKNOWN;
    int state=0;
    string token="";
    if(s.get_token(state, token)){
        // cout<<"state: ["<<state<<"]\n";
        //row associated with success state of given token type
        switch(state){
            case 1:                 //digits success
            case 3:                 type=TOKEN_NUMBER;      break;
            case START_PUNC:        type=TOKEN_PUNC;        break;
            case START_ALPHA:       type=TOKEN_ALPHA;       break;
            case START_SPACES:      type=TOKEN_SPACE;       break;
            case START_OPERATOR:    type=TOKEN_OPERATOR;    break;
            case START_QUOTE:       type=TOKEN_QUOTE;       break;
            case START_QUOTE+1:       type=TOKEN_QUOTE;       break;
            default:                type=TOKEN_UNKNOWN;     break;
        }
    }
    else{
        token=s._buffer[s._pos-1];
        // cout<<"get_token return false"<<endl;
    }
    t=Token(token,type);
    return s;
}

//set a new string as the input string
void STokenizer::set_string(char str[]){
    strcpy(_buffer, str);
    _pos=0;
}

//create table for all the tokens we will recognize (e.g. doubles, words...)
void STokenizer::make_table(int _table[][MAX_COLUMNS]){
    bool debug=false;
    init_table(_table);
    mark_fail(_table, 0);               //did not leave start

    // digits
    mark_success(_table,1); //no decimal point in digit
    mark_fail(_table, 2);   //decimal point processed (may or may not have leading digits), but no following 
    mark_success(_table,3); //numbers following decimal point 
    
    mark_cells(START_DOUBLE,_table,DIGITS,START_DOUBLE);
    mark_cells(0,_table,DIGITS,START_DOUBLE);
    
    mark_cells(0, _table, DIGITS, 1);   //state [0] --- DIGITS ---> [1]
    mark_cells(1, _table, DIGITS, 1);   //state [1] --- DIGITS ---> [1]
    mark_cell(0, _table, '.', 2);       //state [0] --- '.' ------> [2] 
    mark_cell(1, _table, '.', 2);       //state [1] --- '.' ------> [2] 
    mark_cells(2, _table, DIGITS, 3);   //state [2] --- DIGITS ---> [3]
    mark_cells(3, _table, DIGITS, 3);   //state [3] --- DIGITS ---> [3]
    
    // alpha
    mark_success(_table,START_ALPHA);
    mark_cells(0,_table,ALFA,START_ALPHA);
    mark_cells(START_ALPHA,_table,ALFA,START_ALPHA);
    mark_cells(START_ALPHA,_table,DIGITS,START_ALPHA);

    // spaces
    mark_success(_table,START_SPACES);
    mark_cells(0,_table,SPACES,START_SPACES);
    mark_cells(START_SPACES,_table,SPACES,START_SPACES);
    // operator
    mark_success(_table,START_OPERATOR);
    mark_cells(0,_table,OPERATORS,START_OPERATOR);
    mark_cells(START_OPERATOR,_table,OPERATORS,START_OPERATOR);
    // punct
    mark_success(_table,START_PUNC);
    mark_cells(0,_table,PUNC,START_PUNC);
    mark_cells(START_PUNC,_table,PUNC,START_PUNC);
    // quote
    mark_cells(0,_table,QUOTE,START_QUOTE);
    mark_fail(_table,START_QUOTE);
    mark_cells(START_QUOTE,_table,ALFA,START_QUOTE);
    mark_cells(START_QUOTE,_table,DIGITS,START_QUOTE);
    mark_cells(START_QUOTE,_table,SPACES,START_QUOTE);
    mark_cells(START_QUOTE,_table,OPERATORS,START_QUOTE);
    mark_cells(START_QUOTE,_table,PUNC,START_QUOTE);
    mark_success(_table,START_QUOTE+1);
    mark_cells(START_QUOTE,_table,QUOTE,START_QUOTE+1);

    if(debug)
        print_table(_table);
}

//extract the longest string that match one of the acceptable token types
bool STokenizer::get_token(int& start_state, string& token){
    bool debug=false;

    if(_buffer[_pos]<0 || _table[start_state][_buffer[_pos]]==TOKEN_UNKNOWN){    //foreign char
        start_state=TOKEN_UNKNOWN;
        token=_buffer[_pos];
        _pos++;
        return false;
    }

    int start_pos=_pos;
    int last_success_pos=_pos;
    int current_state=_table[start_state][_buffer[_pos]];

    while(_buffer[_pos]!='\0' && current_state!=-1){    //checks if unknown
        if(debug){cout<<"GET TOKEN while top | current state/row ["<<current_state<<"], buffer temp:["<<_buffer[_pos]<<"] is ==> "<<_table[current_state][_buffer[_pos]]<<endl;}
        if(_table[current_state][SUCCESS_COLUMN]==STATE_SUCCESS){
            last_success_pos=_pos;
            start_state=current_state;
            if(debug) {cout<<"success/token:    ["<<token<<"]\tcurrent state:    "<<start_state<<endl;}
        }
        current_state=_table[current_state][_buffer[++_pos]];//get next state
    }
    if(debug){
        cout<<"exit while: _pos             ["<<_pos<<"]"<<endl;
        cout<<"exit while: last_success_pos ["<<last_success_pos<<"]"<<endl;
        if(current_state==start_state){
            cout<<"============entire token valid: state ["<<current_state<<"]"<<endl;
        }
        else{
            cout<<"============entire token NOT valid=================="<<endl;
            cout<<"start state   ["<<start_state<<"]"<<endl;
            cout<<"current state ["<<current_state<<"]"<<endl;
            cout<<"===================================================="<<endl;
        }
    }
    
    _pos=last_success_pos+1;
    for(;start_pos<_pos; start_pos++){
        if(_buffer[start_pos]!='\"')
            token+=_buffer[start_pos];
    }
    return token!="";
}
    /*
    int success = _pos;
    token = "";

    for (int i = _pos; i < strlen(_buffer); i++) {
        if (_buffer[i] < 0) {
            start_state = _table[start_state][MAX_COLUMNS + _buffer[i]];
        }
        else {
            start_state = _table[start_state][_buffer[i]];
        }
        // start_state = _table[start_state][_buffer[i]];
        if (is_success(_table, start_state)) {
            success = i;
        }
        
        if (start_state == -1 || _buffer[i] == '\0') {
            break;
        }
    }

    for (int i = _pos ; i <= success; i++) {
        if (_buffer[i] != '\"') {
            token += _buffer[i];
        }
    }

    _pos = success + 1;

    if (token.length() == 0) {
        return false;
    }
    return true;


    //start_state holds the state of the valid token
    //current_state holds the state of the char at pos --> ahead; should end on fail
    // assert(token!="");
    // assert(_table[current_state][SUCCESS_COLUMN]!=STATE_SUCCESS || _buffer[_pos]=='\0');
    // return _table[start_state][SUCCESS_COLUMN]==STATE_SUCCESS;
    */