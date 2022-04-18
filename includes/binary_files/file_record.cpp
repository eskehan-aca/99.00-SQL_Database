#include "file_record.h"
long FileRecord::write(fstream &outs){
    //r.write(f); //take the Record r and write it into file f and return the record number of this Record
    //write to the end of the file.
    outs.seekg(0, outs.end);
    long pos=outs.tellp();    //retrieves the current position of the file pointer
    //pay attention to this:
    outs.write(&_record[0][0], sizeof(_record));
    return pos/sizeof(_record);  //record number
}

long FileRecord::read(fstream &ins, long recno){
    //returns the number of bytes read.
    //    r.read(f, 6);
    //    cout<<r<<endl;
    long pos= recno * sizeof(_record);
    ins.seekg(pos, ios_base::beg);

    //ins.read(&_record[0], sizeof(_record))
    ins.read(&_record[0][0], sizeof(_record));
    //don't you want to mark the end of  the cstring with null?
    //_record[] => [zero'\0'trash trash trash trash]
    //don't need the null character, but for those of us with OCD and PTSD:
    // _record[ins.gcount()] = '\0';
    return ins.gcount();

}