#include "semantics.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<string>
#include<map>
#include <assert.h>
#include<iostream>

void AnalasysForProgram(tree_node* ptr){
    if(ptr==nullptr){
        return;
    }
    tree_node* ExtDefList_ = ptr->child_node[0];
    AnalasysForExtDefList(ExtDefList_);
}

void AnalasysForExtDefList(tree_node* ptr){
//ExtDefList : ExtDef ExtDefList 
    if(ptr==nullptr)
        return;
    //assert(1 - strcmp(ptr->name,"ExtDefList") ); //"wrong at AnalasysBegins"

    tree_node* extDef_ = ptr->child_node[0];
    tree_node* extDefList_ = ptr->child_node[1];
    
    AnalasysForExtDef(extDef_);
    AnalasysForExtDefList(extDefList_);
}

void AnalasysForExtDef(tree_node* ptr){
/*
ExtDef : Specifier ExtDecList SEMI 
    | Specifier SEMI
    | Specifier FunDec CompSt 
*/
    if(ptr==nullptr)
        return;
    if (ptr->child_num==2)
    {
        tree_node*  Specifier_ = ptr->child_node[0];
        tree_node*  SEMI_ = ptr->child_node[1];
        AnalasysForSpecifier(Specifier_);
        AnalasysForSEMI(SEMI_);
    }else if(ptr->child_num==3)
    {
        tree_node*  Specifier_ = ptr->child_node[0];
        tree_node*  second_node = ptr->child_node[1];
        AnalasysForSpecifier(Specifier_);
//debug 
//std::cout << "ExtDef second_node->node_type "  << second_node->node_type<< std::endl;

        if(second_node->node_type==ENUM_ExtDecList){
            //debug 
            //std::cout << "ENUM_ExtDecList" << std::endl;

            AnalasysForExtDecList(second_node);
            tree_node*  SEMI_ = ptr->child_node[2];
            AnalasysForSEMI(SEMI_);
        }else if(second_node->node_type==ENUM_FunDec){ //ENUM_FunDec
            //debug 
            //std::cout << "ENUM_FunDec" << std::endl;

            AnalasysForFunDec(second_node);
            tree_node*  CompSt_ = ptr->child_node[2];
            AnalasysForCompSt(CompSt_);
        }
    }
    
}


void AnalasysForExtDecList(tree_node* ptr){
    
    if(ptr==nullptr)
        return;
    if (ptr->child_num == 1 )
    {
//debug 
//std::cout << "ENUM_VarDec 1 " << std::endl;

        tree_node*  VarDec_ = ptr->child_node[0];
        AnalasysForVarDec(VarDec_);
    }else if (ptr->child_num == 3 )
    {
        tree_node*  VarDec_ = ptr->child_node[0];
        tree_node*  COMMA_ = ptr->child_node[1];
        tree_node*  ExtDecList_ = ptr->child_node[2];
//debug 
//std::cout << "ENUM_VarDec 3 " << std::endl;

        AnalasysForVarDec(VarDec_);
        AnalasysForExtDecList(ExtDecList_);
    }
    
}

void AnalasysForSpecifier(tree_node* ptr){
/*
Specifier : TYPE 
    | StructSpecifier
*/
    if(ptr==nullptr)
        return;

//debug
//std::cout <<"AnalasysForSpecifier node_name"<< ptr->child_node[0]->node_name << std::endl;
//std::cout <<"AnalasysForSpecifier node_name"<< ptr->child_node[0]->node_type << std::endl;

    if(ptr->child_node[0]->node_type == ENUM_TYPE){
        global_type_ptr->kind = global_type_ptr->BASIC;

        if(strcmp(ptr->child_node[0]->node_name,"int")==0){
            global_type_ptr->u.basic = 0;
        }else{//  if(strcmp(ptr->node_name,"float")==0)
            global_type_ptr->u.basic = 1;
        }
        //AnalasysForVarDec(ptr->child_node[0]);
    }else{//  if(ptr->child_node[0]->node_type  == ENUM_StructSpecifier)
        global_type_ptr->kind = global_type_ptr->STRUCTURE;
        //todo
        AnalasysForStructSpecifier(ptr->child_node[0]);
    }
    

}

