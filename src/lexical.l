%option yylineno
%{
    #include "syntax.tab.h"
    #include <stdio.h>
    #include <stdarg.h>
    #include <string.h>
    #include "tree.h"

    extern int error_flag;
    extern int second_flag;
    int yycolumn = 1;
    struct tree_node* root;
    #define YY_USER_ACTION yylloc.first_line = yylloc.last_line = yylineno; yylloc.first_column = yycolumn; yylloc.last_column = yycolumn + yyleng -1; yycolumn += yyleng;


%}

WS [ \r\n\t\f\v]+
INT_DEC 0|([1-9][0-9]*)
INT_OCT 0([0-7]+)
INT_HEX 0[Xx][0-9A-Fa-f]+
FLOAT_N [0-9]+\.[0-9]+
FLOAT_E ([0-9]*\.[0-9]+|[0-9]+\.[0-9]*)[E|e][+-]?[0-9]+
ID [_a-zA-Z]([_a-zA-Z0-9]*)

COMMENT_LINE "//".*\n
COMMENT_MUL "/"\*([^*]|\*+[^/*])*\*+"/"

%%
\n {yycolumn=1;}
{COMMENT_LINE} {}
{COMMENT_MUL} {}
{WS} {}
";" {
    if(second_flag==1){
    //fprintf(stderr,"SEMI %s at Line %d.\n",yytext,yylineno);
    yylval.node = create_token_node(ENUM_SEMI,yylineno,0);
    return SEMI;
  }
}

"," {
    if(second_flag==1){
    //fprintf(stderr,"COMMA %s at Line %d.\n",yytext,yylineno);
    yylval.node = create_token_node(ENUM_COMMA,yylineno,0);
    return COMMA;
  }
}

"=" {
    if(second_flag==1){
    //fprintf(stderr,"ASSIGNOP %s at Line %d.\n",yytext,yylineno);
    yylval.node = create_token_node(ENUM_ASSIGNOP,yylineno,0);
    return ASSIGNOP; 
  }
}

">"|"<"|">="|"<="|"=="|"!=" {
    if(second_flag==1){
    //fprintf(stderr,"RELOP %s at Line %d.\n",yytext,yylineno);
    yylval.node = create_token_node(ENUM_RELOP,yylineno,0);
    return RELOP;
  }
}

"+" {
    if(second_flag==1){
    //fprintf(stderr,"PLUS %s at Line %d.\n",yytext,yylineno);
    yylval.node = create_token_node(ENUM_PLUS,yylineno,0);
    return PLUS;
  }
}

"-" {
    if(second_flag==1){
    //fprintf(stderr,"MINUS %s at Line %d.\n",yytext,yylineno);
    yylval.node = create_token_node(ENUM_MINUS,yylineno,0);
    return MINUS;
  }
}

"*" {
    if(second_flag==1){
    //fprintf(stderr,"STAR %s at Line %d.\n",yytext,yylineno);
    yylval.node = create_token_node(ENUM_STAR,yylineno,0);
    return STAR;
  }
}

"/" {
    if(second_flag==1){
    //fprintf(stderr,"DIV %s at Line %d.\n",yytext,yylineno);
    yylval.node = create_token_node(ENUM_DIV,yylineno,0);
    return DIV;
  }
}

"&&" {
    if(second_flag==1){
    //fprintf(stderr,"AND %s at Line %d.\n",yytext,yylineno);
    yylval.node = create_token_node(ENUM_AND,yylineno,0);
    return AND;
  }
}

"||" {
    if(second_flag==1){
    //fprintf(stderr,"OR %s at Line %d.\n",yytext,yylineno);
    yylval.node = create_token_node(ENUM_OR,yylineno,0);
    return OR;
  }
}

"." {
    if(second_flag==1){
    //fprintf(stderr,"DOT %s at Line %d.\n",yytext,yylineno);
    yylval.node = create_token_node(ENUM_DOT,yylineno,0);
    return DOT;
  }
}

"!" {
    if(second_flag==1){
    //fprintf(stderr,"NOT %s at Line %d.\n",yytext,yylineno);
    yylval.node = create_token_node(ENUM_NOT,yylineno,0);
    return NOT;
  }
}

"int"|"float" {
    if(second_flag==1){
    //fprintf(stderr,"TYPE %s at Line %d.\n",yytext,yylineno);
    yylval.node = create_token_node(ENUM_TYPE,yylineno,SELECTOR_TYPE,yytext); //todo 
    return TYPE;
  }
}

"(" {
    if(second_flag==1){
    //fprintf(stderr,"LP %s at Line %d.\n",yytext,yylineno);
    yylval.node = create_token_node(ENUM_LP,yylineno,0);
    return LP;
  }
}
")" {
    if(second_flag==1){
    //fprintf(stderr,"RP %s at Line %d.\n",yytext,yylineno);
    yylval.node = create_token_node(ENUM_RP,yylineno,0);
    return RP;
  }
}

"[" {
    if(second_flag==1){
    //fprintf(stderr,"LB %s at Line %d.\n",yytext,yylineno);
    yylval.node = create_token_node(ENUM_LB,yylineno,0);
    return LB;
  }
}

"]" {
    if(second_flag==1){
    //fprintf(stderr,"RB %s at Line %d.\n",yytext,yylineno);
    yylval.node = create_token_node(ENUM_RB,yylineno,0);
    return RB;
  }
}

"{" {
    if(second_flag==1){
    //fprintf(stderr,"LC %s at Line %d.\n",yytext,yylineno);
    yylval.node = create_token_node(ENUM_LC,yylineno,0);
    return LC;
  }
}

"}" {
    if(second_flag==1){
    //fprintf(stderr,"RC %s at Line %d.\n",yytext,yylineno);
    yylval.node = create_token_node(ENUM_RC,yylineno,0);
    return RC;
  }
}

"struct" {
    if(second_flag==1){
    //fprintf(stderr,"STRUCT %s at Line %d.\n",yytext,yylineno);
    yylval.node = create_token_node(ENUM_STRUCT,yylineno,0);
    return STRUCT;
  }
}
"return" {
    if(second_flag==1){
    //fprintf(stderr,"RETURN %s at Line %d.\n",yytext,yylineno);
    yylval.node = create_token_node(ENUM_RETURN,yylineno,0);
    return RETURN;
  }
}

"if" {
    if(second_flag==1){
    //fprintf(stderr,"IF %s at Line %d.\n",yytext,yylineno);
    yylval.node = create_token_node(ENUM_IF,yylineno,0);
    return IF;
  }
}

"else" {
    if(second_flag==1){
    //fprintf(stderr,"ELSE %s at Line %d.\n",yytext,yylineno);
    yylval.node = create_token_node(ENUM_ELSE,yylineno,0);
    return ELSE;
  }
}

"while" {
    if(second_flag==1){
    //fprintf(stderr,"WHILE %s at Line %d.\n",yytext,yylineno);
    yylval.node = create_token_node(ENUM_WHILE,yylineno,0);
    return WHILE;
  }
}

{INT_OCT} {
    if(second_flag==1){
    char * pEnd;
    //fprintf(stderr,"INT %lld at Line %d.\n",strtoll(yytext,&pEnd,8),yylineno);
    //yylval.type_int = strtoll(yytext,&pEnd,8);
    yylval.node = create_token_node(ENUM_INT,yylineno,SELECTOR_INT,strtoll(yytext,&pEnd,8));
    return INT;
  }
}
{INT_HEX} {
    if(second_flag==1){
    char * pEnd;
    //fprintf(stderr,"INT %lld at Line %d.\n",strtoll(yytext,&pEnd,16),yylineno);
    //yylval.type_int = strtoll(yytext,&pEnd,16);
    yylval.node = create_token_node(ENUM_INT,yylineno,SELECTOR_INT,strtoll(yytext,&pEnd,16));
    return INT;
  }
}

{FLOAT_E} {
    if(second_flag==1){
    //fprintf(stderr,"FLOAT %f at Line %d.\n",atof(yytext),yylineno);
    //yylval.type_float = atof(yytext);
    yylval.node = create_token_node(ENUM_FLOAT, yylineno, SELECTOR_FLOAT,atof(yytext));
    return FLOAT;
  }
}
{FLOAT_N} {
    if(second_flag==1){
    //fprintf(stderr,"FLOAT %f at Line %d.\n",atof(yytext),yylineno);
    //yylval.type_float = atof(yytext);
    yylval.node = create_token_node(ENUM_FLOAT, yylineno, SELECTOR_FLOAT,atof(yytext));
    return FLOAT;
  }
}


{INT_DEC} {
    if(second_flag==1){
      //fprintf(stderr,"INT %lld at Line %d.\n",atoll(yytext),yylineno);
      //yylval.type_int = atoll(yytext);
      yylval.node = create_token_node(ENUM_INT,yylineno,SELECTOR_INT,atoll(yytext));
      return INT;
    }

}


{ID} {
    if(second_flag==1){
    //fprintf(stderr,"ID %s at Line %d.\n",yytext,yylineno);
        yylval.node = create_token_node(ENUM_ID, yylineno, SELECTOR_ID,yytext);
    return ID;
  }
}

. {
    error_flag = 1;
    fprintf(stderr,"Error type A at Line %d: Mysterious characters \"%s\".\n", yylineno, yytext);

}
%%
