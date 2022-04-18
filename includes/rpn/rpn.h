// #ifndef RPN_H
// #define RPN_H
// #include "../tokenizer/constants.h"
// #include "../queue/MyQueue.h"
// #include "../stack/MyStack.h"
// #include "../tokens/token.h"
// #include "../tokens/token_children.h"

// class RPN{
// public:
//     RPN(Queue<Token*> &q, vector<mmap_sl*>& indices, mmap_sl& fields);
//     void set_input(Queue<Token*> &q);
//     vectorlong operator ()();    

// private:
//     //VARIABLES
//     Queue<Token*> _postfix;
//     vectorlong _recnos;
//     bool _evaluated;

//     vector<mmap_sl*>* _indices;
//     mmap_sl* _fields;

//     //FUNCTIONS
//     vectorlong evaluate();
// };

// #endif //RPN_H