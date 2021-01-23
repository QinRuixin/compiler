#include "ir.h"
#include "tree.h"
#include "intercode.h"
#include "cstring"
#include <fstream>
#include <map>
#include <list>

// ------------ intermediate representation ------------

using namespace std;
list<InterCode*> InterCodes;
int temp_cnt;
int label_cnt;
int var_cnt;

void append_code(InterCode* cur_code){
    InterCodes.push_back(cur_code);
}

string new_var(){
    string temp("v");
    return temp + to_string(++var_cnt);
}

string new_temp(){
    string temp("t");
    return temp + to_string(++temp_cnt);
}

string new_label(){
    string label("label");
    return label + to_string(++label_cnt);
}

Operand* new_var_operand(string _name){
    Operand* res = (Operand*)malloc(sizeof(Operand));
    res->kind = res->VARIABLE;
    res->u.value = _name;
    return res;
}
Operand* new_getadd_operand(string _name){
    Operand* res = (Operand*)malloc(sizeof(Operand));
    res->kind = res->GET_ADDRESS;
    res->u.value = _name;
    return res;
}
Operand* new_constant_operand(int val_num){
    Operand* res = (Operand*)malloc(sizeof(Operand));
    res->kind = res->CONSTANT;
    res->u.val_no = val_num;
    return res;
}

Operand* new_label_operand(string _name){
    Operand* res = (Operand*)malloc(sizeof(Operand));
    res->kind = res->LABEL;
    res->u.value = _name;
    return res;
}


InterCode* new_assign_code(Operand* operand_left,Operand* operand_right){
        InterCode* cur_code = (InterCode*) malloc(sizeof(InterCode));
        cur_code->kind = cur_code->ASSIGN;
        cur_code->u.assign.left = operand_left;
        cur_code->u.assign.right = operand_right;
        return cur_code;
}

InterCode* new_binop_code(Operand* res, Operand* operand1,Operand* operand2){
        InterCode* cur_code= (InterCode*) malloc(sizeof(InterCode));
        cur_code->u.binop.result = res;
        cur_code->u.binop.op1 = operand1;
        cur_code->u.binop.op2 = operand2;
        return cur_code;

}

InterCode* new_sinop_code(Operand* op){
        InterCode* cur_code= (InterCode*) malloc(sizeof(InterCode));
        cur_code->u.sinop.op = op;
        return cur_code;
}

InterCode* new_dec_code(Operand* op, int size){
    InterCode* cur_code= (InterCode*) malloc(sizeof(InterCode));
    cur_code->u.dec.op = op;
    cur_code->u.dec.size = size;
    return cur_code;
}

InterCode* new_ifop_code(Operand* t1, Operand* t2, Operand* label_true,string op_rel){
        InterCode* cur_code= (InterCode*) malloc(sizeof(InterCode));
        cur_code->kind = cur_code->IFGOTO;
        cur_code->u.ifop.t1 = t1;
        cur_code->u.ifop.t2 = t2;
        cur_code->u.ifop.label = label_true;
        cur_code->u.ifop.op_rel = op_rel;
        return cur_code;
}


void printOperand(std::ofstream& outputfile, Operand* operand){
    if(operand==nullptr){
        return; //todo?  place is null
    }
    switch (operand->kind)
    {
    case operand->VARIABLE:
        outputfile << operand->u.value;
        break;
    case operand->CONSTANT:
        outputfile << "#" << operand->u.val_no; //todo  float?
        break;
    case operand->LABEL:
        outputfile << operand->u.value;
        break;
    case operand->ADDRESS:
        outputfile << "&" << operand->u.value;
        break;
    case operand->DEADD:
        outputfile << "*" << operand->u.value;
        break;
    default:
        break;
    }
}

