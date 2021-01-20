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

void Translate(tree_node* ptr,std::map<std::string, struct Sysmtable_item>& Sysmtable){
    if(ptr==nullptr){
        return;
    }

    NODE_TYPE cur_type = ptr->node_type;
    switch (cur_type)
    {
    case ENUM_Exp:
        TranslateExp(ptr, Sysmtable, "");
        break;
    //todo
    default:
        int child_nums = ptr->child_num;
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
    int child_nums = ptr->child_num;
    for(int i = 0; i < child_nums; ++i){
        Translate(ptr->child_node[i], Sysmtable);
    }
    for(InterCode* interCode: InterCodes){
        if(!interCode->code.empty()){
            outputfile << interCode->code << endl;
        }
       
    }
}

void TranslateExp(tree_node* ptr,std::map<std::string, struct Sysmtable_item>& Sysmtable,string place){
    //todo
    //outputfile << "TranslateProgram\n" ;
    if(ptr==nullptr){
        return;
    }
    int child_nums = ptr->child_num;
    if(child_nums == 1){
        InterCode* cur_code= new InterCode();
        //cur_code->code = nullptr;
        //todo
        if(ptr->node_type==ENUM_INT){
            if(!place.empty())
                cur_code->code = place+" := #"+to_string(ptr->int_val);
        }else {
            auto it = Sysmtable.find(ptr->node_name);
            if(!place.empty())
                cur_code->code = place+" := "+ it->second.name;
        }
        InterCodes.push_back(cur_code);
        return;
    }
    //Exp1 ASSIGNOP Exp2
    if(ptr->child_node[1]->node_type== ENUM_ASSIGNOP){
        auto it = Sysmtable.find(ptr->node_name);
        string t1 = new_temp();
        TranslateExp(ptr->child_node[2],Sysmtable,t1);
        InterCode* cur_code1= new InterCode();
        InterCode* cur_code2= new InterCode();
        cur_code1->code = it->second.name + " := "+ t1;
        if(!place.empty())
            cur_code2->code =  place+" := "+ it->second.name;
        InterCodes.push_back(cur_code1);
        InterCodes.push_back(cur_code2);
        return;
    }
    //for(int i = 0; i < child_nums; ++i){
    //    Translate(ptr->child_node[i], Sysmtable);
    //}


}

void append_code(CODE_TYPE type,...){
    //initializer_list?
}
