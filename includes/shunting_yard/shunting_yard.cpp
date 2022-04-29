#include "shunting_yard.h"

//CTOR
ShuntingYard::ShuntingYard():_in_postfix(false){
    _queue=Queue<Token*>();
}
ShuntingYard::ShuntingYard(Queue<Token*> &q):_queue(q), _in_postfix(false){}
void ShuntingYard::infix(Queue<Token*>& q){
    _queue=q;
    _in_postfix=false;
}
//FUNCTIONS
Queue<Token*>& ShuntingYard::postfix(){
    if(_in_postfix)
        return _queue;

    //CHANGE FROM INFIX TO POSTFIX
    Queue<Token*> postfix;    //_queue holds infix value
    Stack<Token*> operators;

    while(!_queue.empty()){
        Token* pop=_queue.pop();
        switch(pop->Type()){
            case STRING:{
                postfix.push(pop);
                break;
            }
            case LPAREN:{
                operators.push(pop);
                break;
            }
            case RPAREN:{
                Token* poppedOp=operators.pop();
                while(poppedOp->Type()!=LPAREN){
                    //pop operators stack into pf q until u see LPAREN
                    postfix.push(poppedOp);     //push op to pf q
                    if(syDebug){cout<<"     RPAREN: pushed "<<poppedOp<<endl;}
                    poppedOp=operators.pop();   //get next op -> exits while loop with poppedOp holding LPAREN
                    if(syDebug){cout<<"     RPAREN: holding "<<poppedOp<<endl;}
                }
                assert(poppedOp->Type()==LPAREN && pop->Type()==RPAREN); //discard parentheses
                break;
            }
            case LOGICAL:
            case RELATIONAL:
            case OPERATOR:{
                if(operators.empty() || operators.top()->Type()==LPAREN){
                    operators.push(pop);
                }
                else{
                    int popPrec=static_cast<Operator*>(pop)->Precedence();
                    int stackPrec=static_cast<Operator*>(operators.top())->Precedence();                    
                    if(popPrec<stackPrec){
                        operators.push(pop);
                    }
                    else{
                        while(!operators.empty() && (operators.top()->Type()==OPERATOR||operators.top()->Type()==LOGICAL||operators.top()->Type()==RELATIONAL) && popPrec>=static_cast<Operator*>(operators.top())->Precedence()){
                            postfix.push(operators.pop());
                        }
                        operators.push(pop);
                    }
                }
                break;
            }
            default:{
                cout<<"INVALID TOKEN: "<<*pop<<endl;
                assert(false);
            }
        }
    }
    while(!operators.empty()){
        Token* pop=operators.pop();
        assert(pop->Type()==OPERATOR || pop->Type()==LOGICAL|| pop->Type()==RELATIONAL);
        postfix.push(pop);
    }
    assert(_queue.empty() && operators.empty());
    
    //update priv vars!        
    _queue=postfix;
    _in_postfix=true;

    if(syDebug){
        cout<<_queue<<endl;
        int c=1;
        for(Queue<Token*>::Iterator i=_queue.begin(); i!=_queue.end(); i++){
            cout<<"["<<c++<<"]: "<<**i<<endl;
        }
    }

    return _queue;
}
Queue<Token*>& ShuntingYard::postfix(Queue<Token*> &q){
    _queue=q;
    _in_postfix=false;
    return postfix();
}