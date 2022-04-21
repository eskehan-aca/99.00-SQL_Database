#include "sql.h"
SQL::SQL(){}
SQL::SQL(const string& fname){
    int queries=0;
    cout<<"> BATCH PROCESSING FILE \""<<fname<<"\""<<endl;
    string cmd;
    ifstream f(fname);
    assert(f.is_open());
    while(getline(f,cmd)){
        if(cmd.substr(0,2)=="//")
            cout<<cmd<<endl;
        else if(!cmd.empty())
            cout<<"["<<++queries<<"]\t"<<cmd<<endl<<command(cmd);
    }
    f.close();
    cout<<"> END OF BATCH PROCESS"<<endl;
}
vectorlong& SQL::select_recnos(){
    return _recnos;
}
Table SQL::command(const string& cmd){
    char c[cmd.size()+1];
    strcpy(c, cmd.c_str());
    
    Table t;
    Parser p(c);

    mmap_ss ptree=p.parse_tree();
    
    string tablename=ptree["table_name"][0];
    string s=ptree["command"][0];
    
    if(s=="make"||s=="create"){
        t=Table(tablename,ptree["fields"]);
    }
    else if(s=="select"){
        t=Table(tablename);
        if(ptree["fields"][0]=="*"){
            if(ptree.contains("condition")){
                t=t.select(t.get_fields(),ptree["condition"]);
            }
            else
                t=t.select_all();
        }
        else{
            if(ptree.contains("condition")){
                t=t.select(ptree["fields"],ptree["condition"]);
            }
            else{
                t=t.select_all(ptree["fields"]);
                t.set_fields(ptree["fields"]);
            }
        }
    }
    else if(s=="insert"){
        t=Table(tablename);
        t.insert_into(ptree["values"]);
    }
    _recnos=t.select_recnos();
    return t;
}