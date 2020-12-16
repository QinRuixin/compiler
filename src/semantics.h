#ifndef SEMANTIC_H
#define SEMANTIC_H

#include "tree.h"
#include<iostream>
#include<map>
#define BASIC_INT 0
#define BASIC_FLOAT 1

#define L_ 0
#define R_ 1

typedef struct Type_* Type;
typedef struct FieldList_* FieldList;
typedef struct Function_* Function;
typedef struct Structure_* Structure;

struct Type_{
    enum{ BASIC, ARRAY, STRUCTURE,FUNCTION } kind;
    union{
        int basic; // 0 int 1 float
        struct {Type elem; int size; } array;
        Structure structure;
        Function function;
    }u;
    int LR_value;
};

struct FieldList_{
    char* name;
    Type type;
    FieldList tail;
};

struct Function_{
    char* name;
    Type returnType;
    FieldList parameter;
};

struct Structure_{
    char* name;
    FieldList domain;
};


struct Sysmtable_item{
    enum{VARIABLE, FUNCTION, STRUCTURE,CONST}kind;
    std::string name;
    Type type;
    int row;
    //todo


};

#endif

void AnalasysForProgram(tree_node* ptr);
void AnalasysForExtDefList(tree_node* ptr);
void AnalasysForExtDef(tree_node* ptr);
void AnalasysForExtDecList(tree_node* ptr, Type type);

Type AnalasysForSpecifier(tree_node* ptr);
Type AnalasysForStructSpecifier(tree_node* ptr);
void AnalasysForOptTag(tree_node* ptr);
void AnalasysForTag(tree_node* ptr);

FieldList AnalasysForVarDec(tree_node* ptr, Type type);
Function AnalasysForFunDec(tree_node* ptr, Type type );
FieldList AnalasysForVarList(tree_node* ptr);
FieldList AnalasysForParamDec(tree_node* ptr);

void AnalasysForCompSt(tree_node* ptr,Type returnType);
void AnalasysForStmtList(tree_node* ptr,Type returnType);
void AnalasysForStmt(tree_node* ptr,Type returnType);

FieldList AnalasysForDefList(tree_node* ptr);
FieldList AnalasysForDef(tree_node* ptr);
FieldList AnalasysForDecList(tree_node* ptr, Type type);
FieldList AnalasysForDec(tree_node* ptr, Type type);

Type AnalasysForExp(tree_node* ptr);
void AnalasysForID(tree_node* ptr);
