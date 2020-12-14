#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<string>
#include<map>
#include <assert.h>

//extern "C" 
#include "tree.h"
//#include"syntax.tab.h"
//#include "lex.yy.c"
extern FILE* yyin;

extern "C" int yylex();
extern "C" int yyparse();
//extern "C" void traverse(tree_node* root,int cur_deep);
extern "C" int yyrestart(FILE* file);
extern tree_node* root;


int error_flag;
int syntax_error_flag;
int second_flag;
extern int yylineno;

struct Sysmtable_item{
    enum{VARIABLE, FUNCTION, CONST}kind;
    std::string name;
    //Type type;
    int row;
    //todo


};

std::map<std::string, struct Sysmtable_item> Sysmtable;

void AnalasysForExtDef(tree_node* ptr){
    
}

void AnalasysForExtDefList(tree_node* ptr){
    
    if(ptr==nullptr)
        return;
    //assert(strcmp(ptr->name,"ExtDefList") == 0 ); //"wrong at AnalasysBegins"

    tree_node* extDef_ = ptr->child_node[0];
    tree_node* extDefList_ = ptr->child_node[1];
    
    AnalasysForExtDef(extDef_);
    AnalasysForExtDefList(extDefList_);
}


int main(int argc, char** argv){
    error_flag = 0;
    second_flag = 0;
    syntax_error_flag = 0;
    if(argc > 1){
        if(!(yyin = fopen(argv[1],"r"))){
            perror(argv[1]);
            return 1;
        }
    }
    while (yylex()!=0);
    // second time. 
    if(error_flag==0){
        second_flag = 1;
        yyin = fopen(argv[1],"r");
        yylineno = 1;
        //yyrestart(yyin);
        yyparse();
    }
    if(syntax_error_flag == 0){
        //todo
        traverse(root, 0);
    }

    return 0;
    
}