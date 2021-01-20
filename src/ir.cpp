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

void printOperand(std::ofstream& outputfile, Operand* operand){
    switch (operand->kind)
    {
    case operand->VARIABLE:
        outputfile << operand->u.value;
        break;
    case operand->CONSTANT:
        outputfile << "#" << operand->u.val_no; //todo  float?
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
        case interCode->SUB:{
            auto stru_binop = interCode->u.binop;
            printOperand(outputfile, stru_binop.result);
            outputfile << " := " ;
            printOperand(outputfile, stru_binop.op1);
            outputfile << " - " ;
            printOperand(outputfile, stru_binop.op2);
            outputfile << endl;
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
    if(child_nums == 1){
        InterCode* cur_code= (InterCode*) malloc(sizeof(InterCode));
        //todo
        ptr = ptr->child_node[0];
        if(ptr->node_type==ENUM_INT){

            if(place!=nullptr){
                cur_code->kind = cur_code->ASSIGN;
                cur_code->u.assign.left = place;

                Operand* r_operand = (Operand*)malloc(sizeof(Operand));
                r_operand->kind = r_operand->CONSTANT;
                r_operand->u.val_no = ptr->int_val; //todo float?
                cur_code->u.assign.right = r_operand;
                InterCodes.push_back(cur_code);
            }
        }else if(ptr->node_type==ENUM_ID){
//cout <<  "Sysmtable.find(ptr->node_name);" << ptr->node_name <<endl;       
            auto it = Sysmtable.find(ptr->node_name);
            if(place!=nullptr){
                cur_code->kind = cur_code->ASSIGN;
                cur_code->u.assign.left = place;
//cout << place->u.value << endl;
                Operand* r_operand = new_var_operand(it->second.name);
                cur_code->u.assign.right = r_operand;
                InterCodes.push_back(cur_code);
            }
        }
        
        return;
    }
    
    tree_node* ptr_child0 = ptr->child_node[0]; 
    tree_node* ptr_child1 = ptr->child_node[1]; 
    if(ptr_child0->node_type== ENUM_MINUS){     //MINUS Exp1
        // ptr_child1 Exp1
        string t1 = new_temp();
//cout << "t1 : " << t1 << endl;
        Operand* operand_t1 = new_var_operand(t1);
        TranslateExp(ptr_child1,Sysmtable,operand_t1); //cur_code1
        if(place!=nullptr){
            InterCode* cur_code2= (InterCode*) malloc(sizeof(InterCode));
            cur_code2->kind = cur_code2->SUB;
            cur_code2->u.binop.result = place;
            cur_code2->u.binop.op1 = new_constant_operand(0);
            cur_code2->u.binop.op2 = operand_t1;
            InterCodes.push_back(cur_code2);
        }
        return;
    }
    if(ptr_child1->node_type== ENUM_ASSIGNOP){  //Exp1 ASSIGNOP Exp2
        // ptr_child0 Exp1
        auto it = Sysmtable.find(ptr_child0->child_node[0]->node_name); // Exp1 -> ID get ID name
//cout << "ptr_child0->child_node[0]->node_name " << ptr_child0->child_node[0]->node_name << endl;
        string t1 = new_temp();
        Operand* operand_t1 = new_var_operand(t1);
        TranslateExp(ptr->child_node[2],Sysmtable,operand_t1);
        InterCode* cur_code1= (InterCode*) malloc(sizeof(InterCode));
        InterCode* cur_code2= (InterCode*) malloc(sizeof(InterCode));

//cout << it->second.name << "_it->second.name" << endl;
        Operand* operand_var = new_var_operand(it->second.name); 
  
        cur_code1->kind = cur_code1->ASSIGN;
        cur_code1->u.assign.left = operand_var;
        cur_code1->u.assign.right = operand_t1;
        InterCodes.push_back(cur_code1);
        
        if(place!=nullptr){
            cur_code2->kind = cur_code1->ASSIGN;
            cur_code2->u.assign.left = place;
            cur_code2->u.assign.right = operand_var;
            InterCodes.push_back(cur_code2);
        }

        return;
    }
    //for(int i = 0; i < child_nums; ++i){
    //    Translate(ptr->child_node[i], Sysmtable);
    //}
}

void append_code(CODE_TYPE type,...){
    //initializer_list?
}
