#ifndef FILE_RECORD_H
#define FILE_RECORD_H
#include "utilities.h"

struct FileRecord{
public:
    FileRecord(){
        _record[0][0]='\0';
        recno=-1;
        //['\0'|  | G | a | r | b | a |  g  |  e |    ]
    }
    FileRecord(char str[]){
        recno=-1;
        strncpy(_record[++recno], str, MAX);
    }
    FileRecord(string s){
        recno=-1;
        strncpy(_record[++recno], s.c_str(), MAX);
    }
    FileRecord(vector<string> vectorstr){
        recno=-1;
        for(int i=0; i<vectorstr.size(); i++){
            strncpy(_record[++recno], vectorstr[i].c_str(), MAX);
        }
    }
    
    long write(fstream& outs);              //returns the record number
    long read(fstream& ins, long recno);    //returns the number of bytes read = MAX, or zero if read passed the end of file


    friend ostream& operator<<(ostream& outs, const FileRecord& r){
        for(int i=0; i<MAX+1 && *r._record[i]!='\0'; i++)
            outs<<setw(30)<<r._record[i];
        return outs;
    }
    
// private:
    static const int MAX=100;
    char _record[MAX+1][MAX+1]; //NULL char
    int recno;
};
#endif // FILE_RECORD_H
