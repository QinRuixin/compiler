#ifndef INTERCODE_H
#define INTERCODE_H
#include <string>

//typedef struct Operand_* Operand;

typedef struct Operand
{
    enum { VARIABLE, CONSTANT, ADDRESS  } kind;
    union{
        int val_no;
        std::string value;
    } u;
}Operand;

typedef struct InterCode
{
    enum { ASSIGN, ADD, SUB, MUL } kind;
    union {
        struct{
            Operand *right, *left;
        } assign;
        struct
        {
            Operand *result, *op1, *op2;
        } binop;
        
    }u;
    std::string code;
}InterCode;

#endif