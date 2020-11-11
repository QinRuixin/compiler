%option yylineno
%{
    #include "syntax.tab.h"
    #include<stdio.h>
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
%%
{WS} {}
{INT_DEC} {
    fprintf(stderr,"INT %d at Line %d.\n",atoi(yytext),yylineno);
}
{INT_OCT} {
    char * pEnd;
    fprintf(stderr,"INT %ld at Line %d.\n",strtol(yytext,&pEnd,8),yylineno);
}
{INT_HEX} {
    char * pEnd;
    fprintf(stderr,"INT %ld at Line %d.\n",strtol(yytext,&pEnd,16),yylineno);
}
{FLOAT} {
    fprintf(stderr,"FLOAT %f at Line %d.\n",atof(yytext),yylineno);
}
{FLOAT_E} {
    fprintf(stderr,"FLOAT %f at Line %d.\n",atof(yytext),yylineno);
}

{SEMI} {
    fprintf(stderr,"SEMI %s at Line %d.\n",yytext,yylineno);
}
{COMMA} {
    fprintf(stderr,"COMMA %s at Line %d.\n",yytext,yylineno);
}
{ASSIGNOP} {
    fprintf(stderr,"ASSIGNOP %s at Line %d.\n",yytext,yylineno);
}
{RELOP} {
    fprintf(stderr,"RELOP %s at Line %d.\n",yytext,yylineno);
}
{PLUS} {
    fprintf(stderr,"PLUS %s at Line %d.\n",yytext,yylineno);
}
{MINUS} {
    fprintf(stderr,"MINUS %s at Line %d.\n",yytext,yylineno);
}
{STAR} {
    fprintf(stderr,"STAR %s at Line %d.\n",yytext,yylineno);
}
{DIV} {
    fprintf(stderr,"DIV %s at Line %d.\n",yytext,yylineno);
}
{AND} {
    fprintf(stderr,"AND %s at Line %d.\n",yytext,yylineno);
}
{OR} {
    fprintf(stderr,"OR %s at Line %d.\n",yytext,yylineno);
}
{DOT} {
    fprintf(stderr,"DOT %s at Line %d.\n",yytext,yylineno);
}
{NOT} {
    fprintf(stderr,"NOT %s at Line %d.\n",yytext,yylineno);
}
{TYPE} {
    fprintf(stderr,"TYPE %s at Line %d.\n",yytext,yylineno);
}
{LP} {
    fprintf(stderr,"LP %s at Line %d.\n",yytext,yylineno);
}
{RP} {
    fprintf(stderr,"RP %s at Line %d.\n",yytext,yylineno);
}
{LB} {
    fprintf(stderr,"LB %s at Line %d.\n",yytext,yylineno);
}
{RB} {
    fprintf(stderr,"RB %s at Line %d.\n",yytext,yylineno);
}
{LC} {
    fprintf(stderr,"LC %s at Line %d.\n",yytext,yylineno);
}
{RC} {
    fprintf(stderr,"RC %s at Line %d.\n",yytext,yylineno);
}
{STRUCT} {
    fprintf(stderr,"STRUCT %s at Line %d.\n",yytext,yylineno);
}
{RETURN} {
    fprintf(stderr,"RETURN %s at Line %d.\n",yytext,yylineno);
}
{IF} {
    fprintf(stderr,"IF %s at Line %d.\n",yytext,yylineno);
}
{ELSE} {
    fprintf(stderr,"ELSE %s at Line %d.\n",yytext,yylineno);
}
{WHILE} {
    fprintf(stderr,"WHILE %s at Line %d.\n",yytext,yylineno);
}
{ID} {
    fprintf(stderr,"ID %s at Line %d.\n",yytext,yylineno);
}
. {
    fprintf(stderr,"Error type A at Line %d: Mysterious characters \"%s\"\n", yylineno, yytext);
}
%%
