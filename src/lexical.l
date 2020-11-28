%option yylineno
%{
    #include "syntax.tab.h"
    #include<stdio.h>
    extern int error_flag;
    extern int second_flag;
%}
WS [ \r\n\t\f\v]+
INT_DEC 0|([1-9][0-9]*)
INT_OCT 0([0-7]+)
INT_HEX 0[Xx][0-9A-Fa-f]+
FLOAT [0-9]+\.[0-9]+
FLOAT_E ([0-9]*\.[0-9]+|[0-9]+\.[0-9]*)[E|e][+-]?[0-9]+
ID [_a-zA-Z]([_a-zA-Z0-9]*)
SEMI ";"
COMMA ","
ASSIGNOP "="
RELOP ">"|"<"|">="|"<="|"=="|"!="
PLUS "+"
MINUS "-"
STAR "*"
DIV "/"
AND "&&"
OR "||"
DOT \.
NOT "!"
TYPE "int"|"float"
LP "("
RP ")"
LB "["
RB "]"
LC "{"
RC "}"
STRUCT struct
RETURN return
IF if
ELSE else
WHILE while
COMMENT_LINE "//".*\n
COMMENT_MUL "/"\*([^*]|\*+[^/*])*\*+"/"

%%

{COMMENT_LINE} {}
{COMMENT_MUL} {}


\n {
  yycolumn = 1;
}

{WS} {}

";" {
    if(second_flag==1){
    fprintf(stderr,"SEMI %s at Line %d.\n",yytext,yylineno);
  }
}
"," {
    if(second_flag==1){
    fprintf(stderr,"COMMA %s at Line %d.\n",yytext,yylineno);
  }
}
"=" {
    if(second_flag==1){
    fprintf(stderr,"ASSIGNOP %s at Line %d.\n",yytext,yylineno);
  }
}
">"|"<"|">="|"<="|"=="|"!=" {
    if(second_flag==1){
    fprintf(stderr,"RELOP %s at Line %d.\n",yytext,yylineno);
  }
}
"+" {
    if(second_flag==1){
    fprintf(stderr,"PLUS %s at Line %d.\n",yytext,yylineno);
  }
}
"-" {
    if(second_flag==1){
    fprintf(stderr,"MINUS %s at Line %d.\n",yytext,yylineno);
  }
}
"*" {
    if(second_flag==1){
    fprintf(stderr,"STAR %s at Line %d.\n",yytext,yylineno);
  }
}
"/" {
    if(second_flag==1){
    fprintf(stderr,"DIV %s at Line %d.\n",yytext,yylineno);
  }
}
"&&" {
    if(second_flag==1){
    fprintf(stderr,"AND %s at Line %d.\n",yytext,yylineno);
  }
}
"||" {
    if(second_flag==1){
    fprintf(stderr,"OR %s at Line %d.\n",yytext,yylineno);
  }
}
"." {
    if(second_flag==1){
    fprintf(stderr,"DOT %s at Line %d.\n",yytext,yylineno);
  }
}
"!" {
    if(second_flag==1){
    fprintf(stderr,"NOT %s at Line %d.\n",yytext,yylineno);
  }
}
"int"|"float" {
    if(second_flag==1){
    fprintf(stderr,"TYPE %s at Line %d.\n",yytext,yylineno);
  }
}
"(" {
    if(second_flag==1){
    fprintf(stderr,"LP %s at Line %d.\n",yytext,yylineno);
  }
}
")" {
    if(second_flag==1){
    fprintf(stderr,"RP %s at Line %d.\n",yytext,yylineno);
  }
}
"[" {
    if(second_flag==1){
    fprintf(stderr,"LB %s at Line %d.\n",yytext,yylineno);
  }
}
"]" {
    if(second_flag==1){
    fprintf(stderr,"RB %s at Line %d.\n",yytext,yylineno);
  }
}
"{" {
    if(second_flag==1){
    fprintf(stderr,"LC %s at Line %d.\n",yytext,yylineno);
  }
}
"}" {
    if(second_flag==1){
    fprintf(stderr,"RC %s at Line %d.\n",yytext,yylineno);
  }
}
{STRUCT} {
    if(second_flag==1){
    fprintf(stderr,"STRUCT %s at Line %d.\n",yytext,yylineno);
  }
}
{RETURN} {
    if(second_flag==1){
    fprintf(stderr,"RETURN %s at Line %d.\n",yytext,yylineno);
  }
}
{IF} {
    if(second_flag==1){
    fprintf(stderr,"IF %s at Line %d.\n",yytext,yylineno);
  }
}
{ELSE} {
    if(second_flag==1){
    fprintf(stderr,"ELSE %s at Line %d.\n",yytext,yylineno);
  }
}
{WHILE} {
    if(second_flag==1){
    fprintf(stderr,"WHILE %s at Line %d.\n",yytext,yylineno);
  }
}

{INT_OCT} {
    if(second_flag==1){
    char * pEnd;
    fprintf(stderr,"INT %lld at Line %d.\n",strtoll(yytext,&pEnd,8),yylineno);
    }
}
{INT_HEX} {
    if(second_flag==1){
    char * pEnd;
    fprintf(stderr,"INT %lld at Line %d.\n",strtoll(yytext,&pEnd,16),yylineno);
    }
}

{FLOAT_E} {
    if(second_flag==1){
    fprintf(stderr,"FLOAT %f at Line %d.\n",atof(yytext),yylineno);
  }
}
{FLOAT} {
    if(second_flag==1){
    fprintf(stderr,"FLOAT %f at Line %d.\n",atof(yytext),yylineno);
  }
}


{INT_DEC} {
    if(second_flag==1){
      fprintf(stderr,"INT %lld at Line %d.\n",atoll(yytext),yylineno);
    }

}


{ID} {
    if(second_flag==1){
    fprintf(stderr,"ID %s at Line %d.\n",yytext,yylineno);
  }
}

. {
    error_flag = 1;
    fprintf(stderr,"Error type A at Line %d: Mysterious characters \"%s\".\n", yylineno, yytext);

}
%%
