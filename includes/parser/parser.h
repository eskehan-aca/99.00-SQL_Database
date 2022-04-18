#ifndef PARSER_H
#define PARSER_H
#include "../tokenizer/stokenize.h"
#include "../tokenizer/state_machine_functions.h"
#include "../table/typedefs.h"

class Parser{
public:
    Parser(char s[]);
    bool set_string(char s[]);
 
    mmap_ss& parse_tree();
    bool fail() {return !_success;}

private:
    //functs
    bool make_table();
    bool tokenize_buffer();
    bool build_keyword_list();          //build keywords map
    bool get_parse_tree();              //return success state
    int get_column(string keyword);     //map token to _keywords

    //vars
    static int _table[MAX_ROWS][MAX_COLUMNS];
    char _buffer[MAX_BUFFER];       //input string
    bool _success;
    mmap_ss* _ptree;                //parse tree
    map_sl _keywords;               //keywords to col #s > tells which row of table
    tokenq _input;

};

#endif // PARSER_H
