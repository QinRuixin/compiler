#include "semantics.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<string>
#include<map>
#include <assert.h>
#include<iostream>


std::map<std::string, struct Sysmtable_item> Sysmtable;

int isINT(Type child_type){
    if (child_type==nullptr )
    {
        return 0;
    }
    return child_type->kind==child_type->BASIC && child_type->u.basic==BASIC_INT;
}
/*
int isArgsMatch(FieldList param,tree_node* Args_){
    if(Args_->child_node)
}

int notINT(Type child_type){
    if (child_type==nullptr )
    {
        return 1;
    }
    return child_type->kind!=child_type->BASIC || (child_type->kind==child_type->BASIC && child_type->u.basic!=BASIC_INT);
}

int notTYPEbasicEq(Type main_type,Type child_type){
    if (main_type==nullptr || child_type==nullptr )
    {
        return 1;
    }
    
    return main_type->kind != main_type->BASIC || child_type->kind != child_type->BASIC || (main_type->u.basic != child_type->u.basic );
}
*/
int TypeEq(Type main_type,Type child_type){
    if (main_type==nullptr && child_type==nullptr )
    {
        return 1;
    }
    if (main_type==nullptr || child_type==nullptr )
    {
        return 0;
    }
    if(main_type->kind!=child_type->kind){
        return 0;
    }
    if(main_type->kind == main_type->BASIC){
        return main_type->u.basic == child_type->u.basic;
    }else if(main_type->kind == main_type->ARRAY){
        //return (main_type->u.array.size == child_type->u.array.size) &&
         return TypeEq(main_type->u.array.elem, child_type->u.array.elem);
    }else if(main_type->kind == main_type->STRUCTURE){
        //todo? struct name equ??
        //return main_type->u.structure->name == child_type->u.structure->name;
        return FieldListEq(main_type->u.structure->domain, child_type->u.structure->domain);
    }
    return 1;
}

int FieldListEq(FieldList fieldlist1, FieldList fieldlist2){
    if(fieldlist1 == nullptr && fieldlist2 == nullptr){
        return 1;
    }else if(fieldlist1 == nullptr || fieldlist2 == nullptr){
        return 0;
    }
    if(1 - TypeEq(fieldlist1->type,fieldlist2->type)){
        return 0;
    }
    return FieldListEq(fieldlist1->tail, fieldlist2->tail);

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
    if(specifier_type == nullptr){ //todo ??
        return;
    }
    if(ptr->child_num==3){
        tree_node*  second_node = ptr->child_node[1];
        if(second_node->node_type==ENUM_ExtDecList){
            //debug 
            //std::cout << "ENUM_ExtDecList" << std::endl;

            AnalasysForExtDecList(second_node, specifier_type);
        }else if(second_node->node_type==ENUM_FunDec){ //ENUM_FunDec
            //debug 
            //std::cout << "ENUM_FunDec" << std::endl;

            Function function =  AnalasysForFunDec(second_node,specifier_type);
            if(function == nullptr){
                return;
                // quit the whole CompSt
            }
            tree_node*  CompSt_ = ptr->child_node[2];
            AnalasysForCompSt(CompSt_, specifier_type);
        }
    }
    
}


