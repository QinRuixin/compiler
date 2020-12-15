#include "semantics.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<string>
#include<map>
#include <assert.h>
#include<iostream>


Type global_type_ptr;
std::map<std::string, struct Sysmtable_item> Sysmtable;


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
/* 全局变量 结构体 函数
ExtDef : Specifier ExtDecList SEMI 
    | Specifier SEMI
    | Specifier FunDec CompSt 
*/
    if(ptr==nullptr)
        return;
    
    tree_node*  Specifier_ = ptr->child_node[0];
    Type specifier_type = AnalasysForSpecifier(Specifier_);
    if (ptr->child_num==2)
    {
        tree_node*  SEMI_ = ptr->child_node[1];
        //AnalasysForSEMI(SEMI_);
    }else if(ptr->child_num==3)
    {
        tree_node*  second_node = ptr->child_node[1];
//debug 
//std::cout << "ExtDef second_node->node_type "  << second_node->node_type<< std::endl;

        if(second_node->node_type==ENUM_ExtDecList){
            //debug 
            //std::cout << "ENUM_ExtDecList" << std::endl;

            AnalasysForExtDecList(second_node, specifier_type);
            tree_node*  SEMI_ = ptr->child_node[2];
            AnalasysForSEMI(SEMI_);
        }else if(second_node->node_type==ENUM_FunDec){ //ENUM_FunDec
            //debug 
            //std::cout << "ENUM_FunDec" << std::endl;

            AnalasysForFunDec(second_node);
            tree_node*  CompSt_ = ptr->child_node[2];
            AnalasysForCompSt(CompSt_, specifier_type);
        }
    }
    
}


void AnalasysForExtDecList(tree_node* ptr, Type type){
    
    if(ptr==nullptr)
        return;
    if (ptr->child_num == 1 )
    {
//debug 
//std::cout << "ENUM_VarDec 1 " << std::endl;

        tree_node*  VarDec_ = ptr->child_node[0];
        AnalasysForVarDec(VarDec_, type);
    }else if (ptr->child_num == 3 )
    {
        tree_node*  VarDec_ = ptr->child_node[0];
        tree_node*  COMMA_ = ptr->child_node[1];
        tree_node*  ExtDecList_ = ptr->child_node[2];
//debug 
//std::cout << "ENUM_VarDec 3 " << std::endl;

        AnalasysForVarDec(VarDec_, type);
        AnalasysForExtDecList(ExtDecList_, type);
    }
    
}

Type AnalasysForSpecifier(tree_node* ptr){
/*
Specifier : TYPE 
    | StructSpecifier
*/
    if(ptr==nullptr)
        return nullptr;

//debug
//std::cout <<"AnalasysForSpecifier node_name"<< ptr->child_node[0]->node_name << std::endl;
//std::cout <<"AnalasysForSpecifier node_name"<< ptr->child_node[0]->node_type << std::endl;

    Type res = new Type_();
    if(ptr->child_node[0]->node_type == ENUM_TYPE){
        res->kind = res->BASIC;

        global_type_ptr->kind = global_type_ptr->BASIC;

        if(strcmp(ptr->child_node[0]->node_name,"int")==0){
            res->u.basic = BASIC_INT;

            global_type_ptr->u.basic = BASIC_INT;
        }else{//  if(strcmp(ptr->node_name,"float")==0)
            res->u.basic = BASIC_FLOAT;

            global_type_ptr->u.basic = BASIC_FLOAT;
        }
        //AnalasysForVarDec(ptr->child_node[0]);
    }else{//  if(ptr->child_node[0]->node_type  == ENUM_StructSpecifier)
        res->kind = res->STRUCTURE;
        global_type_ptr->kind = global_type_ptr->STRUCTURE;
        //todo
        res = AnalasysForStructSpecifier(ptr->child_node[0]);
    }
    return res;

}

Type AnalasysForStructSpecifier(tree_node* ptr){
/*
StructSpecifier : STRUCT OptTag LC DefList RC 
    | STRUCT Tag 
*/
    if(ptr==nullptr)
        return nullptr;
    Type res = new Type_();
        //to do from here
    tree_node*  STRUCT_ = ptr->child_node[0];
    global_type_ptr->kind = global_type_ptr->STRUCTURE;
    res->kind = res->STRUCTURE;
    if(ptr->child_num == 2 ){
        tree_node*  Tag_ = ptr->child_node[1];
//debug
//std::cout << Tag_->node_name << std::endl;
        // AnalasysForID(); 
        tree_node*  ID_ = Tag_->child_node[0];
//debug
//std::cout << ID_->node_name << std::endl;
        auto it = Sysmtable.find(ID_->node_name);
        if(it ==Sysmtable.end() || it->second.type->kind != it->second.type->STRUCTURE){
            fprintf(stderr,"Error type 17 at Line %d: %s %s.\n",ID_->line_no,"Undifined structure",ID_->node_name);
        }else{
            Sysmtable_item item = it->second;
            res = item.type;
        /*
            Sysmtable_item cur_item;
            cur_item.kind = cur_item.VARIABLE;
            cur_item.name = ID_->node_name;
            cur_item.row = ID_->line_no;
            cur_item.type = global_type_ptr;
            Sysmtable.insert(std::pair<std::string,Sysmtable_item>(cur_item.name,cur_item));
        */

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
            //    FieldList fieldList_stru = new FieldList_();
            //todo 
            //    fieldList_stru->name = ID_->node_name;
            //    fieldList_stru->type = fieldList_stru;
            //    fieldList_stru->tail = nullptr;
                // from Struct
                res->u.structure = AnalasysForDefList(DefList_);

                Sysmtable_item cur_item;
                cur_item.kind = cur_item.CONST;
                cur_item.name = ID_->node_name;
                cur_item.row = ID_->line_no;
                cur_item.type = res;
                
                Sysmtable.insert(std::pair<std::string,Sysmtable_item>(cur_item.name,cur_item));

            } 
        }
        
    }
    return res;

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

