#include<stdio.h>
#include<stdlib.h>

extern FILE* yyin;
extern "C" int yylex();
extern "C" int yyparse();

int error_flag;
int syntax_error_flag;
int second_flag;
extern int yylineno;

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
    // second time. first time in lab2.
    if(error_flag==0){
        second_flag = 1;
        yylineno = 1;
        yyin = fopen(argv[1],"r");
        yyparse();
    }
    if(syntax_error_flag == 0){
        //todo
    }

    return 0;
    
}