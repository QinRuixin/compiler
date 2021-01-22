#ifndef INTERCODE_H
#define INTERCODE_H
#include <string>

//typedef struct Operand_* Operand;

typedef struct Operand
{
    enum { VARIABLE, CONSTANT, ADDRESS, LABEL  } kind;
    union{
        int val_no;
        std::string value;
    } u;
}Operand;

typedef struct InterCode
{
    enum { ASSIGN, ADD, SUB, MUL, DIV, RETURN, LABEL, GOTO, IFGOTO, FUNCTION, PARAM,
            READ, WRITE, CALL, ARG, DEC, ADDRESS } kind;
    union {
        struct{
            Operand *right, *left;
        } assign;
        struct
        {
            Operand *result, *op1, *op2;
        } binop;
        struct{
            Operand *op;
        } sinop;
        struct{
            Operand *t1, *t2, *label;
            std::string op_rel;
        } ifop;
        struct 
        {
            Operand *op;
            int size;
        }dec;
        
    }u;
    std::string code;
}InterCode;

#endif