void printCode(std::ofstream& outputfile){
    for(InterCode* interCode: InterCodes){
        switch (interCode->kind)
        {
        case interCode->ASSIGN:{
            auto stru_assign = interCode->u.assign;
            printOperand(outputfile, stru_assign.left);
            outputfile << " := " ;
            printOperand(outputfile, stru_assign.right);
            outputfile << endl;
            //outputfile << stru_assign.left->u.value << " := " << endl;
            break;
        }
        case interCode->ADD:{
            auto stru_binop = interCode->u.binop;
            printOperand(outputfile, stru_binop.result);
            outputfile << " := " ;
            printOperand(outputfile, stru_binop.op1);
            outputfile << " + " ;
            printOperand(outputfile, stru_binop.op2);
            outputfile << endl;
            break;
        }
        case interCode->SUB:{
            auto stru_binop = interCode->u.binop;
            printOperand(outputfile, stru_binop.result);
            outputfile << " := " ;
            printOperand(outputfile, stru_binop.op1);
            outputfile << " - " ;
            printOperand(outputfile, stru_binop.op2);
            outputfile << endl;
            break;
        }
        case interCode->MUL:{
            auto stru_binop = interCode->u.binop;
            printOperand(outputfile, stru_binop.result);
            outputfile << " := " ;
            printOperand(outputfile, stru_binop.op1);
            outputfile << " * " ;
            printOperand(outputfile, stru_binop.op2);
            outputfile << endl;
            break;
        }
        case interCode->DIV:{
            auto stru_binop = interCode->u.binop;
            printOperand(outputfile, stru_binop.result);
            outputfile << " := " ;
            printOperand(outputfile, stru_binop.op1);
            outputfile << " / " ;
            printOperand(outputfile, stru_binop.op2);
            outputfile << endl;
            break;
        }
        case interCode->RETURN:{
            auto stru_sinop = interCode->u.sinop;
            outputfile << "RETURN ";
            printOperand(outputfile, stru_sinop.op);
            outputfile << endl;
            //outputfile << endl;
            break;
        }
        case interCode->LABEL:{
            auto stru_sinop = interCode->u.sinop;
            outputfile << "LABEL ";
            printOperand(outputfile, stru_sinop.op);
            outputfile << " :";
            outputfile << endl;
            break;
        }
        case interCode->GOTO:{
            auto stru_sinop = interCode->u.sinop;
            outputfile << "GOTO ";
            printOperand(outputfile, stru_sinop.op);
            outputfile << endl;
            break;
        }
        case interCode->IFGOTO:{
            auto stru_ifop = interCode->u.ifop;
            outputfile << "IF ";
            printOperand(outputfile, stru_ifop.t1);
            outputfile << " ";
            outputfile << stru_ifop.op_rel;
            outputfile << " ";
            printOperand(outputfile, stru_ifop.t2);
            outputfile << " GOTO ";
            printOperand(outputfile, stru_ifop.label);
            outputfile << endl;
            break;
        }
        case interCode->FUNCTION:{
            outputfile << endl;
            auto stru_sinop = interCode->u.sinop;
            outputfile << "FUNCTION ";
            printOperand(outputfile, stru_sinop.op);
            outputfile << " :";
            outputfile << endl;
            break;
        }
        case interCode->PARAM:{
            auto stru_sinop = interCode->u.sinop;
            outputfile << "PARAM ";
            printOperand(outputfile, stru_sinop.op);
            outputfile << endl;
            break;
        }
        case interCode->READ:{
            auto stru_sinop = interCode->u.sinop;
            outputfile << "READ ";
            printOperand(outputfile, stru_sinop.op);
            outputfile << endl;
            break;
        }
        case interCode->WRITE:{
            auto stru_sinop = interCode->u.sinop;
            outputfile << "WRITE ";
            printOperand(outputfile, stru_sinop.op);
            outputfile << endl;
            break;
        }
        case interCode->CALL:{
            auto stru_assign = interCode->u.assign;
            printOperand(outputfile, stru_assign.left);
            outputfile << " := CALL ";
            printOperand(outputfile, stru_assign.right);
            outputfile << endl;
            break;
        }
        case interCode->ARG:{
            auto stru_sinop = interCode->u.sinop;
            outputfile << "ARG ";
            printOperand(outputfile, stru_sinop.op);
            outputfile << endl;
            break;
        }
        case interCode->DEC:{
            auto stru_dec = interCode->u.dec;
            outputfile << "DEC ";
            printOperand(outputfile, stru_dec.op);
            outputfile << " ";
            outputfile << stru_dec.size;
            outputfile << endl;
            break;
        }
        default:
            break;
        }
    }
}

