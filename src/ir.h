#include "semantics.h"
#include "intercode.h"
#include<fstream>

typedef enum CodeType{
    LABEL, FUNCTION, ASSIGN_CODE, BINOP, GETADD, DEADD_R, DEADD_L,
    GOTO, IF, RETURN, DEC, ARG, CALL, PARAM, READ, WRITE
} CODE_TYPE;

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
void Translate(tree_node* ptr,std::map<std::string, struct Sysmtable_item>& Sysmtable);
void Translate(tree_node* ptr,std::map<std::string, struct Sysmtable_item>& Sysmtable);
void Translate(tree_node* ptr,std::map<std::string, struct Sysmtable_item>& Sysmtable);

void append_code(InterCode* cur_code);