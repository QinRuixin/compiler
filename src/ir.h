#include "semantics.h"
#include<fstream>

typedef enum CodeType{
    LABEL, FUNCTION, ASSIGN, BINOP, GETADD, DEADD_R, DEADD_L,
    GOTO, IF, RETURN, DEC, ARG, CALL, PARAM, READ, WRITE
} CODE_TYPE;

void Translate(tree_node* ptr,std::map<std::string, struct Sysmtable_item>& Sysmtable);
void TranslateProgram(tree_node* ptr, std::map<std::string, struct Sysmtable_item>& Sysmtable, std::ofstream& outputfile);
void TranslateExp(tree_node* ptr, std::map<std::string, struct Sysmtable_item>& Sysmtable, std::string place);
void Translate(tree_node* ptr,std::map<std::string, struct Sysmtable_item>& Sysmtable);
void Translate(tree_node* ptr,std::map<std::string, struct Sysmtable_item>& Sysmtable);
void Translate(tree_node* ptr,std::map<std::string, struct Sysmtable_item>& Sysmtable);
void Translate(tree_node* ptr,std::map<std::string, struct Sysmtable_item>& Sysmtable);
void Translate(tree_node* ptr,std::map<std::string, struct Sysmtable_item>& Sysmtable);
void Translate(tree_node* ptr,std::map<std::string, struct Sysmtable_item>& Sysmtable);
void Translate(tree_node* ptr,std::map<std::string, struct Sysmtable_item>& Sysmtable);
void Translate(tree_node* ptr,std::map<std::string, struct Sysmtable_item>& Sysmtable);
void Translate(tree_node* ptr,std::map<std::string, struct Sysmtable_item>& Sysmtable);

void append_code(CODE_TYPE type,...);