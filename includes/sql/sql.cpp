#include "sql.h"
SQL::SQL(/* args */){}
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
    if(s=="select"){
        t=Table(tablename);
        if(ptree["fields"][0]=="*"){
            if(ptree.contains("condition")){
                t.select(t.get_fields(),ptree["condition"]);
            }
            else
                t.select_all();
        }
        else{
            if(ptree.contains("condition")){
                t.select(ptree["fields"],ptree["condition"]);
            }
            else{
                t.select_all(ptree["fields"]);
                t.set_fields(ptree["fields"]);
            }
        }
    }
    if(s=="insert"){
        t=Table(tablename);
        t.insert_into(ptree["values"]);
    }
    _recnos=t.select_recnos();
    return t;
}