#ifndef TABLE_H
#define TABLE_H
#include "typedefs.h"

class Table{
public:
    Table();
    Table(const string& name);
    Table(const string& name, const vectorstr& fields);
    ~Table();

    void insert_into(const vectorstr& record);
    Table select(const vectorstr& fields, const vectorstr& infix);
    Table select(const vectorstr& fields, const string& field, const string& op, const string& value); //for testing only
    Table select(const vectorstr& fields, Queue<Token*>& conditions);

    Table select_all();
    Table select_all(const vectorstr& fields);

    vectorlong& select_recnos();
    vectorstr& get_fields();
    void set_fields(const vectorstr& fields);

    bool set_recnos(const vectorlong& recnos);
    bool empty() const;

    friend ostream& operator<<(ostream& outs, const Table& t){
        FileRecord rec;
        fstream f;
        string fname=t._table_name+".tbl";
        open_fileRW(f,fname.c_str());
        outs<<"table name: "<<t._table_name<<", records: "<<t._last_recno+1<<endl;
        outs<<"record";
        for(int i=0; i<t._field_names.size(); i++){
            outs<<setw(30)<<t._field_names[i];
        }
        outs<<endl;
        for(int i=0; i<=t._last_recno; i++){
            rec.read(f,i);
            outs<<setw(6)<<i<<rec<<endl;
        }
        outs<<endl;
        return outs;
    }
private:
    //functs
    int _get_last_recno();
    void _reindex();
    
    vectorstr _get_record(long recno);
    vectorstr _get_record(const vectorstr& fields, long recno);
    
    vectorlong _cond(Queue<Token*>& postfix);
    Table _select_recnos(const vectorstr& fields, const vectorlong& recnos);

    Queue<Token*> _tokenize_vectorstr(const vectorstr& infix);

    //vars
    vector<MMap<string,long>*> _indices;//field name, recno vectors
                                        //data in here
                                        //contains multimap for each field

                                        //search in mmap >
                                        //[0] = 'elizabeth',{###, ###, recnos}  

    Map<string,long> _fields;           //FIELDNAME, INDEX associated w/ _indices vec
                                        //firstn, 0
    
    vectorlong _rec_indices;             //'73', 65, 20
    vectorstr _field_names;             //labels
    string _table_name;
    int _last_recno;
    bool _empty;
    static int _serial;
};

// Map<string,MMap<string,long>>    //fieldname, actual value (eg: first, 'elizabeth', {##, ###, recnos})
                                    //values associated w the field 

#endif // TABLE_H
