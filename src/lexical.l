%option yylineno
%{
    #include "syntax.tab.h"
    #include<stdio.h>
    #include <stdarg.h>
    extern int error_flag;
    extern int second_flag;
    int yycolumn = 1;
    #define YY_USER_ACTION yylloc.first_line = yylloc.last_line = yylineno; yylloc.first_column = yycolumn; yylloc.last_column = yycolumn + yyleng -1; yycolumn += yyleng;


    typedef enum NT{
        ENUM_INT, ENUM_FLOAT, ENUM_ID, ENUM_SEMI, ENUM_COMMA, 
        ENUM_ASSIGNOP, ENUM_RELOP, ENUM_PLUS, ENUM_MINUS, ENUM_STAR, ENUM_DIV, 
        ENUM_AND, ENUM_OR, ENUM_DOT, ENUM_NOT, ENUM_TYPE, 
        ENUM_LP, ENUM_RP, ENUM_LB, ENUM_RB, ENUM_LC, ENUM_RC, 
        ENUM_STRUCT, ENUM_RETURN, ENUM_IF, ENUM_ELSE, ENUM_WHILE,

        ENUM_Epsilon,

        ENUM_Program, ENUM_ExtDefList, ENUM_ExtDef, ENUM_ExtDecList,

        ENUM_Specifier, ENUM_StructSpecifier, ENUM_OptTag, ENUM_Tag,

        ENUM_VarDec, ENUM_FunDec, ENUM_VarList, ENUM_ParamDec,

        ENUM_CompSt, ENUM_StmtList, ENUM_Stmt,

        ENUM_DefList, ENUM_Def, ENUM_DecList, ENUM_Dec,

        ENUM_Exp, ENUM_Args
    } NODE_TYPE;

    const char* type_name[] = {
        "INT", "FLOAT", "ID", "SEMI", "COMMA", 
        "ASSIGNOP", "RELOP", "PLUS", "MINUS", "STAR", "DIV",
        "AND", "OR", "DOT", "NOT", "TYPE",
        "LP", "RP", "LB", "RB", "LC", "RC",
        "STRUCT", "RETURN", "IF", "ELSE", "WHILE",

        "Epsilon",

        "Program", "ExtDefList", "ExtDef", "ExtDecList",

        "Specifier", "StructSpecifier", "OptTag", "Tag",

        "VarDec", "FunDec", "VarList", "ParamDec",

        "CompSt", "StmtList", "Stmt",

        "DefList", "Def", "DecList", "Dec",

        "Exp", "Args"
    };
    //struct tree_node;
    typedef struct tree_node{
        NODE_TYPE node_type;
        //char node_name[32+1];
        struct tree_node* child_node[10];
        int child_num;
        int line_no;
        long long int int_val;
        double float_val;
    } tree_node;

    tree_node* test_root;
    tree_node* create_node(NODE_TYPE enum_type, int lineno,int childnum,...);
    void traverse(struct tree_node* root,int cur_deep);

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
    return SEMI;
  }
}

"," {
    if(second_flag==1){
    //fprintf(stderr,"COMMA %s at Line %d.\n",yytext,yylineno);
    return COMMA;
  }
}

"=" {
    if(second_flag==1){
    //fprintf(stderr,"ASSIGNOP %s at Line %d.\n",yytext,yylineno);
    return ASSIGNOP; 
  }
}

">"|"<"|">="|"<="|"=="|"!=" {
    if(second_flag==1){
    //fprintf(stderr,"RELOP %s at Line %d.\n",yytext,yylineno);
    return RELOP;
  }
}

"+" {
    if(second_flag==1){
    //fprintf(stderr,"PLUS %s at Line %d.\n",yytext,yylineno);
    return PLUS;
  }
}

"-" {
    if(second_flag==1){
    //fprintf(stderr,"MINUS %s at Line %d.\n",yytext,yylineno);
    return MINUS;
  }
}

"*" {
    if(second_flag==1){
    //fprintf(stderr,"STAR %s at Line %d.\n",yytext,yylineno);
    return STAR;
  }
}

"/" {
    if(second_flag==1){
    //fprintf(stderr,"DIV %s at Line %d.\n",yytext,yylineno);
    return DIV;
  }
}

"&&" {
    if(second_flag==1){
    //fprintf(stderr,"AND %s at Line %d.\n",yytext,yylineno);
    return AND;
  }
}

"||" {
    if(second_flag==1){
    //fprintf(stderr,"OR %s at Line %d.\n",yytext,yylineno);
    return OR;
  }
}

"." {
    if(second_flag==1){
    //fprintf(stderr,"DOT %s at Line %d.\n",yytext,yylineno);
    return DOT;
  }
}

"!" {
    if(second_flag==1){
    //fprintf(stderr,"NOT %s at Line %d.\n",yytext,yylineno);
    return NOT;
  }
}

