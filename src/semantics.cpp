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

int isINT(Type child_type){
    return child_type->kind==child_type->BASIC && child_type->u.basic==BASIC_INT;
}

int notINT(Type child_type){
    return child_type->kind!=child_type->BASIC || (child_type->kind==child_type->BASIC && child_type->u.basic!=BASIC_INT);
}

int notTYPEbasicEq(Type main_type,Type child_type){
    return main_type->kind != main_type->BASIC || child_type->kind != main_type->BASIC || (main_type->u.basic != child_type->u.basic );
}

int TypeEq(Type main_type,Type child_type){
    if(main_type->kind!=child_type->kind){
        return 0;
    }
    if(main_type->kind == main_type->BASIC){
        return main_type->u.basic == child_type->u.basic;
    }else if(main_type->kind == main_type->ARRAY){
        return (main_type->u.array.size == child_type->u.array.size) &&
            TypeEq(main_type->u.array.elem, child_type->u.array.elem);
    }else if(main_type->kind == main_type->STRUCTURE){
        //todo? struct name equ??
        return main_type->u.structure->name == child_type->u.structure->name;
    }
    return 1;
}

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
    if(ptr->child_num==3){
        tree_node*  second_node = ptr->child_node[1];
//debug 
//std::cout << "ExtDef second_node->node_type "  << second_node->node_type<< std::endl;

        if(second_node->node_type==ENUM_ExtDecList){
            //debug 
            //std::cout << "ENUM_ExtDecList" << std::endl;

            AnalasysForExtDecList(second_node, specifier_type);
            
        }else if(second_node->node_type==ENUM_FunDec){ //ENUM_FunDec
            //debug 
            //std::cout << "ENUM_FunDec" << std::endl;

            AnalasysForFunDec(second_node,specifier_type);
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
            //res->LR_value = R_;
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
    //global_type_ptr->kind = global_type_ptr->STRUCTURE;
    res->kind = res->STRUCTURE;
    if(ptr->child_num == 2 ){ // STRUCT Tag 
        tree_node*  Tag_ = ptr->child_node[1];
        tree_node*  ID_ = Tag_->child_node[0];
        auto it = Sysmtable.find(ID_->node_name);
        if(it ==Sysmtable.end() || it->second.type->kind != it->second.type->STRUCTURE){
            fprintf(stderr,"Error type 17 at Line %d: %s %s.\n",ID_->line_no,"Undifined structure",ID_->node_name);
            return nullptr;
        }
        return it->second.type;

    }else{
        // STRUCT OptTag LC DefList RC 
        tree_node*  OptTag_ = ptr->child_node[1];
        tree_node*  DefList_ = ptr->child_node[3];
        Structure cur_struct = new Structure_();
        cur_struct->domain = AnalasysForDefList(DefList_);
        res->kind = res->STRUCTURE;
        res->u.structure = cur_struct;
        if(OptTag_->child_node[0]!=nullptr){
            tree_node*  ID_ = OptTag_->child_node[0];

            if(Sysmtable.find(ID_->node_name)!=Sysmtable.end() ){
                fprintf(stderr,"Error type 16 at Line %d: %s %s.\n",ID_->line_no,"Duplicated name",ID_->node_name);
                return nullptr;
            }else{
                
                cur_struct->name = ID_->node_name;
                Sysmtable_item cur_item;
                cur_item.kind = cur_item.STRUCTURE;
                cur_item.name = ID_->node_name;
                cur_item.row = ID_->line_no;
                cur_item.type = res;
                Sysmtable.insert(std::pair<std::string,Sysmtable_item>(cur_item.name,cur_item));

            } 
        }
//        else{
//            //todo?
//            AnalasysForDefList(DefList_);
//        }
        
    }
    return res;

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
        Type second_type = AnalasysForExp(Exp_);
        if(notTYPEbasicEq(fieldList->type,type) ){
            fprintf(stderr,"Error type 5 at Line %d: %s %s.\n",Exp_->line_no,Exp_->node_name,"Type mismatched for assignment");
            // return nullptr;
        }
    }
    return fieldList;

}
//---
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
    if(ptr->child_node[0]->node_type==ENUM_RETURN){ // RETURN Exp SEMI 
        tree_node* Exp_ = ptr->child_node[1];
        Type second_type = AnalasysForExp(Exp_);
        if(TypeEq(returnType, second_type)){
            fprintf(stderr,"Error type 8 at Line %d: %s %s.\n",Exp_->line_no,"Type mismatched for return ",Exp_->node_name);
        }
        return;
    }
    if (ptr->child_num == 2 ){ // Exp SEMI 
        tree_node*  Exp_ = ptr->child_node[0];
        AnalasysForExp(Exp_);
    }else if(ptr->child_num == 1 ){ // CompSt
        tree_node*  CompSt_ = ptr->child_node[0];
        AnalasysForCompSt(CompSt_,returnType);
    }else {
        tree_node*  Exp_ = ptr->child_node[2];
        Type type = AnalasysForExp(Exp_);
        if(notINT(type)){
            fprintf(stderr,"Error type 7 at Line %d: %s %s.\n",Exp_->line_no,"Type mismatched for return ",Exp_->node_name);
            return;
        }
        tree_node*  Stmt_ = ptr->child_node[4];
        AnalasysForStmt(Stmt_,returnType);
        if(ptr->child_num == 7 ){
            tree_node*  Stmt_ = ptr->child_node[6];
            AnalasysForStmt(Stmt_,returnType);
        }
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
Function AnalasysForFunDec(tree_node* ptr, Type type ){
    /*
    FunDec : ID LP VarList RP
    | ID LP RP
    */
    if(ptr==nullptr)
        return nullptr;
    //global_type_ptr->kind = global_type_ptr->STRUCTURE;
    //global_type_ptr->u.basic = BASIC_INT;
    tree_node* ID_ = ptr->child_node[0];
    Function function = new Function_();

    if(Sysmtable.find(ID_->node_name)!=Sysmtable.end() ){
        fprintf(stderr,"Error type 4 at Line %d: %s %s.\n",ID_->line_no,"Redifined variable",ID_->node_name);
        return nullptr;
    }else{
        function->name = ID_->node_name;
        function->returnType = type;
        Sysmtable_item cur_item;
        cur_item.kind = cur_item.FUNCTION;
        cur_item.name = ID_->node_name;
        cur_item.row = ID_->line_no;
        cur_item.type = type; //return type?
        //todo
        Sysmtable.insert(std::pair<std::string,Sysmtable_item>(ID_->node_name,cur_item));
    }
    if(ptr->child_num==4){
        tree_node*  VarList_ = ptr->child_node[2];
        function->parameter = AnalasysForVarList(VarList_);

    }else{
        function->parameter = nullptr;
    }
    return function;

}


FieldList AnalasysForVarList(tree_node* ptr){
    /*
VarList : ParamDec COMMA VarList 
    | ParamDec 
    */
    if(ptr==nullptr)
        return nullptr;
    tree_node*  ParamDec_ = ptr->child_node[0];
    FieldList param = AnalasysForParamDec(ParamDec_);
    if (ptr->child_num == 3 ){
        tree_node*  VarList_ = ptr->child_node[2];
        param->tail = AnalasysForVarList(VarList_);
    }
    return param;
}

FieldList AnalasysForParamDec(tree_node* ptr){
/*
ParamDec : Specifier VarDec
*/
    if(ptr==nullptr)
        return nullptr;
    tree_node*  Specifier_ = ptr->child_node[0];
    tree_node*  VarDec_ = ptr->child_node[1];
    Type type = AnalasysForSpecifier(Specifier_);
    
    return AnalasysForVarDec(VarDec_, type);

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
    Type res = new Type_();
    
    if(ptr==nullptr)
        return nullptr;
    if (ptr->child_num == 1 ){ // | ID| INT| FLOAT 
        if(ptr->child_node[0]->node_type==ENUM_ID){
            tree_node* ID_ = ptr->child_node[0];
            if(Sysmtable.find(ID_->node_name) == Sysmtable.end() ){
                 fprintf(stderr,"Error type 1 at Line %d: %s %s.\n",ID_->line_no,"Undifined variable",ID_->node_name);
                return nullptr;
            }else{
                Sysmtable_item ID_item =  Sysmtable.find(ID_->node_name)->second;
                res = ID_item.type;
            }
            
        }else if(ptr->child_node[0]->node_type==ENUM_INT){
            res->kind = res->BASIC;
            res->u.basic = BASIC_INT;
            res->LR_value = R_;

        }else if(ptr->child_node[0]->node_type==ENUM_FLOAT){
            res->kind = res->BASIC;
            res->u.basic = BASIC_FLOAT;
            res->LR_value = R_;
        }
        return res;
    }else if(ptr->child_node[0]->node_type==ENUM_ID){ // | ID LP Args RP | ID LP RP 
        // function call
        tree_node* ID_ = ptr->child_node[0];
        if(Sysmtable.find(ID_->node_name) == Sysmtable.end() ){
                fprintf(stderr,"Error type 2 at Line %d: %s %s.\n",ID_->line_no,"Undifined function",ID_->node_name);
                return nullptr;
        }else{
            //todo args mismatched
        }
        //todo returnType
        //res->kind = res->

    }else if(ptr->child_node[0]->node_type==ENUM_LP){ // LP Exp RP 
        tree_node* Exp_ = ptr->child_node[1];
        return AnalasysForExp(Exp_);

    }else if(ptr->child_node[1]->node_type==ENUM_Exp){   // | MINUS Exp %prec UMINUS | NOT Exp 
        tree_node* Exp_  = ptr->child_node[1];
        
        Type child_type = AnalasysForExp( Exp_);
        if(ptr->child_node[0]->node_type==ENUM_MINUS){
            if(child_type->kind!=child_type->BASIC){
                fprintf(stderr,"Error type 7 at Line %d: %s %s.\n",Exp_->line_no,"MINUS not BASIC",Exp_->node_name);
                return nullptr;
            }else{
                res->kind=res->BASIC;

            }

        }else{ //ptr->child_node[0]->node_type==ENUM_NOT  
            // only INT can participate logic calculation 
            if( notINT(child_type) ){
                fprintf(stderr,"Error type 7 at Line %d: %s %s.\n",Exp_->line_no,"NOT not INT",Exp_->node_name);
                return nullptr;

            }else{
                res->kind=res->BASIC;
                res->u.basic!=BASIC_INT;
            }
        }
    }else{ // start with Exp
        //     | Exp LB Exp RB | Exp DOT ID 
        tree_node* Exp_0  = ptr->child_node[0];
        Type main_type = AnalasysForExp( Exp_0 );
        if( main_type == nullptr){
            //todo  qwiuhdasdjhk[1.5]  qwiuhdasdjhk doesn't exist

            return nullptr;
        }
        if(ptr->child_node[1]->node_type==ENUM_LB){ // Exp LB Exp RB 
            tree_node* Exp_2  = ptr->child_node[2];
            // Exp_0 should be ARRAY
            if(main_type->kind!=main_type->ARRAY){
                fprintf(stderr,"Error type 10 at Line %d: %s %s.\n",Exp_0->line_no,Exp_0->node_name,"is not an array");
                return nullptr;
            }

            Type child_type = AnalasysForExp( Exp_2 );
            if(notINT(child_type)){
                fprintf(stderr,"Error type 12 at Line %d: %s %s.\n",Exp_2->line_no,Exp_2->node_name,"is not INT");
                return nullptr;
            }
            //todo  what's the type of a[1] 
        }else if(ptr->child_node[1]->node_type==ENUM_DOT){ // Exp DOT ID 
            tree_node* ID_  = ptr->child_node[2];
            //todo search the struct
            if(main_type->kind!=main_type->STRUCTURE){
                fprintf(stderr,"Error type 13 at Line %d: %s %s.\n",Exp_0->line_no,Exp_0->node_name,"Ill use of .");
                return nullptr;
            }
            //todo  what's the type of a.u

        }else{
            tree_node* Exp_2  = ptr->child_node[2];
            Type child_type = AnalasysForExp( Exp_2 );
            //5 
            tree_node* Operator  = ptr->child_node[1];
            if(Operator->node_type==ENUM_ASSIGNOP ){
    //Exp ASSIGNOP Exp 
                if(main_type->LR_value==R_){
                    fprintf(stderr,"Error type 6 at Line %d: %s %s.\n",Exp_0->line_no,Exp_0->node_name,"R value in the left");
                    return nullptr;    
                }
                if( notTYPEbasicEq(main_type,child_type) ){ //todo basic
                    fprintf(stderr,"Error type 5 at Line %d: %s %s.\n",Exp_0->line_no,Exp_0->node_name,"Type mismatched for assignment");
                    return nullptr;                    
                }

            }else if(Operator->node_type== ENUM_AND || Operator->node_type== ENUM_OR){
    //| Exp AND Exp | Exp OR Exp 
                if(notINT(main_type) || notINT(child_type)){
                fprintf(stderr,"Error type 5 at Line %d: %s %s.\n",Exp_0->line_no,Exp_0->node_name,"Type mismatched for Logic calculation");
                return nullptr;
                }
            }else{
    //| Exp RELOP Exp | Exp PLUS Exp | Exp MINUS Exp | Exp STAR Exp | Exp DIV Exp 
                if( notTYPEbasicEq(main_type,child_type)){
                    fprintf(stderr,"Error type 7 at Line %d: %s %s.\n",Exp_0->line_no,Exp_0->node_name,"Type mismatched for Operands");
                    return nullptr;
                }else{
                    res->kind = main_type->kind;
                    res->u = main_type->u;
                }
            }
            
        //return nullptr;
        }
    }
    return res;

}