void AnalasysForExtDecList(tree_node* ptr, Type type){
    //ExtDecList : VarDec 
    // | VarDec COMMA ExtDecList 
    if(ptr==nullptr)
        return;
    if (ptr->child_num == 1 )
    {
        tree_node*  VarDec_ = ptr->child_node[0];
        AnalasysForVarDec(VarDec_, type);
    }else if (ptr->child_num == 3 )
    {
        tree_node*  VarDec_ = ptr->child_node[0];
        tree_node*  ExtDecList_ = ptr->child_node[2];
        FieldList fieldList = AnalasysForVarDec(VarDec_,type);
        if(fieldList == nullptr){
            return ;
            //quit the 
        }
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
    Type res = new Type_();
    if(ptr->child_node[0]->node_type == ENUM_TYPE){
        res->kind = res->BASIC;

        if(strcmp(ptr->child_node[0]->node_name,"int")==0){
            res->u.basic = BASIC_INT;
        }else{//  if(strcmp(ptr->node_name,"float")==0)
            res->u.basic = BASIC_FLOAT;
        }
    }else{//  if(ptr->child_node[0]->node_type  == ENUM_StructSpecifier)
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
        
        if(OptTag_->child_node[0]!=nullptr){
            tree_node*  ID_ = OptTag_->child_node[0];

            if(Sysmtable.find(ID_->node_name)!=Sysmtable.end() ){
                fprintf(stderr,"Error type 16 at Line %d: %s %s.\n",ID_->line_no,"Duplicated name",ID_->node_name);
                return nullptr;
            }
            
            cur_struct->name = ID_->node_name;
            cur_struct->domain = AnalasysForDefList(DefList_, SRC_STRUCT);
            Sysmtable_item cur_item;
            cur_item.kind = cur_item.STRUCTURE;
            cur_item.name = ID_->node_name;
            cur_item.row = ID_->line_no;
            res->u.structure = cur_struct;
            cur_item.type = res;
            //debug 
        /*    std::cout << "insert " << cur_item.name <<std::endl;
            FieldList tempField =  cur_struct->domain;
            while ( tempField != nullptr)
            {
                std::cout << tempField->name << std::endl;
                tempField = tempField->tail;
            }
        */
            Sysmtable.insert(std::pair<std::string,Sysmtable_item>(cur_item.name,cur_item));
        
        }else{
            cur_struct->domain = AnalasysForDefList(DefList_,  SRC_STRUCT);
        }
        res->u.structure = cur_struct;
        
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

FieldList AnalasysForDefList(tree_node* ptr, int src){
    /*
DefList : Def DefList 
    | // Epsl
    */
    if(ptr==nullptr)
        return nullptr;
    
    tree_node*  Def_ = ptr->child_node[0];
    tree_node*  DefList_ = ptr->child_node[1];
    FieldList res = AnalasysForDef(Def_, src);
    if(res==nullptr){
        return AnalasysForDefList(DefList_,src);
    }
    res->tail =  AnalasysForDefList(DefList_, src);
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

FieldList AnalasysForDef(tree_node* ptr, int src){
    /*
Def : Specifier DecList SEMI 
    ;
    */
    if(ptr==nullptr)
        return nullptr;

    tree_node*  Specifier_ = ptr->child_node[0];
    tree_node*  DecList_ = ptr->child_node[1];
    Type type = AnalasysForSpecifier(Specifier_);
    
    return AnalasysForDecList(DecList_,type, src);
}

FieldList AnalasysForDecList(tree_node* ptr, Type type, int src){
    /*
DecList : Dec 
    | Dec COMMA DecList
    */
    if(ptr==nullptr)
        return nullptr;

    tree_node*  Dec_ = ptr->child_node[0];
    FieldList fieldList = AnalasysForDec(Dec_, type, src);

    if(ptr->child_num!=1){
        tree_node*  DecList_ = ptr->child_node[2];
        if(fieldList == nullptr){
        ///debug 
//    std::cout <<   " fieldList == nullptr " << type << std::endl;
            fieldList =  AnalasysForDecList(DecList_,type, src); 
        }else{
            fieldList->tail= AnalasysForDecList(DecList_,type, src);
        }
    }
    return fieldList;

}

FieldList AnalasysForDec(tree_node* ptr, Type type, int src){
    /*
Dec : VarDec 
    | VarDec ASSIGNOP Exp 
    ;
    */
    if(ptr==nullptr)
        return nullptr;

    tree_node*  VarDec_ = ptr->child_node[0];
    FieldList fieldList = AnalasysForVarDec(VarDec_,type, src);
    if(fieldList == nullptr){
        return nullptr;
    }
    if(ptr->child_num == 3){
        if(src == SRC_STRUCT){
            fprintf(stderr,"Error type 15 at Line %d: %s %s.\n",VarDec_->line_no,VarDec_->node_name," assignment in structure");
            return nullptr;  
        }
        tree_node*  Exp_ = ptr->child_node[2];
        Type second_type = AnalasysForExp(Exp_);
// type 6?  impossible
        if(second_type==nullptr){
            return nullptr;
        }
        //debug 
    //    std::cout <<  second_type << " " << type << std::endl;
        if( ( 1 - TypeEq(second_type,type) ) ){
            fprintf(stderr,"Error type 5 at Line %d: %s %s.\n",Exp_->line_no,Exp_->node_name,"Type mismatched for assignment");
            return nullptr;
        }
        fieldList->type = type; //todo?
    }
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
    if(ptr->child_node[0]->node_type==ENUM_RETURN){ // RETURN Exp SEMI 
        tree_node* Exp_ = ptr->child_node[1];
        Type second_type = AnalasysForExp(Exp_);
        if(second_type==nullptr){
            ; // todo?
        }
        //debug 
    //    std::cout << returnType->kind << " " << second_type->kind <<std::endl;
        if( 1 - TypeEq(returnType, second_type)){
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
        if(type==nullptr){
            return;
        }
    //    if(( 1 - isINT(type) )){
    //        fprintf(stderr,"Error type 7 at Line %d: %s %s.\n",Exp_->line_no,"Type mismatched ",Exp_->node_name);
    //        return;
    //    }
        tree_node*  Stmt_ = ptr->child_node[4];
        AnalasysForStmt(Stmt_,returnType);
        if(ptr->child_num == 7 ){
            tree_node*  Stmt_ = ptr->child_node[6];
            AnalasysForStmt(Stmt_,returnType);
        }
    }

}

FieldList AnalasysForVarDec(tree_node* ptr, Type type, int src){
/*
VarDec : ID 
    | VarDec LB INT RB
*/
    if(ptr==nullptr)
        return nullptr;
    FieldList fieldList = new FieldList_();
    fieldList->type = type;
    if(ptr->child_num==1){
        tree_node* ID_ = ptr->child_node[0];
        //std::map<std::string, struct Sysmtable_item> Sysmtable;
//debug
std::cout << "ID_->node_name" << ID_->node_name << std::endl;
        if(Sysmtable.find(ID_->node_name)!=Sysmtable.end() ){
            if(src == SRC_STRUCT){
                fprintf(stderr,"Error type 15 at Line %d: %s %s.\n",ID_->line_no,"Redifined field ",ID_->node_name);
            }else{
                fprintf(stderr,"Error type 3 at Line %d: %s %s.\n",ID_->line_no,"Redifined variable",ID_->node_name);
            }
            return nullptr;
        }else{
            Sysmtable_item cur_item;
            // todo
            cur_item.kind = cur_item.VARIABLE;
            cur_item.name = ID_->node_name;
            cur_item.row = ID_->line_no;
            cur_item.type = type;
            Sysmtable.insert(std::pair<std::string,Sysmtable_item>(ID_->node_name,cur_item));
            fieldList->name = ID_->node_name;
            return fieldList;
        }
        //std::cout << ptr->node_name << std::endl;
        //std::cout << ptr->node_type << std::endl;
    }else{
        tree_node* VarDec_ = ptr->child_node[0];
        tree_node* INT_ = ptr->child_node[2];
        Type child_type = new Type_();
        child_type->kind = fieldList->type->ARRAY;
        child_type->u.array.size = INT_->int_val;
        child_type->u.array.elem = type;
        fieldList = AnalasysForVarDec(VarDec_, child_type, src);
    }
    return fieldList;
    
}

//---
Function AnalasysForFunDec(tree_node* ptr, Type type ){
    /*
    FunDec : ID LP VarList RP
    | ID LP RP
    */
    if(ptr==nullptr)
        return nullptr;
    tree_node* ID_ = ptr->child_node[0];
    Function function = new Function_();
    function->returnType = type;
    if(ptr->child_num==4){
        tree_node*  VarList_ = ptr->child_node[2];
        function->parameter = AnalasysForVarList(VarList_);
        if(function->parameter == nullptr){
            return nullptr; //function parem error
        }
    }else{
        function->parameter = nullptr;
    }
    if(Sysmtable.find(ID_->node_name)!=Sysmtable.end() ){
        fprintf(stderr,"Error type 4 at Line %d: %s %s.\n",ID_->line_no,"Redifined variable",ID_->node_name);
        return nullptr;
    }else{
        function->name = ID_->node_name;
        Sysmtable_item cur_item;
        cur_item.kind = cur_item.FUNCTION;
        cur_item.name = ID_->node_name;
        cur_item.row = ID_->line_no;
        Type cur_type = new Type_();
        cur_type->kind = cur_type->FUNCTION;
        cur_type->u.function = function;
        cur_item.type = cur_type;
        //todo
        Sysmtable.insert(std::pair<std::string,Sysmtable_item>(ID_->node_name,cur_item));
    }

    return function;

}

//---

FieldList AnalasysForVarList(tree_node* ptr){
    /*
VarList : ParamDec COMMA VarList 
    | ParamDec 
    */
    if(ptr==nullptr)
        return nullptr;
    tree_node*  ParamDec_ = ptr->child_node[0];
    FieldList param = AnalasysForParamDec(ParamDec_);
    if(param == nullptr){
        return nullptr;
    }
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
    if(type == nullptr){
        return nullptr;
    }
    
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
            if(Sysmtable.find(ID_->node_name)->second.kind!=Sysmtable.find(ID_->node_name)->second.FUNCTION){
                fprintf(stderr,"Error type 11 at Line %d: %s %s.\n",ID_->line_no,ID_->node_name,"is not a function");
                return nullptr;
            }
            Type cur_type = Sysmtable.find(ID_->node_name)->second.type;
            if(cur_type->kind!=cur_type->FUNCTION){
                fprintf(stderr,"Error type 11 at Line %d: %s %s.\n",ID_->line_no,ID_->node_name,"is not a function");
                return nullptr;                
            }
            Function cur_function = cur_type->u.function;
            tree_node* Args_ = ptr->child_node[2];
//todo
            FieldList args = AnalasysForArgs(Args_);

            if( 1 - FieldListEq(cur_function->parameter,args)){
                fprintf(stderr,"Error type 9 at Line %d: %s %s.\n",ID_->line_no,ID_->node_name,"is not applicable for arguments ");
                return nullptr;        
            }
            return cur_function->returnType;
        }

    }else if(ptr->child_node[0]->node_type==ENUM_LP){ // LP Exp RP 
        tree_node* Exp_ = ptr->child_node[1];
        return AnalasysForExp(Exp_);

    }else if(ptr->child_node[1]->node_type==ENUM_Exp){   // | MINUS Exp %prec UMINUS | NOT Exp 
        tree_node* Exp_  = ptr->child_node[1];
        
        Type child_type = AnalasysForExp( Exp_);
        if(ptr->child_node[0]->node_type==ENUM_MINUS){
            if(child_type->kind!=child_type->BASIC){
                //fprintf(stderr,"Error type 7 at Line %d: %s %s.\n",Exp_->line_no,"MINUS not BASIC",Exp_->node_name);
                return nullptr;
            }else{
                res->kind=res->BASIC;

            }

        }else{ //ptr->child_node[0]->node_type==ENUM_NOT  
            // only INT can participate logic calculation 
            if( 1- isINT(child_type) ){
                //fprintf(stderr,"Error type 7 at Line %d: %s %s.\n",Exp_->line_no,"NOT not INT",Exp_->node_name);
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
            if( 1 - isINT(child_type)){
                fprintf(stderr,"Error type 12 at Line %d: %s %s.\n",Exp_2->line_no,Exp_2->node_name,"is not INT");
                return nullptr;
            }
            return main_type->u.array.elem; // todo check out of range
        }else if(ptr->child_node[1]->node_type==ENUM_DOT){ // Exp DOT ID 
            tree_node* ID_  = ptr->child_node[2];
            //todo search the struct
            if(main_type->kind!=main_type->STRUCTURE){
                fprintf(stderr,"Error type 13 at Line %d: %s %s.\n",Exp_0->line_no,"Ill use of ",Exp_0->node_name);
                return nullptr;
            }
            Structure cur_struct = main_type->u.structure;
            FieldList cur_fieldList = cur_struct->domain;
            int hit_flag = 0;
            // node_name can not be null
            while ( cur_fieldList != nullptr)
            {
                //debug
        //    std::cout << cur_fieldList->name << "<-cur  to find->" << ID_->node_name << std::endl;
                if(strcmp(cur_fieldList->name,ID_->node_name) == 0){  //cur_fieldList->name==ID_->node_name){
                    hit_flag = 1;
                    break;
                }
                cur_fieldList = cur_fieldList->tail;
            }
            if(hit_flag){
                //debug  todo
            //    std::cout << "cur_fieldList->type " <<cur_fieldList->type <<std::endl;
                return cur_fieldList->type; // the type of a.u
            }else{
                fprintf(stderr,"Error type 14 at Line %d: %s %s.\n",ID_->line_no,"non-existent field ",ID_->node_name);
                return nullptr;
            }
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
                if( main_type!=nullptr && ( 1 - TypeEq(main_type,child_type) )){ //todo basic
                    fprintf(stderr,"Error type 5 at Line %d: %s %s.\n",Exp_0->line_no,Exp_0->node_name,"Type mismatched for assignment");
                    return nullptr;                    
                }
                return child_type;
            }else if(Operator->node_type== ENUM_AND || Operator->node_type== ENUM_OR){
    //| Exp AND Exp | Exp OR Exp 
                if(main_type!=nullptr && ( (1-isINT(main_type)) || (1-isINT(child_type)) ) ){
    //todo           //fprintf(stderr,"Error type 5 at Line %d: %s %s.\n",Exp_0->line_no,Exp_0->node_name,"Type mismatched for Logic calculation");
                    return nullptr;
                }
                return child_type;
            }else{
    //| Exp RELOP Exp | Exp PLUS Exp | Exp MINUS Exp | Exp STAR Exp | Exp DIV Exp 
                if( 1 - TypeEq(main_type,child_type)){
                    fprintf(stderr,"Error type 7 at Line %d: %s %s.\n",Exp_0->line_no,Exp_0->node_name,"Type mismatched for Operands");
                    return nullptr;
                }else{
                    res->kind = main_type->kind;
                    res->u = main_type->u;
                }
                return child_type;
            }
            
        //return nullptr;
        }
    }
    return res;

}

FieldList AnalasysForArgs(tree_node* ptr){
/*
Args : Exp COMMA Args 
    | Exp 
*/
    if(ptr==nullptr){
        return nullptr;
    }
    FieldList res = new FieldList_();
      
    tree_node*  Exp_ = ptr->child_node[0];
    Type temp =  AnalasysForExp(Exp_);
    if(temp == nullptr){
        return nullptr;
    }
    res->type = temp;
    if(ptr->child_num == 3){
        tree_node*  Args_ = ptr->child_node[2];
        FieldList temp = AnalasysForArgs(Args_);
        if(temp == nullptr){
            return nullptr;
        }
        res->tail = temp;
    }
    return res;
}