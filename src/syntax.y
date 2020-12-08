%{
    #include "lex.yy.c"
    #include <stdarg.h>
    void yyerror(const char* msg);
    int yyparse(void);
    extern int syntax_error_flag;


%}

/* TYPE */
%union {
    struct tree_node* node;
}

/* tokens */
%token <node> INT FLOAT ID SEMI COMMA ASSIGNOP RELOP PLUS MINUS STAR DIV AND OR DOT NOT TYPE LP RP LB RB LC RC STRUCT RETURN IF ELSE WHILE

%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE
%right ASSIGNOP
%left OR
%left AND
%left RELOP
%left PLUS MINUS
%left STAR DIV
%right UMINUS
%right NOT
%left DOT LP RP LB RB

%type <node> Program ExtDefList ExtDef ExtDecList Specifier StructSpecifier OptTag Tag VarDec FunDec VarList ParamDec CompSt StmtList Stmt DefList Def DecList Dec Exp Args

%%
Program : ExtDefList {$$=create_node(ENUM_Program,@$.first_line,1,$1); if(syntax_error_flag == 0){traverse($$,0);} }
    ;
ExtDefList : ExtDef ExtDefList {$$=create_node(ENUM_ExtDefList,@$.first_line,2,$1,$2);}
    | /* Epsl */ {$$=NULL;}
    ;
ExtDef : Specifier ExtDecList SEMI {$$=create_node(ENUM_ExtDef,@$.first_line,3,$1,$2,$3);}
    | Specifier SEMI {$$=create_node(ENUM_ExtDef,@$.first_line,2,$1,$2);}
    | Specifier FunDec CompSt {$$=create_node(ENUM_ExtDef,@$.first_line,3,$1,$2,$3);}
    | error SEMI {$$=create_node(ENUM_ExtDef,@$.first_line,0);}
    ;
ExtDecList : VarDec {$$=create_node(ENUM_ExtDecList,@$.first_line,1,$1);}
    | VarDec COMMA ExtDecList {$$=create_node(ENUM_ExtDecList,@$.first_line,3,$1,$2,$3);}
    ;

Specifier : TYPE {$$=create_node(ENUM_Specifier,@$.first_line,1,$1);}
    | StructSpecifier  {$$=create_node(ENUM_Specifier,@$.first_line,1,$1);}
    ;
StructSpecifier : STRUCT OptTag LC DefList RC {$$=create_node(ENUM_StructSpecifier,@$.first_line,5,$1,$2,$3,$4,$5);}
    | STRUCT Tag {$$=create_node(ENUM_StructSpecifier,@$.first_line,2,$1,$2);}
    ;
OptTag : ID {$$=create_node(ENUM_OptTag,@$.first_line,1,$1);}
    | /* Epsl */ {$$=NULL;}
    ;
Tag : ID {$$=create_node(ENUM_Tag,@$.first_line,1,$1);}
    ;

VarDec : ID {$$=create_node(ENUM_VarDec,@$.first_line,1,$1);}
    | VarDec LB INT RB {$$=create_node(ENUM_VarDec,@$.first_line,4,$1,$2,$3,$4);}
    ;
FunDec : ID LP VarList RP {$$=create_node(ENUM_FunDec,@$.first_line,4,$1,$2,$3,$4);}
    | ID LP RP {$$=create_node(ENUM_FunDec,@$.first_line,3,$1,$2,$3);}
    | error RP {$$=create_node(ENUM_FunDec,@$.first_line,0);}
    ;
VarList : ParamDec COMMA VarList {$$=create_node(ENUM_VarList,@$.first_line,3,$1,$2,$3);}
    | ParamDec {$$=create_node(ENUM_VarList,@$.first_line,1,$1);}
    ;
ParamDec : Specifier VarDec {$$=create_node(ENUM_ParamDec,@$.first_line,2,$1,$2);}
    ;

CompSt : LC DefList StmtList RC {$$=create_node(ENUM_CompSt,@$.first_line,4,$1,$2,$3,$4);}
    | LC error RC {$$=create_node(ENUM_CompSt,@$.first_line,0);}
    ;
StmtList : Stmt StmtList {$$=create_node(ENUM_StmtList,@$.first_line,2,$1,$2);}
    | /* Epsl */ {$$=NULL;}
    ;