void Translate(tree_node* ptr,std::map<std::string, struct Sysmtable_item>& Sysmtable){
    if(ptr==nullptr){
        return;
    }

//cout << "ptr->node_type : " << ptr->node_type <<endl;

    NODE_TYPE cur_type = ptr->node_type;
    switch (cur_type)
    {
    case ENUM_Exp:
    //todo?
//cout <<"ENUM_Exp : child_num "<<ptr->child_num << " line_no " << ptr->line_no << endl;
        TranslateExp(ptr, Sysmtable, nullptr);
        break;
    case ENUM_Stmt:
        TranslateStmt(ptr, Sysmtable);
        break;
    case ENUM_CompSt:
        TranslateCompSt(ptr, Sysmtable);
        break;
    case ENUM_FunDec:
        TranslateFunDec(ptr, Sysmtable);
        break;
    case ENUM_VarDec:
        TranslateVarDec(ptr, Sysmtable);
        break;
    //todo
    default:
        int child_nums = ptr->child_num;
//        cout <<"child_nums : " << child_nums << endl;
        for(int i = 0; i < child_nums; ++i){
            Translate(ptr->child_node[i], Sysmtable);
        }
        break;
    }

}

void TranslateProgram(tree_node* ptr,std::map<std::string, struct Sysmtable_item>& Sysmtable, std::ofstream& outputfile){
    temp_cnt = 0;
    label_cnt = 0;
    var_cnt = 0;
    if(ptr==nullptr){
        return;
    }
    //todo add read and write function
/*    for(auto sysm :Sysmtable){
        cout << sysm.second.name << endl;
        cout << sysm.first << endl;
    } */
    int child_nums = ptr->child_num;
    for(int i = 0; i < child_nums; ++i){
        Translate(ptr->child_node[i], Sysmtable);
    }
    printCode(outputfile);

}

