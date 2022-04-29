#include "table.h"
int Table::_serial=0;

Table::Table(){
    _empty=true;
    _last_recno=-1;
    _table_name="";
}
Table::Table(const string& name){
    // vectorlong _rec_indices;             --handle in reindex
    // int _last_recno;                     --handle in reindex
    // bool _empty;                         --handle here if empty, 
    // static int _serial;                  --do not increment
    _table_name=name;
    set_fields(get_fields());
    
    for(int i=0; i<_field_names.size(); i++){
        //init _fields 
        _fields.insert(_field_names[i],i);
        //init _indices vec
        _indices.push_back(new mmap_sl());    //inserting empty maps (containers)
    }

    _get_last_recno();
    _reindex();
}
Table::Table(const string& name, const vectorstr& field){
    _serial++;
    _empty=true;
    _last_recno=-1;
    _table_name=name;
    _field_names=field;

    ofstream f(name+"_fields.txt");
    assert(f.is_open());
    for(int i=0; i<_field_names.size(); i++){
        //write _field_names into file
        f<<_field_names[i]<<endl;
        //init _fields 
        _fields.insert(_field_names[i],i);
        //init _indices vec
        _indices.push_back(new mmap_sl());    //inserting empty maps (containers)
    }
    f.close();
    
    string fname=_table_name+".tbl";
    fstream rec;
    open_fileW(rec,fname.c_str());
    rec.close();
}
Table::~Table(){}

