#include <iostream>
#include "includes/tokenizer/ftokenize.h"
#include "includes/bplustree/multimap.h"
#include "includes/bplustree/map.h"
#include <string>

using namespace std;

MMap<string, long> get_word_indices(char* file_name);

int main(int argc, char *argv[]){
    MMap<string, long> word_indices;
    word_indices = get_word_indices("solitude.txt");
    cout<<endl<<endl<<endl;

    //list all nodes of the index mmap:
    for (MMap<string, long>::Iterator it = word_indices.begin();
         it != word_indices.end(); it++){
        cout<<*it<<endl;
    }


    cout<<endl<<endl<<endl;
    cout<<"---------------------------------------------------"<<endl;
    string this_word = "ice";
    cout<<"---------------------------------------------------"<<endl;
    cout<<"Indices of \""<<this_word<<"\""<<endl;
    //list indices of this_word:
    if (word_indices.contains(this_word)){
        cout<<this_word<<": "<<word_indices[this_word]<<endl;
    }
    cout<<endl<<endl<<endl;

    cout<<"---------------------------------------------------"<<endl;
    string from = "ask";
    string to = "asker";
    //list from .. to:
    cout<<"listing indices from \""<<from<<"\" to \""<<to<<"\""<<endl;
    cout<<"---------------------------------------------------"<<endl;
    for (MMap<string, long>::Iterator it =
                word_indices.lower_bound(from);
         it != word_indices.upper_bound(to) &&
         it != word_indices.end(); it++){
        cout<<*it<<endl;
    }

    cout <<endl<<endl<<endl<< "========== E N D  ====================" << endl;
    return 0;
}


MMap<string, long> get_word_indices(char* file_name){
    const bool debug = false;
    MMap<string, long> word_indices;
    FTokenizer ftk("solitude.txt");
    Token t;
    long count = 0;

    ftk >> t;
    while (ftk.more()){
        //only the "words"
        if (t.type_string() == "ALPHA"){
            string s;
            s = t.token_str();
            word_indices[s] += count;
            count++;
            if (debug)
                cout<<"|"<<t.token_str()<<"|"<<endl;
        }
        ftk >> t;
    }
    return word_indices;
}