void AnalasysForStructSpecifier(tree_node* ptr){
/*
StructSpecifier : STRUCT OptTag LC DefList RC 
    | STRUCT Tag 
*/
    if(ptr==nullptr)
        return;

        //to do from here
    tree_node*  STRUCT_ = ptr->child_node[0];
    global_type_ptr->kind = global_type_ptr->STRUCTURE;
    
    if(ptr->child_num == 2 ){
        tree_node*  Tag_ = ptr->child_node[1];
//debug
//std::cout << Tag_->node_name << std::endl;
        // AnalasysForID(); 
        tree_node*  ID_ = Tag_->child_node[0];
//debug
//std::cout << ID_->node_name << std::endl;
        if(Sysmtable.find(ID_->node_name)==Sysmtable.end() ){
            fprintf(stderr,"Error type 17 at Line %d: %s %s.\n",ID_->line_no,"Undifined structure",ID_->node_name);
        }else{
            Sysmtable_item cur_item;
            cur_item.kind = cur_item.VARIABLE;
            cur_item.name = ID_->node_name;
            cur_item.row = ID_->line_no;
            cur_item.type = global_type_ptr;
            Sysmtable.insert(std::pair<std::string,Sysmtable_item>(cur_item.name,cur_item));

        }
    }else{
        //OptTag DefList 1 3
        tree_node*  OptTag_ = ptr->child_node[1];
        tree_node*  DefList_ = ptr->child_node[3];
        if(OptTag_->child_node[0]!=nullptr){
            tree_node*  ID_ = OptTag_->child_node[0];

            if(Sysmtable.find(ID_->node_name)!=Sysmtable.end() ){
                fprintf(stderr,"Error type 16 at Line %d: %s %s.\n",ID_->line_no,"Duplicated name",ID_->node_name);
            }else{
                Sysmtable_item cur_item;
                cur_item.kind = cur_item.VARIABLE;
                cur_item.name = ID_->node_name;
                cur_item.row = ID_->line_no;
                cur_item.type = global_type_ptr;
                Sysmtable.insert(std::pair<std::string,Sysmtable_item>(cur_item.name,cur_item));

            } 
        }
        AnalasysForDefList(DefList_);

    }
    /*
    switch (ptr->child_num)
    {
    case :
        tree_node*  _ = ptr->child_node[0];
        tree_node*  _ = ptr->child_node[1];
        AnalasysFor();
        AnalasysFor();
        break;
    
    default:
        break;
    }   
    */

}

void AnalasysForCompSt(tree_node* ptr){
    // CompSt : LC DefList StmtList RC 
    if(ptr==nullptr)
        return;

    tree_node*  DefList_ = ptr->child_node[1];
    tree_node*  StmtList_ = ptr->child_node[2];
    AnalasysForDefList(DefList_);
    AnalasysForStmtList(StmtList_);

}

void AnalasysForDefList(tree_node* ptr){
    /*
DefList : Def DefList 
    | // Epsl
    */
    if(ptr==nullptr)
        return;

    tree_node*  Def_ = ptr->child_node[0];
    tree_node*  DefList_ = ptr->child_node[1];
    AnalasysForDef(Def_);
    AnalasysForDefList(DefList_);


}


void AnalasysForStmtList(tree_node* ptr){
    /*
StmtList : Stmt StmtList 
    | // Epsl
    */
    if(ptr==nullptr)
        return;

    tree_node*  Stmt_ = ptr->child_node[0];
    tree_node*  StmtList_ = ptr->child_node[1];
    AnalasysForStmt(Stmt_);
    AnalasysForStmtList(StmtList_);

}

void AnalasysForDef(tree_node* ptr){
    /*
Def : Specifier DecList SEMI 
    ;
    */
    if(ptr==nullptr)
        return;

    tree_node*  Specifier_ = ptr->child_node[0];
    tree_node*  DecList_ = ptr->child_node[1];
    AnalasysForSpecifier(Specifier_);
    AnalasysForDecList(DecList_);

}

