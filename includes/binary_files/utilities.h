#ifndef UTILITIES_H
#define UTILITIES_H
#include <cmath>
#include <iostream>
#include <iomanip>
#include <set>
#include <vector>
#include <string>
#include <cstring>
#include <fstream>
using namespace std;

bool file_exists(const char filename[]);
void open_fileRW(fstream& f, const char filename[]) throw(char*);
void open_fileW(fstream& f, const char filename[]);

void write_info(fstream& f, const char filename[]);
void read_info(fstream& f, const char filename[]);

#endif // UTILITIES_H