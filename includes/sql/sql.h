#ifndef SQL_H
#define SQL_H
#include "../table/typedefs.h"
#include "../table/table.h"
#include "../parser/parser.h"

class SQL{
public:
    SQL();
    SQL(const string& fname);
    Table command(const string& cmd);
    vectorlong& select_recnos();
private:
    vectorlong _recnos;
};



#endif // SQL_H
