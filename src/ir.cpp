#include "ir.h"
#include "tree.h"
#include "intercode.h"
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
        case interCode->RETURN:{
            auto stru_sinop = interCode->u.sinop;
            outputfile << "RETURN ";
            printOperand(outputfile, stru_sinop.op);
            outputfile << endl;
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
        default:
            break;
        }
    }
}

void Translate(tree_node* ptr,std::map<std::string, struct Sysmtable_item>& Sysmtable){
    if(ptr==nullptr){
        return;
    }


    NODE_TYPE cur_type = ptr->node_type;
    switch (cur_type)
    {
    case ENUM_Exp:
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
        ptr = ptr->child_node[0];
        if(ptr->node_type==ENUM_INT){
            if(place!=nullptr){
                Operand* r_operand = new_constant_operand(ptr->int_val);
                cur_code = new_assign_code(place, r_operand);
                append_code(cur_code);
            }
        }else if(ptr->node_type==ENUM_ID){     
            auto it = Sysmtable.find(ptr->node_name);
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

    if(ptr_child0->node_type== ENUM_MINUS){     //MINUS Exp1
        // ptr_child1 Exp1
        string t1 = new_temp();
//cout << "t1 : " << t1 << endl;
        Operand* operand_t1 = new_var_operand(t1);
        TranslateExp(ptr_child1,Sysmtable,operand_t1); //cur_code1
        if(place!=nullptr){
            InterCode* cur_code2= new_binop_code(place, new_constant_operand(0), operand_t1 );
            cur_code2->kind = cur_code2->SUB;
            append_code(cur_code2);
        }
        return;
    }else if(ptr_child1->node_type== ENUM_ASSIGNOP){  //Exp1 ASSIGNOP Exp2
        // ptr_child0 Exp1
        auto it = Sysmtable.find(ptr_child0->child_node[0]->node_name); // Exp1 -> ID get ID name
//cout << "ptr_child0->child_node[0]->node_name " << ptr_child0->child_node[0]->node_name << endl;
        string t1 = new_temp();
        Operand* operand_t1 = new_var_operand(t1);
        TranslateExp(ptr->child_node[2],Sysmtable,operand_t1);
        Operand* operand_var = new_var_operand(it->second.name); 
        InterCode* cur_code1= new_assign_code(operand_var, operand_t1);
        append_code(cur_code1);
        if(place!=nullptr){
            InterCode* cur_code2= new_assign_code(place, operand_var);
            append_code(cur_code2);
        }
        return;
    }else if(ptr_child1->node_type== ENUM_PLUS){    //Exp1 PLUS Exp2
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
            cur_code3->kind = cur_code3->ADD;
            append_code(cur_code3);
        }
        return;
    }else{  //todo logic calculation

    }
    //for(int i = 0; i < child_nums; ++i){
    //    Translate(ptr->child_node[i], Sysmtable);
    //}

}


void TranslateStmt(tree_node* ptr,std::map<std::string, struct Sysmtable_item>& Sysmtable){

    tree_node* ptr_child0 = ptr->child_node[0]; 
    
//cout << "ok " << ptr->child_num <<endl;
    switch (ptr_child0->node_type)
    {
    case ENUM_Exp:
        TranslateExp(ptr_child0, Sysmtable, nullptr);
        break;
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
    TranslateDefList(ptr->child_node[1], Sysmtable);
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
    
}

void TranslateDecList(tree_node* ptr,std::map<std::string, struct Sysmtable_item>& Sysmtable){

}

void TranslateDec(tree_node* ptr,std::map<std::string, struct Sysmtable_item>& Sysmtable){
    
}

void TranslateFunDec(tree_node* ptr,std::map<std::string, struct Sysmtable_item>& Sysmtable){
    tree_node* ptr_child0 = ptr->child_node[0];
    Operand* operand_func = new_var_operand(ptr_child0->node_name);
    InterCode* cur_code_func = new_sinop_code(operand_func);
    cur_code_func->kind = cur_code_func->FUNCTION;
    append_code(cur_code_func);
cout << " TranslateFunDec" << endl;
    if(ptr->child_num == 4){
        tree_node* ptr_child2 = ptr->child_node[2]; // VarList
        TranslateVarList(ptr_child2, Sysmtable); //PARAM  todo
    }
}

void TranslateVarList(tree_node* ptr,std::map<std::string, struct Sysmtable_item>& Sysmtable){
    tree_node* ptr_child0 = ptr->child_node[0];
cout << " TranslateVarList" << endl;
    TranslateParamDec(ptr_child0, Sysmtable);
    if(ptr_child0->child_num>1){
        tree_node* ptr_child2 = ptr->child_node[2];
        TranslateVarList(ptr_child2, Sysmtable);
    }
}

void TranslateParamDec(tree_node* ptr,std::map<std::string, struct Sysmtable_item>& Sysmtable){
    if(ptr==nullptr){
        return;
    }
cout << " TranslateParamDec" << endl;

    //Specifier VarDec
    tree_node* ptr_child0 = ptr->child_node[0];
    tree_node* ptr_child1 = ptr->child_node[1];
    TranslateVarDec(ptr_child1, Sysmtable);
    
}

void TranslateVarDec(tree_node* ptr,std::map<std::string, struct Sysmtable_item>& Sysmtable){
    if(ptr==nullptr){
        return;
    }

    tree_node* ptr_child0 = ptr->child_node[0];
cout << " TranslateVarDec" << endl;
/*    if(ptr->child_num==1){
        Operand* operand_param = new_var_operand(ptr_child0->node_name);
        InterCode* cur_code_param = new_sinop_code(operand_param);
        cur_code_param->kind = cur_code_param->PARAM;
        append_code(cur_code_param);
    }*/
    //todo
    // VarDec LB INT RB


}