void AnalasysForCompSt(tree_node* ptr,Type returnType){
    // CompSt : LC DefList StmtList RC 
    if(ptr==nullptr)
        return;

    tree_node*  DefList_ = ptr->child_node[1];
    tree_node*  StmtList_ = ptr->child_node[2];
    // from {LC  RC}
    AnalasysForDefList(DefList_);
    AnalasysForStmtList(StmtList_, returnType);
}

FieldList AnalasysForDefList(tree_node* ptr){
    /*
DefList : Def DefList 
    | // Epsl
    */
    if(ptr==nullptr)
        return nullptr;
    
    tree_node*  Def_ = ptr->child_node[0];
    tree_node*  DefList_ = ptr->child_node[1];
    FieldList res = AnalasysForDef(Def_);
    res->tail =  AnalasysForDefList(DefList_);
    return res;

}


void AnalasysForStmtList(tree_node* ptr,Type returnType){
    /*
StmtList : Stmt StmtList 
    | // Epsl
    */
    if(ptr==nullptr)
        return;

    tree_node*  Stmt_ = ptr->child_node[0];
    tree_node*  StmtList_ = ptr->child_node[1];
    AnalasysForStmt(Stmt_,returnType);
    AnalasysForStmtList(StmtList_,returnType);

}

FieldList AnalasysForDef(tree_node* ptr){
    /*
Def : Specifier DecList SEMI 
    ;
    */
    if(ptr==nullptr)
        return nullptr;

    tree_node*  Specifier_ = ptr->child_node[0];
    tree_node*  DecList_ = ptr->child_node[1];
    Type type = AnalasysForSpecifier(Specifier_);
    
    return AnalasysForDecList(DecList_,type);
}

FieldList AnalasysForDecList(tree_node* ptr, Type type){
    /*
DecList : Dec 
    | Dec COMMA DecList
    */
    if(ptr==nullptr)
        return nullptr;

    tree_node*  Dec_ = ptr->child_node[0];
    FieldList fieldList = new FieldList_();
    fieldList = AnalasysForDec(Dec_, type);
    if(ptr->child_num!=1){
        tree_node*  DecList_ = ptr->child_node[2];
        fieldList->tail= AnalasysForDecList(DecList_,type);
    }
    return fieldList;

}

FieldList AnalasysForDec(tree_node* ptr, Type type){
    /*
Dec : VarDec 
    | VarDec ASSIGNOP Exp 
    ;
    */
    if(ptr==nullptr)
        return nullptr;

    tree_node*  VarDec_ = ptr->child_node[0];
    FieldList fieldList = new FieldList_();
    
    fieldList = AnalasysForVarDec(VarDec_,type);
    if(ptr->child_num == 3){
        tree_node*  Exp_ = ptr->child_node[2];
        AnalasysForExp(Exp_);
    }
    //todo ASSIGN
    return fieldList;

}

void AnalasysForStmt(tree_node* ptr,Type returnType){
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
    if(ptr->child_node[0]->node_type==ENUM_RETURN){
        tree_node* Exp_ = ptr->child_node[1];

        //if(returnType->kind != AnalasysForExp(Exp_)){
            //todo
        //    fprintf(stderr,"Error type 8 at Line %d: %s %s.\n",Exp_->line_no,"Type mismatched for return ",Exp_->node_name);
        //}
    }
    if (ptr->child_num == 2 ){
        tree_node*  Exp_ = ptr->child_node[0];
        tree_node*  SEMI_ = ptr->child_node[1];
        AnalasysForExp(Exp_);
        AnalasysForSEMI(SEMI_);
    }else if(ptr->child_num == 1 ){
        tree_node*  CompSt_ = ptr->child_node[0];
        AnalasysForCompSt(CompSt_,nullptr);

    }else if(ptr->child_num == 3 ){

    }else if(ptr->child_num == 5 ){
        //todo

    }else if(ptr->child_num == 7 ){

    }

}


FieldList AnalasysForVarDec(tree_node* ptr, Type type){
/*
VarDec : ID 
    | VarDec LB INT RB
*/
    if(ptr==nullptr)
        return nullptr;
    FieldList fieldList = new FieldList_();
    
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
        cur_item.type = type;
        if(Sysmtable.find(name)!=Sysmtable.end() ){
            fprintf(stderr,"Error type 3 at Line %d: %s %s.\n",ID_->line_no,"Redifined variable",ID_->node_name);
        }else{
            Sysmtable.insert(std::pair<std::string,Sysmtable_item>(name,cur_item));
            fieldList->name = ID_->node_name;
            fieldList->type = type; //todo
        }
        //std::cout << ptr->node_name << std::endl;
        //std::cout << ptr->node_type << std::endl;
    }else{
        tree_node* VarDec_ = ptr->child_node[0];
        tree_node* INT_ = ptr->child_node[2];
         //todo
        fieldList= AnalasysForVarDec(VarDec_, type);
         // change into ARRAY
         //Type_ temp = global_type_ptr->kind;
         //global_type_ptr->u.array.elem = global_type_ptr->kind;
         //global_type_ptr->kind = global_type_ptr->ARRAY;
    }
    return fieldList;
    
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
    //global_type_ptr->u.basic = BASIC_INT;
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
    Type type = AnalasysForSpecifier(Specifier_);
    AnalasysForVarDec(VarDec_, type);

}

Type AnalasysForExp(tree_node* ptr){
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
        return nullptr;
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
    return nullptr;

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