void TranslateExp(tree_node* ptr,std::map<std::string, struct Sysmtable_item>& Sysmtable,Operand* place){

    if(ptr==nullptr){
        return;
    }
    int child_nums = ptr->child_num;
    if(child_nums == 1){ // INT or ID
        InterCode* cur_code;
        tree_node* ptr_child0 = ptr->child_node[0];
        if(ptr_child0->node_type==ENUM_INT){
            if(place!=nullptr){
                Operand* r_operand = new_constant_operand(ptr_child0->int_val);
                cur_code = new_assign_code(place, r_operand);
                append_code(cur_code);
            }
        }else if(ptr_child0->node_type==ENUM_ID){     
            auto it = Sysmtable.find(ptr_child0->node_name);
            if(place!=nullptr){
                Operand* r_operand = new_var_operand(it->second.name);
                cur_code = new_assign_code(place, r_operand);
                append_code(cur_code);
            }
        }
        return;
    }
    // has 2 or 3 child nodes

    tree_node* ptr_child0 = ptr->child_node[0]; 
    tree_node* ptr_child1 = ptr->child_node[1]; 
    if(ptr_child0->node_type==ENUM_LP){ // LP Exp RP
        TranslateExp(ptr_child1,Sysmtable,place);
        return;
    }
    // Exp1 LB Exp2 RB
    if(ptr_child1->node_type==ENUM_LB){
        tree_node* ptr_child2 = ptr->child_node[2]; 
        string t1 = new_temp();
        string t2 = new_temp();
        Operand* operand_t1 = new_var_operand(t1);
        Operand* operand_t2 = new_var_operand(t2);
        TranslateExp(ptr_child2,Sysmtable,operand_t2);
        InterCode* cur_code = new_binop_code(operand_t1,operand_t2,new_constant_operand(4) );
        cur_code->kind = cur_code->MUL;
        append_code(cur_code);
        // right value   & Exp1->ID->name  + operand_t1
        tree_node* ptr_ID = ptr_child0->child_node[0];
        Operand* operand_ID = new_getadd_operand(ptr_ID->node_name);
        InterCode* cur_code3 = new_binop_code(place, operand_ID, operand_t1);
        cur_code3->kind = cur_code3->ADD;
        //todo
//        place->kind = place->ADDRESS;
        append_code(cur_code3);
        return;
    }
    if (ptr_child0->node_type==ENUM_ID )
    {
        if(ptr->child_num == 3){    // ID LP RP 
            if( strcmp(ptr_child0->node_name,"read")==0 && place!=nullptr){
                InterCode* cur_code = new_sinop_code(place);
                cur_code->kind = cur_code->READ;
                append_code(cur_code);
                //cout << "read" << endl;
                return;
            }else { // if(place!=nullptr)
                Operand* func = new_var_operand(ptr_child0->node_name);
                InterCode* cur_code = new_assign_code(place, func);
                cur_code->kind = cur_code->CALL;
                append_code(cur_code);
                //cout << "call" << endl;
                return;
//todo place is nullptr?
            }
        }else{  // ID LP Args RP
            tree_node* ptr_child2 = ptr->child_node[2]; 
            list<Operand*> arg_list;
            TranslateArgs(ptr_child2, Sysmtable, arg_list);
            auto it = arg_list.begin();
            if( strcmp(ptr_child0->node_name,"write")==0){
                InterCode* cur_code = new_sinop_code( *it );
                cur_code->kind = cur_code->WRITE;
                append_code(cur_code);
                return;
            }else{
                while (it != arg_list.end())
                {
                    InterCode* cur_code = new_sinop_code( *it );
                    cur_code->kind = cur_code->ARG;
                    append_code(cur_code);
                    ++it;
                }
                Operand* func = new_var_operand(ptr_child0->node_name);
                InterCode* cur_code = new_assign_code(place, func);
                cur_code->kind = cur_code->CALL;
                append_code(cur_code);
                //cout << "call" << endl;
                return;
            }
        }
        //Sysmtable_item cur_item = Sysmtable.find(ptr_child0->node_name)->second;
        return;
    }
    if(ptr_child0->node_type== ENUM_MINUS){     //MINUS Exp1
        // ptr_child1 Exp1
        string t1 = new_temp();
        Operand* operand_t1 = new_var_operand(t1);
        TranslateExp(ptr_child1,Sysmtable,operand_t1); //cur_code1
        if(place!=nullptr){
            InterCode* cur_code2= new_binop_code(place, new_constant_operand(0), operand_t1 );
            cur_code2->kind = cur_code2->SUB;
            append_code(cur_code2);
        }
        return;
    }else if(ptr_child1->node_type== ENUM_ASSIGNOP){  // Exp1 ASSIGNOP Exp2
//cout << "ENUM_ASSIGNOP" << endl;
        // ptr_child0   Exp1 is ID
        if(ptr_child0->child_num == 1){
            auto it = Sysmtable.find(ptr_child0->child_node[0]->node_name); // Exp1 -> ID get ID name
            string t1 = new_temp();
            Operand* operand_t1 = new_var_operand(t1);
            TranslateExp(ptr->child_node[2],Sysmtable,operand_t1);
            Operand* operand_t2 = new_var_operand(t1); // the same name but different kind
            Operand* operand_var = new_var_operand(it->second.name); 
            if(ptr->child_node[2]->child_num==4 && ptr->child_node[2]->child_node[1]->node_type==ENUM_LB){
                //operand_t2 = new_var_operand("*"+t1); // the same name but different kind
                operand_t2->kind = operand_t2->ADDRESS;
            }
            operand_var->kind = operand_t2->kind;
            InterCode* cur_code1= new_assign_code(operand_var, operand_t2);
            append_code(cur_code1);
            if(place!=nullptr){
                InterCode* cur_code2= new_assign_code(place, operand_var);
//                InterCode* cur_code2= new_assign_code(place, operand_t1);
                append_code(cur_code2);
            }
            return;
        }
        //  Exp1 is array    Exp1 ASSIGNOP Exp2
//        if(ptr_child0->child_num == 4) {
        string t1 = new_temp();
        Operand* operand_t1 = new_var_operand(t1);
        TranslateExp(ptr_child0,Sysmtable,operand_t1);
        Operand* operand_t2 = new_var_operand(t1); // the same name but different kind
        if(ptr_child0->child_node[1]->node_type==ENUM_LB ){    //array
            //operand_t2 = new_var_operand("*"+t1); // the same name but different kind
            operand_t2->kind = operand_t2->ADDRESS;
        }
        TranslateExp(ptr->child_node[2],Sysmtable,operand_t2);
//            Operand* operand_var = new_var_operand(it->second.name); 
//            InterCode* cur_code1= new_assign_code(operand_var, operand_t1);
//            append_code(cur_code1);
        if(place!=nullptr){
//                InterCode* cur_code2= new_assign_code(place, operand_var);
            InterCode* cur_code2= new_assign_code(place, operand_t1);
            append_code(cur_code2);
        }
//        }

        return;
    }else if(ptr_child1->node_type== ENUM_PLUS ||
        ptr_child1->node_type== ENUM_MINUS ||
        ptr_child1->node_type== ENUM_DIV ||
        ptr_child1->node_type== ENUM_STAR
        ){    //Exp1 PLUS Exp2
        // ptr_child0 Exp1

        string t1 = new_temp();
        string t2 = new_temp();
        Operand* operand_t1 = new_var_operand(t1);
        Operand* operand_t2 = new_var_operand(t2);
        TranslateExp(ptr_child0, Sysmtable, operand_t1);
        tree_node* ptr_child2 = ptr->child_node[2]; 
        TranslateExp(ptr_child2, Sysmtable, operand_t2);
        if(place!=nullptr){
            InterCode* cur_code3 = new_binop_code(place, operand_t1, operand_t2); 
            switch (ptr_child1->node_type)
            {
            case ENUM_PLUS:
                cur_code3->kind = cur_code3->ADD;
                break;
            case ENUM_MINUS:
                cur_code3->kind = cur_code3->SUB;
                break;
            case ENUM_DIV:
                cur_code3->kind = cur_code3->DIV;
                break;
            case ENUM_STAR:
                cur_code3->kind = cur_code3->MUL;
                break;
            default:
                break;
            }
            
            append_code(cur_code3);
        }
        return;
    
    }else{  //todo logic calculation
        // TranslateCond(ptr, )
    }
    //for(int i = 0; i < child_nums; ++i){
    //    Translate(ptr->child_node[i], Sysmtable);
    //}

}