Stmt : Exp SEMI {$$=create_node(ENUM_Stmt,@$.first_line,2,$1,$2);}
    | CompSt {$$=create_node(ENUM_Stmt,@$.first_line,1,$1);}
    | RETURN Exp SEMI {$$=create_node(ENUM_Stmt,@$.first_line,3,$1,$2,$3);}
    | IF LP Exp RP Stmt %prec LOWER_THAN_ELSE {$$=create_node(ENUM_Stmt,@$.first_line,5,$1,$2,$3,$4,$5);}
    | IF LP Exp RP Stmt ELSE Stmt {$$=create_node(ENUM_Stmt,@$.first_line,7,$1,$2,$3,$4,$5,$6,$7);}
    | WHILE LP Exp RP Stmt {$$=create_node(ENUM_Stmt,@$.first_line,5,$1,$2,$3,$4,$5);}
    | error SEMI {$$=create_node(ENUM_Stmt,@$.first_line,0);}
    ;

DefList : Def DefList {$$=create_node(ENUM_DefList,@$.first_line,2,$1,$2);}
    | /* Epsl */ {$$=NULL;}
    ;
Def : Specifier DecList SEMI {$$=create_node(ENUM_Def,@$.first_line,3,$1,$2,$3);}
    | error SEMI {$$=create_node(ENUM_Def,@$.first_line,0);}
    ;
DecList : Dec {$$=create_node(ENUM_DecList,@$.first_line,1,$1);}
    | Dec COMMA DecList {$$=create_node(ENUM_DecList,@$.first_line,3,$1,$2,$3);}
    ;
Dec : VarDec {$$=create_node(ENUM_Dec,@$.first_line,1,$1);}
    | VarDec ASSIGNOP Exp {$$=create_node(ENUM_Dec,@$.first_line,3,$1,$2,$3);}
    ;

Exp : Exp ASSIGNOP Exp {$$=create_node(ENUM_Exp,@$.first_line,3,$1,$2,$3);}
    | Exp AND Exp {$$=create_node(ENUM_Exp,@$.first_line,3,$1,$2,$3);}
    | Exp OR Exp {$$=create_node(ENUM_Exp,@$.first_line,3,$1,$2,$3);}
    | Exp RELOP Exp {$$=create_node(ENUM_Exp,@$.first_line,3,$1,$2,$3);}
    | Exp PLUS Exp {$$=create_node(ENUM_Exp,@$.first_line,3,$1,$2,$3);}
    | Exp MINUS Exp {$$=create_node(ENUM_Exp,@$.first_line,3,$1,$2,$3);}
    | Exp STAR Exp {$$=create_node(ENUM_Exp,@$.first_line,3,$1,$2,$3);}
    | Exp DIV Exp {$$=create_node(ENUM_Exp,@$.first_line,3,$1,$2,$3);}
    | LP Exp RP {$$=create_node(ENUM_Exp,@$.first_line,3,$1,$2,$3);}
    | MINUS Exp %prec UMINUS {$$=create_node(ENUM_Exp,@$.first_line,2,$1,$2);}
    | NOT Exp {$$=create_node(ENUM_Exp,@$.first_line,2,$1,$2);}
    | ID LP Args RP {$$=create_node(ENUM_Exp,@$.first_line,4,$1,$2,$3,$4);}
    | ID LP RP {$$=create_node(ENUM_Exp,@$.first_line,3,$1,$2,$3);}
    | Exp LB Exp RB {$$=create_node(ENUM_Exp,@$.first_line,4,$1,$2,$3,$4);}
    | Exp DOT ID {$$=create_node(ENUM_Exp,@$.first_line,3,$1,$2,$3);}
    | ID {$$=create_node(ENUM_Exp,@$.first_line,1,$1);}
    | INT {$$=create_node(ENUM_Exp,@$.first_line,1,$1);}
    | FLOAT {$$=create_node(ENUM_Exp,@$.first_line,1,$1);}
    | Exp LB error RB {$$=create_node(ENUM_Exp,@$.first_line,0);}
    ;
Args : Exp COMMA Args {$$=create_node(ENUM_Args,@$.first_line,3,$1,$2,$3);}
    | Exp {$$=create_node(ENUM_Args,@$.first_line,1,$1);}
    ;

%%
void yyerror(const char* msg){
    syntax_error_flag = 1;
    fprintf(stderr,"Error type B at Line %d: %s.\n",yylineno,msg);
}