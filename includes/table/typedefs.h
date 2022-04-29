#ifndef TYPEDEFS_H
#define TYPEDEFS_H
#include <assert.h>
#include <iostream>
#include <iostream>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <vector>
#include <cmath>
#include <set>
#include "../tokens/token.h"
#include "../stack/MyStack.h"
#include "../queue/MyQueue.h"
#include "../bplustree/map.h"
#include "../bplustree/multimap.h"
#include "../tokenizer/ftokenize.h"
#include "../tokens/token_children.h"
#include "../binary_files/file_record.h"
#include "../shunting_yard/shunting_yard.h"

using namespace std;

typedef vector<string> vectorstr;
typedef vector<long> vectorlong;

typedef Map<string,long> map_sl;
typedef MMap<string,long> mmap_sl;
typedef MMap<string,string> mmap_ss;

typedef Queue<Token*> tokenq;

#endif // TYPEDEFS_H
