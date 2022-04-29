#ifndef CONSTANTS_H
#define CONSTANTS_H

#define SUCCESS_COLUMN          0
#define STATE_SUCCESS           1
#define STATE_FAIL              0

#include <algorithm>
#include <assert.h>
#include <string.h>
#include <iostream>
#include <iomanip>
#include <stdio.h>
#include <fstream>

using namespace std;

const bool syDebug=false;
const bool tblDebug=false;

//FOR TOKENIZER
const int MAX_COLUMNS = 256;
const int MAX_ROWS = 100;
const int MAX_BUFFER = 200;

const char ALFA[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
const char QUOTE[] = "\"";
const char DIGITS[] = "0123456789";
const char OPERATORS[] = "><=!+-%&|";
const char SPACES[] = {' ', '\t', '\n', '\0'};
const char PUNC[] = "?.,:;'`~!";
const int START_DOUBLE = 0; //valid doubles = 
const int START_SPACES = 4;
const int START_ALPHA = 6;
// 's
const int START_PUNC = 10;
const int START_OPERATOR = 20;
const int START_QUOTE = 30;

//FOR PARSER
//adjacency matrix
enum PARSER_KEYS{
    ZERO,   //first is suc. state
    SELECT,
    FROM,
    WHERE,
    CREATE,
    MAKE,
    DROP,
    INSERT,
    INTO,
    VALUES,
    TABLE,
    FIELDS,
    SYMBOL,
};
//rows > where machine starts?
enum PARSER_MACHINES{
    SELECT_ROW=10,
    CREATE_MAKE_ROW=20,
    INSERT_ROW=30,
    DROP_ROW=40,
};

//token types:
const int TOKEN_NUMBER = 1;
const int TOKEN_ALPHA = 2;
const int TOKEN_SPACE = 3;
const int TOKEN_OPERATOR = 4;
const int TOKEN_PUNC = 5;
const int TOKEN_QUOTE = 6;

const int TOKEN_UNKNOWN = -1;
const int TOKEN_END = -2;

//FOR RPN/SY
enum TOKEN_TYPES{
    LPAREN,     //shunting
    RPAREN,     //shunting
    STRING,     
    OPERATOR,   
    LOGICAL,    
    RELATIONAL, 
    RESULTSET   
};

#endif // CONSTANTS_H