void AnalasysForDecList(tree_node* ptr){
    /*
DecList : Dec 
    | Dec COMMA DecList
    */
    if(ptr==nullptr)
        return;

    tree_node*  Dec_ = ptr->child_node[0];
    AnalasysForDec(Dec_);
    if(ptr->child_num!=1){
        tree_node*  DecList_ = ptr->child_node[2];
        AnalasysForDecList(DecList_);
    }

}

void AnalasysForDec(tree_node* ptr){
    /*
Dec : VarDec 
    | VarDec ASSIGNOP Exp 
    ;
    */
    if(ptr==nullptr)
        return;

    tree_node*  VarDec_ = ptr->child_node[0];
    AnalasysForVarDec(VarDec_);
    if(ptr->child_num == 3){
        tree_node*  Exp_ = ptr->child_node[2];
        AnalasysForExp(Exp_);
    }
    //todo ASSIGN

}

void AnalasysForStmt(tree_node* ptr){
/*
Stmt : Exp SEMI 
    | CompSt 
    | RETURN Exp SEMI 
    | IF LP Exp RP Stmt %prec LOWER_THAN_ELSE 
    | IF LP Exp RP Stmt ELSE Stmt 
    | WHILE LP Exp RP Stmt

*/
    if(ptr==nullptr)
        return;
    if (ptr->child_num == 2 ){
        tree_node*  Exp_ = ptr->child_node[0];
        tree_node*  SEMI_ = ptr->child_node[1];
        AnalasysForExp(Exp_);
        AnalasysForSEMI(SEMI_);
    }else if(ptr->child_num == 1 ){
        tree_node*  CompSt_ = ptr->child_node[0];
        AnalasysForCompSt(CompSt_);

    }else if(ptr->child_num == 3 ){

    }else if(ptr->child_num == 5 ){
        //todo

    }else if(ptr->child_num == 7 ){

    }

}


void AnalasysForVarDec(tree_node* ptr){
/*
VarDec : ID 
    | VarDec LB INT RB
*/
    if(ptr==nullptr)
        return;
    if(ptr->child_num==1){
        tree_node* ID_ = ptr->child_node[0];
        //AnalasysForID(ID_);
        //todo
        //std::map<std::string, struct Sysmtable_item> Sysmtable;
        std::string name = ID_->node_name;
        Sysmtable_item cur_item;
        cur_item.kind = cur_item.VARIABLE;
        cur_item.name = ID_->node_name;
        cur_item.row = ID_->line_no;
        cur_item.type = global_type_ptr;
        if(Sysmtable.find(name)!=Sysmtable.end() ){
            fprintf(stderr,"Error type 3 at Line %d: %s %s.\n",ID_->line_no,"Redifined variable",ID_->node_name);
        }else{
            Sysmtable.insert(std::pair<std::string,Sysmtable_item>(name,cur_item));

        }
        //std::cout << ptr->node_name << std::endl;
        //std::cout << ptr->node_type << std::endl;
    }else{
        tree_node* VarDec_ = ptr->child_node[0];
        tree_node* INT_ = ptr->child_node[2];
         //todo
         AnalasysForVarDec(VarDec_);
         // change into ARRAY
         //Type_ temp = global_type_ptr->kind;
         //global_type_ptr->u.array.elem = global_type_ptr->kind;
         //global_type_ptr->kind = global_type_ptr->ARRAY;
    }

    
}

//todo
void AnalasysForFunDec(tree_node* ptr){
    /*
    FunDec : ID LP VarList RP
    | ID LP RP
    */
    if(ptr==nullptr)
        return;
    //global_type_ptr->kind = global_type_ptr->STRUCTURE;
    //global_type_ptr->u.basic = 0;
    tree_node* ID_ = ptr->child_node[0];
    std::string name = ID_->node_name;
    Sysmtable_item cur_item;
    cur_item.kind = cur_item.FUNCTION;
    cur_item.name = ID_->node_name;
    cur_item.row = ID_->line_no;
    cur_item.type = global_type_ptr;
    if(Sysmtable.find(name)!=Sysmtable.end() ){
        fprintf(stderr,"Error type 4 at Line %d: %s %s.\n",ID_->line_no,"Redifined variable",name);
    }else{
        //Sysmtable.insert(std::pair<std::string,Sysmtable_item>(name,cur_item));
    }
    if(ptr->child_num==4){
        tree_node*  VarList_ = ptr->child_node[2];
        AnalasysForVarList(VarList_);

    }else{
        // ==3
        // AnalasysForID(); //todo TODO

    }

}

