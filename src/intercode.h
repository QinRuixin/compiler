#include <string>

typedef struct Operand_* Operand;
struct Operand_{
    enum { VARIABLE, CONSTANT, ADDRESS  } kind;
    union{
        int val_no;
        int value;
    } u;
};

struct InterCode
{
    enum { ASSIGN, ADD, SUB, MUL } kind;
    union {
        struct{
            Operand right, left;
        } assign;
        struct
        {
            Operand result, op1, op2;
        } binop;
        
    }u;
    std::string code;
};

