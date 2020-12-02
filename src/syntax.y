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
    void traverse(struct tree_node* root,int cur_deep);
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
Program : ExtDefList {$$=create_node(ENUM_Program,@$.first_line,1,$1); if(syntax_error_flag == 0){traverse($$,0);} }
    ;
ExtDefList : ExtDef ExtDefList {$$=create_node(ENUM_ExtDefList,@$.first_line,2,$1,$2);}
    | /* Epsl */ {$$=create_node(ENUM_ExtDefList,@$.first_line,0);}
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
    | /* Epsl */ {$$=create_node(ENUM_OptTag,@$.first_line,0);}
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
    | /* Epsl */ {$$=create_node(ENUM_StmtList,@$.first_line,0);}
    ;
Stmt : Exp SEMI {$$=create_node(ENUM_Stmt,@$.first_line,2,$1,$2);}
    | CompSt {$$=create_node(ENUM_Stmt,@$.first_line,1,$1);}
    | RETURN Exp SEMI {$$=create_node(ENUM_Stmt,@$.first_line,3,$1,$2,$3);}
    | IF LP Exp RP Stmt %prec LOWER_THAN_ELSE {$$=create_node(ENUM_Stmt,@$.first_line,5,$1,$2,$3,$4,$5);}
    | IF LP Exp RP Stmt ELSE Stmt {$$=create_node(ENUM_Stmt,@$.first_line,7,$1,$2,$3,$4,$5,$6,$7);}
    | WHILE LP Exp RP Stmt {$$=create_node(ENUM_Stmt,@$.first_line,5,$1,$2,$3,$4,$5);}
    | error SEMI {$$=create_node(ENUM_Stmt,@$.first_line,0);}
    ;

DefList : Def DefList {$$=create_node(ENUM_DefList,@1.first_line,2,$1,$2);}
    | /* Epsl */ {$$=create_node(ENUM_DefList,@$.first_line,0);}
    ;
Def : Specifier DecList SEMI {$$=create_node(ENUM_Def,@1.first_line,3,$1,$2,$3);}
    | error SEMI {$$=create_node(ENUM_Def,@1.first_line,0);}
    ;
DecList : Dec {$$=create_node(ENUM_DecList,@1.first_line,1,$1);}
    | Dec COMMA DecList {$$=create_node(ENUM_DecList,@1.first_line,3,$1,$2,$3);}
    ;
Dec : VarDec {$$=create_node(ENUM_Dec,@1.first_line,1,$1);}
    | VarDec ASSIGNOP Exp {$$=create_node(ENUM_Dec,@1.first_line,3,$1,$2,$3);}
    ;

Exp : Exp ASSIGNOP Exp {$$=create_node(ENUM_Exp,@1.first_line,3,$1,$2,$3);}
    | Exp AND Exp {$$=create_node(ENUM_Exp,@1.first_line,3,$1,$2,$3);}
    | Exp OR Exp {$$=create_node(ENUM_Exp,@1.first_line,3,$1,$2,$3);}
    | Exp RELOP Exp {$$=create_node(ENUM_Exp,@1.first_line,3,$1,$2,$3);}
    | Exp PLUS Exp {$$=create_node(ENUM_Exp,@1.first_line,3,$1,$2,$3);}
    | Exp MINUS Exp {$$=create_node(ENUM_Exp,@1.first_line,3,$1,$2,$3);}
    | Exp STAR Exp {$$=create_node(ENUM_Exp,@1.first_line,3,$1,$2,$3);}
    | Exp DIV Exp {$$=create_node(ENUM_Exp,@1.first_line,3,$1,$2,$3);}
    | LP Exp RP {$$=create_node(ENUM_Exp,@1.first_line,3,$1,$2,$3);}
    | MINUS Exp {$$=create_node(ENUM_Exp,@1.first_line,2,$1,$2);}
    | NOT Exp {$$=create_node(ENUM_Exp,@1.first_line,2,$1,$2);}
    | ID LP Args RP {$$=create_node(ENUM_Exp,@1.first_line,4,$1,$2,$3,$4);}
    | ID LP RP {$$=create_node(ENUM_Exp,@1.first_line,3,$1,$2,$3);}
    | Exp LB Exp RB {$$=create_node(ENUM_Exp,@1.first_line,4,$1,$2,$3,$4);}
    | Exp DOT ID {$$=create_node(ENUM_Exp,@1.first_line,3,$1,$2,$3);}
    | ID {$$=create_node(ENUM_Exp,@1.first_line,1,$1);}
    | INT {$$=create_node(ENUM_Exp,@1.first_line,1,$1);}
    | FLOAT {$$=create_node(ENUM_Exp,@1.first_line,1,$1);}
    | Exp LB error RB {$$=create_node(ENUM_Exp,@1.first_line,0);}
    ;
Args : Exp COMMA Args {$$=create_node(ENUM_Args,@1.first_line,3,$1,$2,$3);}
    | Exp {$$=create_node(ENUM_Args,@1.first_line,1,$1);}
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



void yyerror(const char* msg){
    syntax_error_flag = 1;
    fprintf(stderr, "Error type B at Line %d: %s\n", yylineno, msg);
}