#ifndef SHUNTING_YARD_H
#define SHUNTING_YARD_H
#include "../tokenizer/constants.h"
#include "../queue/MyQueue.h"
#include "../stack/MyStack.h"
#include "../tokens/token.h"
#include "../tokens/token_children.h"
#include "../tokenizer/stokenize.h"

class ShuntingYard{
public:
    ShuntingYard();
    // ShuntingYard(vectorstr &infix);
    ShuntingYard(Queue<Token*> &q);

    //modify priv variable
    void infix(Queue<Token*>& q);   //same as set input

    //return postfix queue
    Queue<Token*>& postfix();
    Queue<Token*>& postfix(Queue<Token*> &q);

private:
    Queue<Token*> _queue;
    bool _in_postfix;  //false meaning expression not yet corrected
};

#endif //SHUNTING_YARD_H 