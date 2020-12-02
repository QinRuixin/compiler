%{
    #include "lex.yy.c"
    #include<stdarg.h>
    #define YY_USER_ACTION yylloc.first_line = yylloc.last_time = yylineno;

    void yyerror(const char* msg);
    int yyparse(void);
    extern int syntax_error_flag;

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
    //tree_node* create_node(NODE_TYPE enum_type, int lineno);
    tree_node* create_node(NODE_TYPE enum_type, int lineno,int childnum,...);
    void traverse(struct tree_node* root);
%}

/* TYPE */
%union {
    struct tree_node* node;
}

/* tokens */
%token <node> INT FLOAT ID SEMI COMMA ASSIGNOP RELOP PLUS MINUS STAR DIV AND OR DOT NOT TYPE LP RP LB RB LC RC STRUCT RETURN IF ELSE WHILE

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

%type <node> Program ExtDefList ExtDef ExtDecList Specifier StructSpecifier OptTag Tag VarDec FunDec VarList ParamDec CompSt StmtList Stmt DefList Def DecList Dec Exp Args

%%
Program : ExtDefList {$$=create_node(ENUM_Program,@1.first_line,1,$1); if(syntax_error_flag == 0){traverse($$);} }
    ;
ExtDefList : ExtDef ExtDefList {$$=create_node(ENUM_ExtDefList,@1.first_line,0);}
    | /* Epsl */
    ;
ExtDef : Specifier ExtDecList SEMI
    | Specifier SEMI
    | Specifier FunDec CompSt
    | error SEMI
    ;
ExtDecList : VarDec
    | VarDec COMMA ExtDecList
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
    | error RP
    ;
VarList : ParamDec COMMA VarList
    | ParamDec
    ;
ParamDec : Specifier VarDec
    ;

CompSt : LC DefList StmtList RC
    | LC error RC
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
    | error SEMI
    ;

DefList : Def DefList
    | /* Epsl */
    ;
Def : Specifier DecList SEMI
    | error SEMI
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
    | Exp LB error RB
    ;
Args : Exp COMMA Args
    | Exp
    ;

%%

/*
tree_node* create_node(NODE_TYPE enum_type, int lineno){
    tree_node* cur_node = (tree_node*)malloc(sizeof(tree_node));
    cur_node->node_type = enum_type;
    cur_node->line_no = lineno;
    cur_node->child_num = 0;
    cur_node->int_val = 0ll;
    cur_node->float_val = 0.0;
}
*/
tree_node* create_node(NODE_TYPE enum_type, int lineno,int childnum,...){
    tree_node* cur_node = (tree_node*)malloc(sizeof(tree_node));
    cur_node->node_type = enum_type;
    cur_node->line_no = lineno;
    cur_node->child_num = 0;
    cur_node->int_val = 0ll;
    cur_node->float_val = 0.0;
    va_list arg_ptr;             //arg_ptr
    va_start(arg_ptr,childnum);   // point to arg after childnum
    for(int i = 0; i< childnum; ++i){
        cur_node->child_node[cur_node->child_num++] = va_arg(arg_ptr, tree_node*);
    }
}

void traverse(tree_node* root){
    // todo INT FLOAT etc
    fprintf(stderr, "%s(%d)\n", type_name[root->node_type], root->line_no);
    for(int i = 0; i < root->child_num; ++i){
        if(root->child_node[i]!=NULL){
            fprintf(stderr, "  ");
            traverse(root->child_node[i]);
        }
    }
}

void yyerror(const char* msg){
    syntax_error_flag = 1;
    fprintf(stderr, "Error type B at Line %d: %s\n", yylineno, msg);
}