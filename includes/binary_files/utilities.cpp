#include "utilities.h"

bool file_exists(const char filename[]){
    const bool debug = false;
    fstream ff;
    ff.open(filename, fstream::in|fstream::binary);
    if (ff.fail()){
        if (debug) cout<<"file_exists(): File does NOT exist: "<<filename<<endl;
        return false;
    }
    if (debug) cout<<"file_exists(): File DOES exist: "<<filename<<endl;
    ff.close();
    return true;
}

void open_fileRW(fstream& f, const char filename[]) throw(char*){
    const bool debug = false;
    //if file DNE, create by openning for output:
    if(!file_exists(filename)){
        f.open(filename,fstream::out|fstream::binary); //create the file
        if(f.fail()){
            throw("file RW failed");
        }
    }
    else{
        f.open(filename,fstream::in|fstream::out|fstream::binary);
        if(f.fail()){
            throw("file failed to open.");
        }
    }
}

void open_fileW(fstream& f, const char filename[]){
    f.open(filename,fstream::out|fstream::binary);
    if(f.fail()){
        throw("file failed to open.");
    }
}