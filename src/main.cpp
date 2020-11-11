#include<stdio.h>
#include<stdlib.h>

extern FILE* yyin;
extern "C" int yylex();

int main(int argc, char** argv){
    if(argc > 1){
        if(!(yyin = fopen(argv[1],"r"))){
            perror(argv[1]);
            return 1;
        }
    }
    while (yylex()!=0);
    {
        
    }
    return 0;
    
}