"int"|"float" {
    if(second_flag==1){
    //fprintf(stderr,"TYPE %s at Line %d.\n",yytext,yylineno);
    return TYPE;
  }
}

"(" {
    if(second_flag==1){
    //fprintf(stderr,"LP %s at Line %d.\n",yytext,yylineno);
    return LP;
  }
}
")" {
    if(second_flag==1){
    //fprintf(stderr,"RP %s at Line %d.\n",yytext,yylineno);
    return RP;
  }
}

"[" {
    if(second_flag==1){
    //fprintf(stderr,"LB %s at Line %d.\n",yytext,yylineno);
    return LB;
  }
}

"]" {
    if(second_flag==1){
    //fprintf(stderr,"RB %s at Line %d.\n",yytext,yylineno);
    return RB;
  }
}

"{" {
    if(second_flag==1){
    //fprintf(stderr,"LC %s at Line %d.\n",yytext,yylineno);
    return LC;
  }
}

"}" {
    if(second_flag==1){
    //fprintf(stderr,"RC %s at Line %d.\n",yytext,yylineno);
    return RC;
  }
}

"struct" {
    if(second_flag==1){
    //fprintf(stderr,"STRUCT %s at Line %d.\n",yytext,yylineno);
    return STRUCT;
  }
}
"return" {
    if(second_flag==1){
    //fprintf(stderr,"RETURN %s at Line %d.\n",yytext,yylineno);
    return RETURN;
  }
}

"if" {
    if(second_flag==1){
    //fprintf(stderr,"IF %s at Line %d.\n",yytext,yylineno);
    return IF;
  }
}

"else" {
    if(second_flag==1){
    //fprintf(stderr,"ELSE %s at Line %d.\n",yytext,yylineno);
    return ELSE;
  }
}

"while" {
    if(second_flag==1){
    //fprintf(stderr,"WHILE %s at Line %d.\n",yytext,yylineno);
    return WHILE;
  }
}

{INT_OCT} {
    if(second_flag==1){
    char * pEnd;
    //fprintf(stderr,"INT %lld at Line %d.\n",strtoll(yytext,&pEnd,8),yylineno);
    //yylval.type_int = strtoll(yytext,&pEnd,8);
    return INT;
  }
}
{INT_HEX} {
    if(second_flag==1){
    char * pEnd;
    //fprintf(stderr,"INT %lld at Line %d.\n",strtoll(yytext,&pEnd,16),yylineno);
    //yylval.type_int = strtoll(yytext,&pEnd,16);
    return INT;
  }
}

{FLOAT_E} {
    if(second_flag==1){
    //fprintf(stderr,"FLOAT %f at Line %d.\n",atof(yytext),yylineno);
    //yylval.type_float = atof(yytext);
    return FLOAT;
  }
}
{FLOAT_N} {
    if(second_flag==1){
    //fprintf(stderr,"FLOAT %f at Line %d.\n",atof(yytext),yylineno);
    //yylval.type_float = atof(yytext);
    return FLOAT;
  }
}


{INT_DEC} {
    if(second_flag==1){
      //fprintf(stderr,"INT %lld at Line %d.\n",atoll(yytext),yylineno);
      //yylval.type_int = atoll(yytext);
      return INT;
    }

}


{ID} {
    if(second_flag==1){
    //fprintf(stderr,"ID %s at Line %d.\n",yytext,yylineno);
    return ID;
  }
}

. {
    error_flag = 1;
    fprintf(stderr,"Error type A at Line %d: Mysterious characters \"%s\".\n", yylineno, yytext);

}
%%

tree_node* create_node(NODE_TYPE enum_type, int lineno,int childnum,...){
    tree_node* cur_node = (tree_node*)malloc(sizeof(tree_node));
    cur_node->node_type = enum_type;
    cur_node->line_no = lineno;
    cur_node->child_num = 0;
    cur_node->int_val = 0ll;
    cur_node->float_val = 0.0;
    if(childnum==0){
        return cur_node;
    }
    va_list arg_ptr;             //arg_ptr
    va_start(arg_ptr,childnum);   // point to arg after childnum
    for(int i = 0; i< childnum; ++i){
        cur_node->child_node[cur_node->child_num++] = va_arg(arg_ptr, tree_node*);
    }
    return cur_node;
}

void traverse(tree_node* root,int cur_deep){
    // todo INT FLOAT etc
    for(int d = 0; d< cur_deep;++d){
        fprintf(stderr, "  ");        
    }
    fprintf(stderr, "%s(%d)\n", type_name[root->node_type], root->line_no);
    for(int i = 0; i < root->child_num; ++i){
        if(root->child_node[i]!=NULL){
            traverse(root->child_node[i],cur_deep+1);
        }
    }
}