void TranslateStmt(tree_node* ptr,std::map<std::string, struct Sysmtable_item>& Sysmtable){

    tree_node* ptr_child0 = ptr->child_node[0]; 
    
    switch (ptr_child0->node_type)
    {
    case ENUM_Exp:{
        string t1 = new_temp();
        Operand* operand_t1 = new_var_operand(t1);
        TranslateExp(ptr_child0, Sysmtable, operand_t1);
        break;
    }
    case ENUM_CompSt:
        TranslateCompSt(ptr_child0, Sysmtable);
        break;
    case ENUM_RETURN:
// /*
    {   string t1 = new_temp();
        Operand* operand_t1 = new_var_operand(t1);
        tree_node* ptr_child1 = ptr->child_node[1]; 
//cout << ptr->child_num << endl; // 3
        TranslateExp(ptr_child1, Sysmtable, operand_t1);
        InterCode* cur_code = new_sinop_code(operand_t1);
        cur_code->kind = cur_code->RETURN;
        //return t1
        append_code(cur_code);
        break;
    }    
//*/
    case ENUM_IF:{  // IF LP Exp RP Stmt (ELSE Stmt)
        tree_node* ptr_child2 = ptr->child_node[2];  // Exp
        string label1 = new_label();
        string label2 = new_label();
        Operand* operand_lb1 = new_label_operand(label1);
        Operand* operand_lb2 = new_label_operand(label2);
        TranslateCond(ptr_child2, operand_lb1, operand_lb2, Sysmtable); // cur_code1
        InterCode* cur_code_lb1 = new_sinop_code(operand_lb1);
        cur_code_lb1->kind = cur_code_lb1->LABEL;
        append_code(cur_code_lb1);
        TranslateStmt(ptr->child_node[4], Sysmtable); // cur_code2
        InterCode* cur_code_lb2 = new_sinop_code(operand_lb2);
        cur_code_lb2->kind = cur_code_lb2->LABEL;
        if(ptr->child_num == 7){
            string label3 = new_label();
            Operand* operand_lb3 = new_label_operand(label3);
            InterCode* cur_code_goto_lb3 = new_sinop_code(operand_lb3);
            cur_code_goto_lb3->kind = cur_code_goto_lb3->GOTO;
            append_code(cur_code_goto_lb3);
            append_code(cur_code_lb2);
//cout << ptr->child_node[6]->child_num << endl;
            TranslateStmt(ptr->child_node[6], Sysmtable); // cur_code3
            InterCode* cur_code_lb3 = new_sinop_code(operand_lb3);
            cur_code_lb3->kind = cur_code_lb3->LABEL;
            append_code(cur_code_lb3);
        }else{
            append_code(cur_code_lb2);
        }
        break;
    }
    case ENUM_WHILE:{   // WHILE LP Exp RP Stmt
        tree_node* ptr_child2 = ptr->child_node[2];  // Exp
        string label1 = new_label();
        string label2 = new_label();
        string label3 = new_label();
        Operand* operand_lb1 = new_label_operand(label1);
        Operand* operand_lb2 = new_label_operand(label2);
        Operand* operand_lb3 = new_label_operand(label3);
        
        InterCode* cur_code_lb1 = new_sinop_code(operand_lb1);
        cur_code_lb1->kind = cur_code_lb1->LABEL;
        append_code(cur_code_lb1);

        TranslateCond(ptr_child2, operand_lb2, operand_lb3, Sysmtable); // cur_code1
        
        InterCode* cur_code_lb2 = new_sinop_code(operand_lb2);
        cur_code_lb2->kind = cur_code_lb2->LABEL;
        append_code(cur_code_lb2);

        TranslateStmt(ptr->child_node[4], Sysmtable); // cur_code2

        InterCode* cur_code_goto_lb1 = new_sinop_code(operand_lb1);
        cur_code_goto_lb1->kind = cur_code_goto_lb1->GOTO;
        append_code(cur_code_goto_lb1);

        InterCode* cur_code_lb3 = new_sinop_code(operand_lb3);
        cur_code_lb3->kind = cur_code_lb3->LABEL;
        append_code(cur_code_lb3);

        break;
    }
    default:
        int child_nums = ptr->child_num;
//        cout <<"child_nums : " << child_nums << endl;
        for(int i = 0; i < child_nums; ++i){
            Translate(ptr->child_node[i], Sysmtable);
        }
        break;
    }

}

