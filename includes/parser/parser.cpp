#include "parser.h"
int Parser::_table[MAX_ROWS][MAX_COLUMNS];

Parser::Parser(char s[]):_success(false){
    _ptree=new mmap_ss();           //init _ptree
    make_table();                   //init _table
    build_keyword_list();           //init _keywords
    set_string(s);                  //init _buffer
    // tokenize_buffer();           //init _input
    // _success=get_parse_tree(_input);
}
bool Parser::set_string(char s[]){
    strncpy(_buffer,s,MAX_BUFFER);  //init _buffer
    tokenize_buffer();              //init _input
    _ptree->clear();                //clear
    _success=get_parse_tree();
}
mmap_ss& Parser::parse_tree(){
    return *_ptree;
}
//PRIV----------------------------------------------------------
bool Parser::make_table(){
    const bool debug=false;
    init_table(_table);
    mark_fail(_table, 0);                                           //did not leave start

    //select machine - 5 rows
    mark_cell(0,_table,SELECT,SELECT_ROW);

    mark_cell(SELECT_ROW,_table,SYMBOL,SELECT_ROW+1);
    mark_cell(SELECT_ROW+1,_table,SYMBOL,SELECT_ROW+1);             //fields
    mark_cell(SELECT_ROW+1,_table,FROM,SELECT_ROW+2);           
    mark_cell(SELECT_ROW+2,_table,SYMBOL,SELECT_ROW+3);             //table name
    mark_success(_table,SELECT_ROW+3);
    mark_cell(SELECT_ROW+3,_table,WHERE,SELECT_ROW+4);
    mark_cell(SELECT_ROW+4,_table,SYMBOL,SELECT_ROW+5);
    mark_success(_table,SELECT_ROW+5);
    mark_cell(SELECT_ROW+5,_table,SYMBOL,SELECT_ROW+5);

    //create machine - 4 rows
    mark_cell(0,_table,CREATE,CREATE_MAKE_ROW);
    mark_cell(0,_table,MAKE,CREATE_MAKE_ROW);

    mark_cell(CREATE_MAKE_ROW,_table,TABLE,CREATE_MAKE_ROW+1);
    mark_cell(CREATE_MAKE_ROW+1,_table,SYMBOL,CREATE_MAKE_ROW+2);   //name of table
    mark_cell(CREATE_MAKE_ROW+2,_table,FIELDS,CREATE_MAKE_ROW+3);   
    mark_cell(CREATE_MAKE_ROW+3,_table,SYMBOL,CREATE_MAKE_ROW+4);
    mark_success(_table,CREATE_MAKE_ROW+4);   
    mark_cell(CREATE_MAKE_ROW+4,_table,SYMBOL,CREATE_MAKE_ROW+4);   

    //insert machine - 4 rows
    mark_cell(0,_table,INSERT,INSERT_ROW);

    mark_cell(INSERT_ROW,_table,INTO,INSERT_ROW+1);
    mark_cell(INSERT_ROW+1,_table,SYMBOL,INSERT_ROW+2);             //name of table
    mark_cell(INSERT_ROW+2,_table,VALUES,INSERT_ROW+3);             
    mark_cell(INSERT_ROW+3,_table,SYMBOL,INSERT_ROW+4);             //value to get inserted
    mark_success(_table,INSERT_ROW+4);   
    mark_cell(INSERT_ROW+4,_table,SYMBOL,INSERT_ROW+4);

    //drop machine - 2 rows
    mark_cell(0,_table,DROP,DROP_ROW);
    mark_cell(DROP_ROW,_table,TABLE,DROP_ROW+1);
    mark_cell(DROP_ROW+1,_table,SYMBOL,DROP_ROW+2);                 //name of table
    mark_success(_table,DROP_ROW+2);
}
bool Parser::tokenize_buffer(){
    STokenizer stk(_buffer);
    Token t;
    string s="";
    stk>>t;
    _input.push(new Token(t));
    while(stk.more()){
        t=Token();
        stk>>t;
        if(t.type()!=TOKEN_SPACE && t.type()!=TOKEN_PUNC && !t.token_str().empty())
            _input.push(new Token(t));
    }
}
bool Parser::build_keyword_list(){
    //build keywords map - represent cols of table
    _keywords.insert("zero",    ZERO);      //success state col
    _keywords.insert("select",  SELECT);    
    _keywords.insert("from",    FROM);
    _keywords.insert("where",   WHERE);
    _keywords.insert("create",  CREATE);
    _keywords.insert("make",    MAKE);
    _keywords.insert("drop",    DROP);      //del table
    _keywords.insert("insert",  INSERT);
    _keywords.insert("into",    INTO);
    _keywords.insert("values",  VALUES);
    _keywords.insert("table",   TABLE);
    _keywords.insert("fields",  FIELDS);
    // _keywords.insert("symbol",  SYMBOL);    //any unknown
}          
bool Parser::get_parse_tree(){
    //return success state
    //pop q, find col for token, find next state, store vals into ptree based on state
    int state=0;
    while(!_input.empty()){
        Token* t=_input.pop();
        state=_table[state][get_column(t->token_str())];
        switch(state){
            case DROP_ROW:
            case SELECT_ROW:
            case INSERT_ROW:
            case INSERT_ROW+1:
            case CREATE_MAKE_ROW:
                _ptree->at("command")+=t->token_str();
                break;
            case SELECT_ROW+1:
            case CREATE_MAKE_ROW+4:
                _ptree->at("fields")+=t->token_str();
                break;
            case SELECT_ROW+3:
            case INSERT_ROW+2:
            case CREATE_MAKE_ROW+2:
            case DROP_ROW+2:
                _ptree->at("table_name")+=t->token_str();
                break;
            case SELECT_ROW+4:
                _ptree->at("where")+=string("yes");
                break;
            case SELECT_ROW+5:
                _ptree->at("condition")+=t->token_str();
                break;
            case INSERT_ROW+4:
                _ptree->at("values")+=t->token_str();
                break;
            default:
                // cout<<__LINE__<<"==================================="<<endl;
                break;
        }
    }
    assert(_input.empty());
    if(_table[state][SUCCESS_COLUMN]==-1){
        cout<<"CLEAR TABLE=========="<<__LINE__<<endl;
        _ptree->clear();
        return false;
    }
    return true;
}  
int Parser::get_column(string keyword){    
    if(_keywords.contains(keyword)){
        return _keywords.get(keyword);
    }
    return SYMBOL;//?
}