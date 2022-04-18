#include "state_machine_functions.h"

//Fill all cells of the array with -1
void init_table(int _table[][MAX_COLUMNS]){
    for(int r=0; r<MAX_ROWS; r++){
        for(int c=0; c<MAX_COLUMNS; c++){
            _table[r][c]=TOKEN_UNKNOWN;
        }
    }
}

//Mark this state (row) with a 1 (success)
void mark_success(int _table[][MAX_COLUMNS], int state){
    _table[state][SUCCESS_COLUMN]=STATE_SUCCESS;
}

//Mark this state (row) with a 0 (fail)
void mark_fail(int _table[][MAX_COLUMNS], int state){
    _table[state][SUCCESS_COLUMN]=STATE_FAIL;
}

//true if state is a success state
bool is_success(int _table[][MAX_COLUMNS], int state){
    return _table[state][SUCCESS_COLUMN]==STATE_SUCCESS;
}

//Mark a range of cells in the array. 
void mark_cells(int row, int _table[][MAX_COLUMNS], int from, int to, int state){
    assert(row<MAX_ROWS && from<=to);
    for(;from<=to;from++){
        _table[row][from]=state;
    }
}

//Mark columns represented by the string columns[] for this row
void mark_cells(int row, int _table[][MAX_COLUMNS], const char cols[], int state){
    for(int i=0; cols[i]!='\0'; i++){
        assert(cols[i]<MAX_COLUMNS);
        // cout<<"changing col "<<(cols[i]-'0')<<endl;
        _table[row][cols[i]]=state;
    }
}

//Mark this row and column
void mark_cell(int row, int _table[][MAX_COLUMNS], int col, int state){
    assert(row<MAX_ROWS && col<MAX_COLUMNS);
    _table[row][col]=state;
}

//This can realistically be used on a small table
void print_table(int _table[][MAX_COLUMNS]){
    /*    cout<<"DIGITS TABLE\n\t";
    for(int i=0; DIGITS[i]!='\0'; i++){
        cout<<"[  '"<< DIGITS[i]<<"'\t]";
    }
    cout<<"[  '.'\t][OTHER\t]";

    for(int r=0; r<10; r++){
        cout<<"\nSTATE "<<r<<"\t";
        for(int c=0; c<12; c++){
            cout<<"[ "<<_table[r][c]<<"\t]";
        }
    }
    */
    /*  cout<<"dropbox ab sequence table"
    for(int r=0; r<10; r++){
        cout<<"\nSTATE "<<r<<"\t";
        for(int c=0; c<12; c++){
            cout<<"[ "<<_table[r][c+'a']<<"\t]";
        }
    }
    */
    for(int c=20; c<100; c++){
        cout<<"[  '"<<char(c)<<"'\t]";
    }
    for(int r=0; r<21; r++){
        cout<<"\nSTATE "<<r<<"\t";
        for(int c=20; c<100; c++){
            cout<<"[ "<<_table[r][c]<<"\t]";
        }
    }
    cout<<endl;
}

//show string s and mark this position on the string:
//hello world   pos: 7
//       ^
void show_string(char s[], int _pos){
    for(int i=0; s[i]!='\0'; i++){
        cout<<s[i];
    }
    cout<<"\tpos: "<<_pos<<endl;
    for(int i=0; i<_pos; i++){
        cout<<" ";
    }
    cout<<"^"<<endl;
}