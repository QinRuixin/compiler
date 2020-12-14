#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<string>
#include<map>

extern FILE* yyin;
//extern struct tree_node;
extern "C" int yylex();
extern "C" int yyparse();
extern "C" int yyrestart(FILE* file);
extern "C" void traverse(struct tree_node* root,int cur_deep);
extern tree_node* root;
//extern enum NODE_TYPE;


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

void AnalasysForExtDefList(struct tree_node* ptr){
    
    if(ptr==nullptr)
        return;
    //Assert(strcmp(ptr->name,"ExtDefList") == 0, "wrong at AnalasysBegins");

    //struct tree_node* extDef = ptr->child_node[0];
    //struct tree_node* extDef = ptr->child_node[1];
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