void TranslateCond(tree_node* ptr, Operand* label_true,Operand* label_false,std::map<std::string, struct Sysmtable_item>& Sysmtable){
    tree_node* ptr_child0 = ptr->child_node[0]; 
    tree_node* ptr_child1 = ptr->child_node[1]; 
    if(ptr_child0->node_type == ENUM_NOT){ // NOT Exp
        TranslateCond(ptr_child1, label_false, label_true, Sysmtable);
        return;
    }
    string t1 = new_temp();
    Operand* operand_t1 = new_var_operand(t1);
    switch (ptr_child1->node_type)
    {
    case ENUM_RELOP:{
        TranslateExp(ptr_child0, Sysmtable, operand_t1);
        string t2 = new_temp();
        Operand* operand_t2 = new_var_operand(t2);
        tree_node* ptr_child2 = ptr->child_node[2]; 
        TranslateExp(ptr_child2, Sysmtable, operand_t2);
        string op_rel = ptr_child1->node_name;
        InterCode* cur_code3 = new_ifop_code(operand_t1, operand_t2,label_true,op_rel);
        append_code(cur_code3);

        InterCode* cur_code4 = new_sinop_code(label_false);
        cur_code4->kind = cur_code4->GOTO;
        append_code(cur_code4);
        break;
    }
    case ENUM_AND:{
        string label1 = new_label();
        Operand* operand_lb1 = new_label_operand(label1);
        //cur_code1
        TranslateCond(ptr_child0, operand_lb1, label_false, Sysmtable); //Exp1
        
        InterCode* cur_code_lb1 = new_sinop_code(operand_lb1);
        cur_code_lb1->kind = cur_code_lb1->LABEL;
        append_code(cur_code_lb1);
        //cur_code2
        tree_node* ptr_child2 = ptr->child_node[2]; //Exp2
        TranslateCond(ptr_child2, label_true, label_false, Sysmtable);
        
        break;
    }
    case ENUM_OR:{
        string label1 = new_label();
        Operand* operand_lb1 = new_label_operand(label1);
        //cur_code1
        TranslateCond(ptr_child0, label_true, operand_lb1, Sysmtable); //Exp1
        
        InterCode* cur_code_lb1 = new_sinop_code(operand_lb1);
        cur_code_lb1->kind = cur_code_lb1->LABEL;
        append_code(cur_code_lb1);
        //cur_code2
        tree_node* ptr_child2 = ptr->child_node[2]; //Exp2
        TranslateCond(ptr_child2, label_true, label_false, Sysmtable);
        
        break;
    }
    default: {// other case
        //cur_code1
        TranslateExp(ptr_child0, Sysmtable, operand_t1);
        //cur_code2
        InterCode* cur_code2 = new_ifop_code(operand_t1,new_constant_operand(0),label_true, "!=");
        append_code(cur_code2);

        InterCode* cur_code_goto_false = new_sinop_code(label_false);
        cur_code_goto_false->kind = cur_code_goto_false->GOTO;
        append_code(cur_code_goto_false);
        break;
        }
    }
}

