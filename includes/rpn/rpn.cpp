// #include "rpn.h"

// //CTOR
// RPN::RPN(Queue<Token*> &q, vector<mmap_sl*>& indices, mmap_sl& fields):_postfix(q),_indices(&indices),_fields(&fields),_evaluated(false){}

// //FUNCTIONS
// void RPN::set_input(Queue<Token*> &q){
//     _postfix=q;
//     _evaluated=false;
// }

// vectorlong RPN::evaluate(){
//     if(_evaluated)  //will be true if prev given # for var and no arg this time or alr eval this funct w/ no var values
//         return _recnos;
    
//     assert(!postfix.empty());
//     Stack<Token*> s;
//     vectorlong result;
//     while(!(postfix.empty())){
//         Token* pop=postfix.pop();
//         switch(pop->Type()){
//             case STRING:{
//                 s.push(pop);
//                 break;
//             }
//             case RELATIONAL:{
//                 Token* right=s.pop();    //stack pop1 = right   (value)
//                 Token* left=s.pop();     //stack pop2 = left    (field)

//                 assert(right->Type()==STRING && left->Type()==STRING);
//                 //perform operation
//                 int k=_fields->get(left->token_str());
//                 result=static_cast<Relational*>(pop)->evaluate(_indices->get(k),right);
                
//                 //push result to stack
//                 s.push(new ResultSet(result));
//                 break;
//             }
//             case LOGICAL:{
//                 Token* right=s.pop();    
//                 Token* left=s.pop();     

//                 assert(right->Type()==RESULTSET && left->Type()==RESULTSET);
//                 result=static_cast<Logical*>(pop)->evaluate(right,left);

//                 //push result to stack
//                 s.push(new ResultSet(result));
//                 break;
//             }
//             case OPERATOR:
//             default:{
//                 cout<<"INVALID TOKEN, TYPE: "<<pop->Type()<<"\tTOKEN: "<<*pop<<endl;
//                 assert(false);
//                 break;
//             }
//         }
//     }
//     assert(postfix.empty() && s.size()==1);
//     result=static_cast<ResultSet*>(s.pop())->recnos();
//     return result;
// }
// vectorlong RPN::operator ()(){
//     return evaluate();
// }