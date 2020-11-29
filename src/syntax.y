%{
    #include "lex.yy.c"
    #define YY_USER_ACTION yylloc.first_line = yylloc.last_time = yylineno;

    void yyerror(const char* msg);
    int yyparse(void);
%}

/* TYPE */
%union {
    long long int type_int;
    double type_float;
    char* s;
}

/* tokens */
%token <type_int> INT
%token <type_float> FLOAT
%token ID SEMI COMMA ASSIGNOP RELOP PLUS MINUS STAR DIV AND OR DOT 
NOT TYPE LP RP LB RB LC RC STRUCT RETURN IF ELSE WHILE

%right ASSIGNOP
%left OR
%left AND
%left RELOP
%left PLUS MINUS
%left STAR DIV
%right NOT
%left DOT LP RP LB RB
%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE

%%
Program : ExtDefList {fprintf(stderr, "Program (%d)",yylineno);}
    ;
ExtDefList : ExtDef ExtDefList
    | /* Epsl */
    ;
ExtDef : Specifier ExtDefList SEMI
    | Specifier SEMI
    | Specifier FunDec CompSt
    ;
ExtDefList : VarDec
    | VarDec COMMA ExtDefList
    ;

Specifier : TYPE
    | StructSpecifier
    ;
StructSpecifier : STRUCT OptTag LC DefList RC
    | STRUCT Tag
    ;
OptTag : ID
    | /* Epsl */
    ;
Tag : ID
    ;

VarDec : ID
    | VarDec LB INT RB
    ;
FunDec : ID LP VarList RP
    | ID LP RP
    ;
VarList : ParamDec COMMA VarList
    | ParamDec
    ;
ParamDec : Specifier VarDec
    ;

CompSt : LC DefList StmtList RC
    ;
StmtList : Stmt StmtList
    | /* Epsl */
    ;
Stmt : Exp SEMI
    | CompSt
    | RETURN Exp SEMI
    | IF LP Exp RP Stmt %prec LOWER_THAN_ELSE
    | IF LP Exp RP Stmt ELSE Stmt
    | WHILE LP Exp RP Stmt
    ;

DefList : Def DefList
    | /* Epsl */
    ;
Def : Specifier DecList SEMI
    ;
DecList : Dec
    | Dec COMMA DecList
    ;
Dec : VarDec
    | VarDec ASSIGNOP Exp
    ;

Exp : Exp ASSIGNOP Exp
    | Exp AND Exp
    | Exp OR Exp
    | Exp RELOP Exp
    | Exp PLUS Exp
    | Exp MINUS Exp
    | Exp STAR Exp
    | Exp DIV Exp
    | LP Exp RP
    | MINUS Exp
    | NOT Exp
    | ID LP Args RP
    | ID LP RP
    | Exp LB Exp RB
    | Exp DOT ID
    | ID
    | INT
    | FLOAT
    ;
Args : Exp COMMA Args
    | Exp
    ;

%%
