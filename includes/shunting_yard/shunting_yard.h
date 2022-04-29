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
    ShuntingYard(Queue<Token*> &q);
    // ShuntingYard(vectorstr &infix);

    //modify priv variable
    void infix(Queue<Token*>& q);   //same as set input

    //return postfix queue
    Queue<Token*>& postfix();
    Queue<Token*>& postfix(Queue<Token*> &q);

private:
    Queue<Token*> _queue;
    bool _in_postfix;
};

#endif //SHUNTING_YARD_H 