void TranslateCompSt(tree_node* ptr,std::map<std::string, struct Sysmtable_item>& Sysmtable){
//todo
    // LC DefList StmtList RC
    if(ptr==nullptr || ptr->child_num < 4){
        return;
    }
    // todo ???
    TranslateDefList(ptr->child_node[1], Sysmtable);
//Translate(ptr->child_node[1], Sysmtable);
    TranslateStmtList(ptr->child_node[2], Sysmtable);
}

void TranslateStmtList(tree_node* ptr,std::map<std::string, struct Sysmtable_item>& Sysmtable){
    if(ptr==nullptr || ptr->child_num < 2){
        return;
    }
    // Stmt StmtList
    TranslateStmt(ptr->child_node[0], Sysmtable);
    TranslateStmtList(ptr->child_node[1], Sysmtable);
}

void TranslateDefList(tree_node* ptr,std::map<std::string, struct Sysmtable_item>& Sysmtable){
    if(ptr==nullptr || ptr->child_num < 2){
        return;
    }
    // Def DefList
    TranslateDef(ptr->child_node[0], Sysmtable);
    TranslateDefList(ptr->child_node[1], Sysmtable);
}

void TranslateDef(tree_node* ptr,std::map<std::string, struct Sysmtable_item>& Sysmtable){
    // Specifier DecList SEMI
    TranslateDecList(ptr->child_node[1], Sysmtable);
}

