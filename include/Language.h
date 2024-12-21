#ifndef LANGUAGE_H
#define LANGUAGE_H


#include <stdio.h>
#include <stdlib.h>

enum ErrorNumbers
{
    NO_ERROR            = 0,
    NULL_ADDRESS_ERROR  = 1,
    CALLOC_ERROR        = 2,
    NUMBER_OF_ARG_ERROR = 3,
    OPEN_ERROR          = 4,
    FSEEK_ERROR         = 5,
    FTELL_ERROR         = 6,
    SYNTAX_ERROR        = 7,
    TYPE_ERROR          = 8,
    SIZE_ERROR          = 9,
};

enum Operations
{
    POISON = 0,
    ADD    = '+',
    SUB    = '-',
    MUL    = '*',
    DIV    = '/',
    POW    = '^',
    COS    = 'c',
    L_SK   = '(',
    R_SK   = ')',
    LF_SK  = '{',
    RF_SK  = '}',
    EQUAL  = '=',
    PRINTF = 'p',
    IF     = 'i',
    WHILE  = 'w',
    EOP    = ';',
    END    = '$'
};

enum NodeType
{
    POISON_TYPE = 0,
    NUM         = 1,
    ID          = 2,
    OP          = 3
};

struct IdInfo
{
    char* id;
    int length;
    int number;
};

union ValueType
{
    double numeral;
    IdInfo id_info;
    Operations operation;
};

struct Node
{
    NodeType type;
    ValueType value;
    Node* left;
    Node* right;
};

struct BufferInfo
{
    char* buffer;
    int pointer;
};

struct IdTableInfo
{
    IdInfo* cell;
    int size;
    int capacity;
};

#define CHECK_NULL_ADDR_ERROR(variable, ERROR_NUMBER)     \
    if(variable == NULL)                                  \
    {                                                     \
        return ERROR_NUMBER;                              \
    }


#endif // LANGUAGE_H
