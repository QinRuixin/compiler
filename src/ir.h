#include "semantics.h"
#include "intercode.h"
#include<fstream>
#include<list>

typedef enum CodeType{
    LABEL, FUNCTION, ASSIGN_CODE, BINOP, GETADD, DEADD_R, DEADD_L,
    GOTO, IF, RETURN, DEC, ARG, CALL, PARAM, READ, WRITE
} CODE_TYPE;

void append_code(InterCode* cur_code);
void printOperand(std::ofstream& outputfile, Operand* operand);
void printCode(std::ofstream& outputfile);

void Translate(tree_node* ptr,std::map<std::string, struct Sysmtable_item>& Sysmtable);
void TranslateProgram(tree_node* ptr, std::map<std::string, struct Sysmtable_item>& Sysmtable, std::ofstream& outputfile);
void TranslateExp(tree_node* ptr, std::map<std::string, struct Sysmtable_item>& Sysmtable, Operand* place);
void TranslateStmt(tree_node* ptr,std::map<std::string, struct Sysmtable_item>& Sysmtable);
void TranslateCond(tree_node* ptr, Operand* label1,Operand* label2,std::map<std::string, struct Sysmtable_item>& Sysmtable);
void TranslateArgs(tree_node* ptr,std::map<std::string, struct Sysmtable_item>& Sysmtable);
void TranslateCompSt(tree_node* ptr,std::map<std::string, struct Sysmtable_item>& Sysmtable);
void TranslateDefList(tree_node* ptr,std::map<std::string, struct Sysmtable_item>& Sysmtable);
void TranslateStmtList(tree_node* ptr,std::map<std::string, struct Sysmtable_item>& Sysmtable);
void TranslateDef(tree_node* ptr,std::map<std::string, struct Sysmtable_item>& Sysmtable);
void TranslateDecList(tree_node* ptr,std::map<std::string, struct Sysmtable_item>& Sysmtable);
void TranslateDec(tree_node* ptr,std::map<std::string, struct Sysmtable_item>& Sysmtable);

void TranslateFunDec(tree_node* ptr,std::map<std::string, struct Sysmtable_item>& Sysmtable);
void TranslateVarList(tree_node* ptr,std::map<std::string, struct Sysmtable_item>& Sysmtable);
void TranslateParamDec(tree_node* ptr,std::map<std::string, struct Sysmtable_item>& Sysmtable);
void TranslateVarDec(tree_node* ptr,std::map<std::string, struct Sysmtable_item>& Sysmtable);
void TranslateArgs(tree_node* ptr,std::map<std::string, struct Sysmtable_item>& Sysmtable,std::list<Operand*>& arg_list);