void TranslateDecList(tree_node* ptr,std::map<std::string, struct Sysmtable_item>& Sysmtable){
    TranslateDec(ptr->child_node[0], Sysmtable);
    if(ptr->child_num==1){
        return;
    }
    // Dec COMMA DecList
    TranslateDecList(ptr->child_node[2], Sysmtable);

}

void TranslateDec(tree_node* ptr,std::map<std::string, struct Sysmtable_item>& Sysmtable){
    
}

void TranslateFunDec(tree_node* ptr,std::map<std::string, struct Sysmtable_item>& Sysmtable){
    tree_node* ptr_child0 = ptr->child_node[0];
    Operand* operand_func = new_var_operand(ptr_child0->node_name);
    InterCode* cur_code_func = new_sinop_code(operand_func);
    cur_code_func->kind = cur_code_func->FUNCTION;
    append_code(cur_code_func);
//cout << " TranslateFunDec" << endl;
    if(ptr->child_num == 4){
        tree_node* ptr_child2 = ptr->child_node[2]; // VarList
        TranslateVarList(ptr_child2, Sysmtable); //PARAM  todo
    }
}

void TranslateVarList(tree_node* ptr,std::map<std::string, struct Sysmtable_item>& Sysmtable){
    tree_node* ptr_child0 = ptr->child_node[0];
//cout << " TranslateVarList" << endl;
    TranslateParamDec(ptr_child0, Sysmtable);
    if(ptr->child_num>1){
        tree_node* ptr_child2 = ptr->child_node[2];
        TranslateVarList(ptr_child2, Sysmtable);
    }
}

void TranslateParamDec(tree_node* ptr,std::map<std::string, struct Sysmtable_item>& Sysmtable){
    if(ptr==nullptr){
        return;
    }
//cout << " TranslateParamDec" << endl;

    //Specifier VarDec
    tree_node* ptr_child0 = ptr->child_node[0];
    tree_node* ptr_child1 = ptr->child_node[1];
    TranslateParamVarDec(ptr_child1, Sysmtable);
    
}

void TranslateParamVarDec(tree_node* ptr,std::map<std::string, struct Sysmtable_item>& Sysmtable){
    if(ptr==nullptr){
        return;
    }

    tree_node* ptr_child0 = ptr->child_node[0];
//cout << " TranslateVarDec" << endl;
//  /*  if(ptr->child_num==1){
        Operand* operand_param = new_var_operand(ptr_child0->node_name);
        InterCode* cur_code_param = new_sinop_code(operand_param);
        cur_code_param->kind = cur_code_param->PARAM;
        append_code(cur_code_param);
//    }*/
    //todo
    // VarDec LB INT RB


}
void TranslateVarDec(tree_node* ptr,std::map<std::string, struct Sysmtable_item>& Sysmtable){
    if(ptr==nullptr){
        return;
    }
    if(ptr->child_num!=4){
        return;
    }
    tree_node* ptr_child0 = ptr->child_node[0];
    if(ptr_child0->child_num!=1){
        return; // multiple dimensional
    }

    tree_node* ptr_ID = ptr_child0->child_node[0];
    tree_node* ptr_INT = ptr->child_node[2];
    Operand* operand_t1 = new_var_operand(ptr_ID->node_name);
    InterCode* cur_code = new_dec_code(operand_t1 , ptr_INT->int_val*4);
    cur_code->kind = cur_code->DEC;
    append_code(cur_code);
    // 22222222
}
void TranslateArgs(tree_node* ptr,std::map<std::string, struct Sysmtable_item>& Sysmtable,list<Operand*>& arg_list){
    // Exp   or   Exp COMMA Args
    string t1 = new_temp();
    Operand* operand_t1 = new_var_operand(t1);
    TranslateExp(ptr->child_node[0],Sysmtable,operand_t1);
    arg_list.push_front(operand_t1);
    if(ptr->child_num==1){
        return ;
    }
    TranslateArgs(ptr->child_node[2],Sysmtable,arg_list);
}