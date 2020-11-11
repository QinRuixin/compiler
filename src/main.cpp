#include<stdio.h>
#include<stdlib.h>

extern FILE* yyin;
extern "C" int yylex();

extern int error_flag;
extern int second_flag;
extern int yylineno;

int main(int argc, char** argv){
    if(argc > 1){
        if(!(yyin = fopen(argv[1],"r"))){
            perror(argv[1]);
            return 1;
        }
    }
    while (yylex()!=0);
    fprintf(stderr,"error_flag %d\n",error_flag);
    // second time
    if(error_flag==0){
        second_flag = 1;
        yylineno = 1;
        yyin = fopen(argv[1],"r");
            while (yylex()!=0);
    }

    return 0;
    
}