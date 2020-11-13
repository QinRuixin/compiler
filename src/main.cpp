#include<stdio.h>
#include<stdlib.h>

extern FILE* yyin;
extern "C" int yylex();
extern "C" void yyrestart(FILE*);

int error_flag;
int second_flag;
//extern int yylineno;

int main(int argc, char** argv){
    error_flag = 0;
    second_flag = 0;
    if(argc > 1){
        if(!(yyin = fopen(argv[1],"r"))){
            perror(argv[1]);
            return 1;
        }
    }
    while (yylex()!=0);
    // second time
    if(error_flag==0){
        yyrestart(yyin);
        second_flag = 1;
        //yylineno = 1;
        //yyin = fopen(argv[1],"r");
        while (yylex()!=0);
    }

    return 0;
    
}