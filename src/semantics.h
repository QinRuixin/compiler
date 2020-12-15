#ifndef SEMANTIC_H
#define SEMANTIC_H

#include "tree.h"
#include<iostream>
#include<map>

typedef struct Type_* Type;
typedef struct FieldList_* FieldList;

struct Type_{
    enum{ BASIC, ARRAY, STRUCTURE } kind;
    union{
        int basic; // 0 int 1 float
        struct {Type elem; int size; } array;
        FieldList structure;
    }u;
};

struct FieldList_{
    char* name;
    Type type;
    FieldList tail;
};


struct Sysmtable_item{
    enum{VARIABLE, FUNCTION, CONST}kind;
    std::string name;
    Type type;
    int row;
    //todo


};

Type global_type_ptr;
std::map<std::string, struct Sysmtable_item> Sysmtable;

#endif

void AnalasysForProgram(tree_node* ptr);
void AnalasysForExtDefList(tree_node* ptr);
void AnalasysForExtDef(tree_node* ptr);
void AnalasysForExtDecList(tree_node* ptr);

void AnalasysForSpecifier(tree_node* ptr);
void AnalasysForStructSpecifier(tree_node* ptr);
void AnalasysForOptTag(tree_node* ptr);
void AnalasysForTag(tree_node* ptr);

void AnalasysForVarDec(tree_node* ptr);
void AnalasysForFunDec(tree_node* ptr);
void AnalasysForVarList(tree_node* ptr);
void AnalasysForParamDec(tree_node* ptr);

void AnalasysForCompSt(tree_node* ptr);
void AnalasysForStmtList(tree_node* ptr);
void AnalasysForStmt(tree_node* ptr);

void AnalasysForDefList(tree_node* ptr);
void AnalasysForDef(tree_node* ptr);
void AnalasysForDecList(tree_node* ptr);
void AnalasysForDec(tree_node* ptr);

void AnalasysForExp(tree_node* ptr);
void AnalasysForID(tree_node* ptr);
void AnalasysFor(tree_node* ptr);
void AnalasysFor(tree_node* ptr);
void AnalasysFor(tree_node* ptr);

void AnalasysForCOMMA(tree_node* ptr);
void AnalasysForSEMI(tree_node* ptr);