void AnalasysForSEMI(tree_node* ptr){
    
    if(ptr==nullptr)
        return;
    if(ptr->child_num)
        ;
}



void AnalasysForVarList(tree_node* ptr){
    /*
VarList : ParamDec COMMA VarList 
    | ParamDec 
    */
    if(ptr==nullptr)
        return;
    tree_node*  ParamDec_ = ptr->child_node[0];
    AnalasysForParamDec(ParamDec_);
    if (ptr->child_num == 3 ){
        tree_node*  VarList_ = ptr->child_node[2];
        AnalasysForVarList(VarList_);
    }else if(ptr->child_num == 1 ){
        ;
    }
}

void AnalasysForParamDec(tree_node* ptr){
/*
ParamDec : Specifier VarDec
*/
    if(ptr==nullptr)
        return;
    tree_node*  Specifier_ = ptr->child_node[0];
    tree_node*  VarDec_ = ptr->child_node[1];
    AnalasysForSpecifier(Specifier_);
    AnalasysForVarDec(VarDec_);

}

void AnalasysForExp(tree_node* ptr){
/*
Exp : Exp ASSIGNOP Exp 
    | Exp AND Exp 
    | Exp OR Exp 
    | Exp RELOP Exp 
    | Exp PLUS Exp 
    | Exp MINUS Exp 
    | Exp STAR Exp 
    | Exp DIV Exp 
    | LP Exp RP 
    | MINUS Exp %prec UMINUS 
    | NOT Exp 
    | ID LP Args RP 
    | ID LP RP 
    | Exp LB Exp RB 
    | Exp DOT ID 
    | ID
    | INT
    | FLOAT 
*/
    if(ptr==nullptr)
        return;
    if (ptr->child_num == 1 ){
        if(ptr->child_node[0]->node_type==ENUM_ID){
            tree_node* ID_ = ptr->child_node[0];
            if(Sysmtable.find(ID_->node_name) == Sysmtable.end() ){
                 fprintf(stderr,"Error type 1 at Line %d: %s %s.\n",ID_->line_no,"Undifined variable",ID_->node_name);
            }else{
                //todo
                //1 Sysmtable.insert(std::pair<std::string,Sysmtable_item>(name,cur_item));

            }
            
        }else if(ptr->child_node[0]->node_type==ENUM_INT){

        }else if(ptr->child_node[0]->node_type==ENUM_FLOAT){

        }

    }else if(ptr->child_node[0]->node_type==ENUM_ID){
        // function call
        tree_node* ID_ = ptr->child_node[0];
        if(Sysmtable.find(ID_->node_name) == Sysmtable.end() ){
                fprintf(stderr,"Error type 2 at Line %d: %s %s.\n",ID_->line_no,"Undifined function",ID_->node_name);
        }else{
            //todo
            //1 Sysmtable.insert(std::pair<std::string,Sysmtable_item>(name,cur_item));

        }

    }else if(ptr->child_num == 3){

    }

//        tree_node*  _ = ptr->child_node[0];
//        tree_node*  _ = ptr->child_node[1];
//        AnalasysFor();
 //       AnalasysFor();
}
/*
void AnalasysFor(tree_node* ptr){
    
    if(ptr==nullptr)
        return;
    if (ptr->child_num ==  ){
        tree_node*  _ = ptr->child_node[0];
        tree_node*  _ = ptr->child_node[1];
        AnalasysFor();
        AnalasysFor();
    }else if(ptr->child_num ==  ){

    }

}

*/
