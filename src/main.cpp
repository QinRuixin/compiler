#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<string>
#include<map>
#include <assert.h>
#include<iostream>
#include<fstream>
//extern "C" 
#include "tree.h"
#include "semantics.h"
//#include"syntax.tab.h"
//#include "lex.yy.c"
#include "ir.h"
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


int main(int argc, char** argv){
    error_flag = 0;
    second_flag = 0;
    syntax_error_flag = 0;
//    if(argc >= 1){
    if(argc > 1){
        if(!(yyin = fopen(argv[1],"r"))){
//        if(!(yyin = fopen("./tests/lab1/test5.cmm","r"))){
            perror(argv[1]);
            return 1;
        }
    }
    while (yylex()!=0);
    // second time. 
    if(error_flag==0){
        second_flag = 1;
        yyin = fopen(argv[1],"r");
//        yyin = fopen("./tests/lab1/test5.cmm","r");
        yylineno = 1;
        //yyrestart(yyin);
        yyparse();
    }
    if(syntax_error_flag == 0){
        //todo
        //traverse(root, 0);
        std::map<std::string, struct Sysmtable_item> Sysmtable_table = AnalasysForProgram(root);
        std::ofstream outputfile;
        outputfile.open(argv[2]);
        TranslateProgram(root, Sysmtable_table, outputfile);
        //outputfile << "writing whis to a file. \n";
        outputfile.close();
    }

    return 0;
    
}