void Table::insert_into(const vectorstr& record){   //record={'joe, biden, 200'}
    FileRecord rec(record);  //obj record filled
    fstream f;
    string fname=_table_name+".tbl";
    open_fileRW(f,fname.c_str());
    _last_recno=rec.write(f);       //check return of rec.write, should =0 upon first insert
    _rec_indices.push_back(_last_recno);
    for(int i=0; i<record.size(); i++){
        _indices[i]->at(record[i]).push_back(_last_recno);
    }  
    _empty=false;
}
Queue<Token*> Table::_tokenize_vectorstr(const vectorstr& infix){
    Queue<Token*> q;
    for(int i=0; i<infix.size(); i++){
        char c[infix[i].length()];
        strcpy(c,infix[i].c_str());
        STokenizer stk(c);
        Token t;
        stk>>t;
        if(t.token_str().size()!=infix[i].size()){
                string s="";
                while(stk.more()){
                    s+=t.token_str();
                    stk>>t;
                }
                q.push(new TokenStr(s));
        }
        else{       
            switch(t.type()){
                case TOKEN_NUMBER:
                case TOKEN_ALPHA:{
                    if(t.token_str()=="and"||t.token_str()=="or"){
                        q.push(new Logical(t.token_str()));
                        break;
                    }
                    q.push(new TokenStr(t.token_str()));
                    break;
                }
                case TOKEN_OPERATOR:{
                    q.push(new Relational(t.token_str()));
                    break;
                }
                case TOKEN_UNKNOWN:{
                    if(t.token_str()=="("){
                        q.push(new LParen());
                        break;
                    }
                    else if(t.token_str()==")"){
                        q.push(new RParen());
                        break;
                    }
                }
                default:{
                    break;
                }
            }
        }
    }
    return q;
}
Table Table::select(const vectorstr& fields, const vectorstr& infix){
    #include "../shunting_yard/shunting_yard.h"
    Queue<Token*> q=_tokenize_vectorstr(infix);
    ShuntingYard sy(q);  
    return select(fields, sy.postfix());
}
Table Table::select(const vectorstr& fields, const string& field, const string& op, const string& value){
    vectorstr infix;
    infix.push_back(field);
    infix.push_back(op);
    infix.push_back(value);
    return select(fields, infix);
}
Table Table::select(const vectorstr& fields, Queue<Token*>& postfix){
    vectorlong recnos=_cond(postfix);   //sets rec indices
    return _select_recnos(fields, recnos);
}
vectorlong& Table::select_recnos(){
    return _rec_indices;
}
Table Table::select_all(){
    if(_rec_indices.size()!=_last_recno+1){
        _rec_indices.clear();
        for(int i=0; i<=_last_recno; i++)
            _rec_indices.push_back(i);
    }
    return _select_recnos(_field_names, _rec_indices);
}
Table Table::select_all(const vectorstr& fields){
    if(_rec_indices.size()!=_last_recno+1){
        _rec_indices.clear();
        for(int i=0; i<=_last_recno; i++)
            _rec_indices.push_back(i);
    }
    return _select_recnos(fields, _rec_indices);
}
vectorlong Table::_cond(Queue<Token*>& postfix){
    //rpn:
    assert(!postfix.empty());
    Stack<Token*> s;
    vectorlong result;
    while(!(postfix.empty())){
        Token* pop=postfix.pop();
        switch(pop->Type()){
            case STRING:{
                s.push(pop);
                break;
            }
            case RELATIONAL:{   //=, etc
                Token* right=s.pop();    //stack pop1 = right   (value)
                Token* left=s.pop();     //stack pop2 = left    (field)

                assert(right->Type()==STRING && left->Type()==STRING);
                //perform operation
                int k=_fields.get(left->token_str());
                result=static_cast<Relational*>(pop)->evaluate(_indices[k],right);
                
                //push result to stack
                s.push(new ResultSet(result));
                break;
            }
            case LOGICAL:{
                Token* right=s.pop();    
                Token* left=s.pop();     

                assert(right->Type()==RESULTSET && left->Type()==RESULTSET);
                result=static_cast<Logical*>(pop)->evaluate(right,left);

                //push result to stack
                s.push(new ResultSet(result));
                break;
            }
            case OPERATOR:
            default:{
                cout<<"INVALID TOKEN, TYPE: "<<pop->Type()<<"\tTOKEN: "<<*pop<<endl;
                assert(false);
                break;
            }
        }
    }
    assert(postfix.empty() && s.size()==1);
    result=static_cast<ResultSet*>(s.pop())->recnos();
    _rec_indices=result;
    return result;
}
vectorstr Table::_get_record(long recno){
    return _get_record(_field_names, recno);
}
vectorstr Table::_get_record(const vectorstr& fields, long recno){
    vectorstr record;
    FileRecord rec;
    fstream f;
    string fname=_table_name+".tbl";
    open_fileRW(f,fname.c_str());    
    rec.read(f,recno);
    for(int j=0; j<fields.size(); j++){
        int k=_fields.get(fields[j]);
        string s=rec._record[k];
        record.push_back(s);
    }
    return record;
}
Table Table::_select_recnos(const vectorstr& fields, const vectorlong& recnos){
    Table t("select_recnos"+to_string(_serial),fields); //create table
    for(int i=0; i<recnos.size(); i++){
        t.insert_into(_get_record(fields, recnos[i]));
    }
    t.set_recnos(recnos);
    if(tblDebug){
        cout<<"================"<<endl;
        cout<<t<<endl;
        cout<<"================"<<endl;
    }
    return t;
}
vectorstr& Table::get_fields(){
    string field;
    ifstream f(_table_name+"_fields.txt");
    assert(f.is_open());
    _field_names.clear();
    while(getline(f,field) && field!="\0" && field!=""){
        _field_names.push_back(field);
    }
    return _field_names;
}
int Table::_get_last_recno(){
    FileRecord rec;
    fstream f;
    string fname=_table_name+".tbl";
    open_fileRW(f,fname.c_str());    
    f.seekg(0, ios::end);
    long pos=f.tellg();
    _last_recno=(pos/sizeof(rec._record))-1;
    _empty=_last_recno==0;
    return _last_recno;
}
void Table::set_fields(const vectorstr& fields){
    _field_names=fields;
}
void Table::_reindex(){
    for(int recno=0; recno<=_last_recno; recno++){
        vectorstr record=_get_record(_field_names,recno);
        _rec_indices.push_back(recno);
        for(int i=0; i<record.size(); i++){
            _indices[i]->at(record[i]).push_back(recno);
        }  
        _empty=false;
    }
}
bool Table::empty() const{
    return _empty;
}
bool Table::set_recnos(const vectorlong& recnos){
    assert(_rec_indices.size()==recnos.size());
    _rec_indices=recnos;
}