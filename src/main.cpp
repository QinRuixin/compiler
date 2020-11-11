#include<stdio.h>
#include<stdlib.h>

extern FILE* yyin;
extern "C" int yylex();

int error_flag = 0;
int first_flag = 1;

int main(int argc, char** argv){
    if(argc > 1){
        if(!(yyin = fopen(argv[1],"r"))){
            perror(argv[1]);
            return 1;
        }
    }
    while (yylex()!=0);

    // second time
    if(error_flag==0){
        first_flag = 0;
        yyin = fopen(argv[1],"r");
            while (yylex()!=0);
    }

    return 0;
    
}