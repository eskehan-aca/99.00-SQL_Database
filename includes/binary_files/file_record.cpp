#include "file_record.h"
long FileRecord::write(fstream &outs){
    //write Record r to end of file, return the recno
    outs.seekg(0, outs.end);
    long pos=outs.tellp();    //current pos of the file pointer
    outs.write(&_record[0][0], sizeof(_record));
    return pos/sizeof(_record);  //record number
}

long FileRecord::read(fstream &ins, long recno){
    //returns the number of bytes read.
    long pos= recno * sizeof(_record);
    ins.seekg(pos, ios_base::beg);
    ins.read(&_record[0][0], sizeof(_record));
    // _record[ins.gcount()] = '\0';
    return ins.gcount();

}