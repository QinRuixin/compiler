%option yylineno
%{
    #include "syntax.tab.h"
    #include<stdio.h>
    extern int error_flag;
    extern int first_flag;
%}
WS [\n|" "|\t]
INT_DEC [0]|[1-9][0-9]*
INT_OCT 0[0-7]+
INT_HEX 0[X|x][0-9A-Fa-f]+
FLOAT [0-9]+\.[0-9]+
FLOAT_E ([0-9]*\.?[0-9]+|[0-9]+\.)[E|e][+-]?[0-9]*
ID [_a-zA-z][_a-zA-z0-9]*
SEMI ;
COMMA ,
ASSIGNOP =
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
WHILE where
COMMENT_LINE [\/]{2}[.]*[\n]
COMMENT_MUL [\/][\*][.]*[\*][\/]
COMMENT_MUL_LEFT [\/][\*]
COMMENT_MUL_RIGHT  [\*][\/]

%%

COMMENT_LINE {
  return 0;
}
COMMENT_MUL {
  return 0;
}
COMMENT_MUL_LEFT {
    error_flag = 1;
    fprintf(stderr,"Error type B at Line %d: Syntax error\n", yylineno);
}
COMMENT_MUL_RIGHT {
    error_flag = 1;
    fprintf(stderr,"Error type B at Line %d: Syntax error\n", yylineno);
}

{WS} {}

{INT_DEC} {
  if(first_flag) return 0;
    fprintf(stderr,"INT %d at Line %d.\n",atoi(yytext),yylineno);
}
{INT_OCT} {
  if(first_flag) return 0;
    char * pEnd;
    fprintf(stderr,"INT %ld at Line %d.\n",strtol(yytext,&pEnd,8),yylineno);
}
{INT_HEX} {
  if(first_flag) return 0;
    char * pEnd;
    fprintf(stderr,"INT %ld at Line %d.\n",strtol(yytext,&pEnd,16),yylineno);
}
{FLOAT} {
    if(first_flag) return 0;
    fprintf(stderr,"FLOAT %f at Line %d.\n",atof(yytext),yylineno);
}
{FLOAT_E} {
    if(first_flag) return 0;
    fprintf(stderr,"FLOAT %f at Line %d.\n",atof(yytext),yylineno);
}

{SEMI} {
    if(first_flag) return 0;
    fprintf(stderr,"SEMI %s at Line %d.\n",yytext,yylineno);
}
{COMMA} {
    if(first_flag) return 0;
    fprintf(stderr,"COMMA %s at Line %d.\n",yytext,yylineno);
}
{ASSIGNOP} {
    if(first_flag) return 0;
    fprintf(stderr,"ASSIGNOP %s at Line %d.\n",yytext,yylineno);
}
{RELOP} {
    if(first_flag) return 0;
    fprintf(stderr,"RELOP %s at Line %d.\n",yytext,yylineno);
}
{PLUS} {
    if(first_flag) return 0;
    fprintf(stderr,"PLUS %s at Line %d.\n",yytext,yylineno);
}
{MINUS} {
    if(first_flag) return 0;
    fprintf(stderr,"MINUS %s at Line %d.\n",yytext,yylineno);
}
{STAR} {
    if(first_flag) return 0;
    fprintf(stderr,"STAR %s at Line %d.\n",yytext,yylineno);
}
{DIV} {
    if(first_flag) return 0;
    fprintf(stderr,"DIV %s at Line %d.\n",yytext,yylineno);
}
{AND} {
    if(first_flag) return 0;
    fprintf(stderr,"AND %s at Line %d.\n",yytext,yylineno);
}
{OR} {
    if(first_flag) return 0;
    fprintf(stderr,"OR %s at Line %d.\n",yytext,yylineno);
}
{DOT} {
    if(first_flag) return 0;
    fprintf(stderr,"DOT %s at Line %d.\n",yytext,yylineno);
}
{NOT} {
    if(first_flag) return 0;
    fprintf(stderr,"NOT %s at Line %d.\n",yytext,yylineno);
}
{TYPE} {
    if(first_flag) return 0;
    fprintf(stderr,"TYPE %s at Line %d.\n",yytext,yylineno);
}
{LP} {
    if(first_flag) return 0;
    fprintf(stderr,"LP %s at Line %d.\n",yytext,yylineno);
}
{RP} {
    if(first_flag) return 0;
    fprintf(stderr,"RP %s at Line %d.\n",yytext,yylineno);
}
{LB} {
    if(first_flag) return 0;
    fprintf(stderr,"LB %s at Line %d.\n",yytext,yylineno);
}
{RB} {
    if(first_flag) return 0;
    fprintf(stderr,"RB %s at Line %d.\n",yytext,yylineno);
}
{LC} {
    if(first_flag) return 0;
    fprintf(stderr,"LC %s at Line %d.\n",yytext,yylineno);
}
{RC} {
    if(first_flag) return 0;
    fprintf(stderr,"RC %s at Line %d.\n",yytext,yylineno);
}
{STRUCT} {
    if(first_flag) return 0;
    fprintf(stderr,"STRUCT %s at Line %d.\n",yytext,yylineno);
}
{RETURN} {
    if(first_flag) return 0;
    fprintf(stderr,"RETURN %s at Line %d.\n",yytext,yylineno);
}
{IF} {
    if(first_flag) return 0;
    fprintf(stderr,"IF %s at Line %d.\n",yytext,yylineno);
}
{ELSE} {
    if(first_flag) return 0;
    fprintf(stderr,"ELSE %s at Line %d.\n",yytext,yylineno);
}
{WHILE} {
    if(first_flag) return 0;
    fprintf(stderr,"WHILE %s at Line %d.\n",yytext,yylineno);
}


{ID} {
    fprintf(stderr,"ID %s at Line %d.\n",yytext,yylineno);
}
. {
    error_flag = 1;
    fprintf(stderr,"Error type A at Line %d: Mysterious characters \"%s\"\n", yylineno, yytext);
}
%%
