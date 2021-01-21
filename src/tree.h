# ifndef TREE_H_INCLUDED
# define TREE_H_INCLUDED
# define SELECTOR_NONE 0
# define SELECTOR_INT 1
# define SELECTOR_FLOAT 2
# define SELECTOR_TYPE 3
# define SELECTOR_ID 4
# define SELECTOR_RELOP 5

typedef enum NT{
    ENUM_INT, ENUM_FLOAT, ENUM_ID, ENUM_SEMI, ENUM_COMMA, 
    ENUM_ASSIGNOP, ENUM_RELOP, ENUM_PLUS, ENUM_MINUS, ENUM_STAR, ENUM_DIV, 
    ENUM_AND, ENUM_OR, ENUM_DOT, ENUM_NOT, ENUM_TYPE, //15
    ENUM_LP, ENUM_RP, ENUM_LB, ENUM_RB, ENUM_LC, ENUM_RC, 
    ENUM_STRUCT, ENUM_RETURN, ENUM_IF, ENUM_ELSE, ENUM_WHILE,

    ENUM_Epsilon,

    ENUM_Program, ENUM_ExtDefList, ENUM_ExtDef, ENUM_ExtDecList,

    ENUM_Specifier, ENUM_StructSpecifier, ENUM_OptTag, ENUM_Tag,

// 36
    ENUM_VarDec, ENUM_FunDec, ENUM_VarList, ENUM_ParamDec,

    ENUM_CompSt, ENUM_StmtList, ENUM_Stmt,

    ENUM_DefList, ENUM_Def, ENUM_DecList, ENUM_Dec,

    ENUM_Exp, ENUM_Args
} NODE_TYPE;


//struct tree_node_st;
#pragma pack (1) 
typedef struct tree_node_st{
    NODE_TYPE node_type;
    char node_name[32+1];
    struct tree_node_st* child_node[10];
    int child_num;
    int line_no;
    long long int int_val;
    double float_val;
} tree_node;
#pragma pack ()

#endif

tree_node* create_node(NODE_TYPE enum_type, int lineno,int childnum,...);
tree_node* create_token_node(NODE_TYPE enum_type, int lineno,int val_selector,...);
void traverse(tree_node* root,int cur_deep);

