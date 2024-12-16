#include "RecursiveDescent.h"


ReturnValue recursiveDescent(TokensInfo* array_of_tokens)
{
    CHECK_NULL_ADDR_RET_VAL(array_of_tokens, NULL_ADDRESS_ERROR);

    int pointer = 0;

    ReturnValue value = {};
    CHECK_RETURN_VALUE(value, getExpression(array_of_tokens, &pointer));

    if(array_of_tokens->address[pointer].value.operation != '$')
    {
        value.error = SYNTAX_ERROR;
        return value;
    }

    pointer++;
    return value;
}

ReturnValue getExpression(TokensInfo* array_of_tokens, int* pointer)
{
    CHECK_NULL_ADDR_RET_VAL(array_of_tokens, NULL_ADDRESS_ERROR);
    CHECK_NULL_ADDR_RET_VAL(pointer,         NULL_ADDRESS_ERROR);

    ReturnValue value = {};
    CHECK_RETURN_VALUE(value, getTerm(array_of_tokens, pointer));

    while( array_of_tokens->address[*pointer].type == OP &&               //TODO? стоит ли делать такой пробельчик?
          (array_of_tokens->address[*pointer].value.operation == '+' ||
           array_of_tokens->address[*pointer].value.operation == '-'   ))
    {
        array_of_tokens->address[*pointer].left = value.node;
        value.node = &(array_of_tokens->address[*pointer]);
        (*pointer)++;

        ReturnValue value_two = {};
        CHECK_RETURN_VALUE(value_two, getTerm(array_of_tokens, pointer));
        if(value_two.error != NO_ERROR)
        {
            return value_two;
        }

        value.node->right = value_two.node;
    }

    return value;
}

ReturnValue getTerm(TokensInfo* array_of_tokens, int* pointer)
{
    CHECK_NULL_ADDR_RET_VAL(array_of_tokens, NULL_ADDRESS_ERROR);
    CHECK_NULL_ADDR_RET_VAL(pointer,         NULL_ADDRESS_ERROR);

    ReturnValue value = {};
    CHECK_RETURN_VALUE(value, getPower(array_of_tokens, pointer));

    while( array_of_tokens->address[*pointer].type == OP &&               //TODO? стоит ли делать такой пробельчик?
          (array_of_tokens->address[*pointer].value.operation == '*' ||
           array_of_tokens->address[*pointer].value.operation == '/'   ))
    {
        array_of_tokens->address[*pointer].left = value.node;
        value.node = &(array_of_tokens->address[*pointer]);
        (*pointer)++;

        ReturnValue value_two = {};
        CHECK_RETURN_VALUE(value_two, getPower(array_of_tokens, pointer));

        value.node->right = value_two.node;
    }

    return value;
}

ReturnValue getPower(TokensInfo* array_of_tokens, int* pointer)
{
    CHECK_NULL_ADDR_RET_VAL(array_of_tokens, NULL_ADDRESS_ERROR);
    CHECK_NULL_ADDR_RET_VAL(pointer,         NULL_ADDRESS_ERROR);

    ReturnValue value = {};
    CHECK_RETURN_VALUE(value, getPrimaryExpression(array_of_tokens, pointer));

    while(array_of_tokens->address[*pointer].type == OP &&
          array_of_tokens->address[*pointer].value.operation == '^')
    {
        array_of_tokens->address[*pointer].left = value.node;
        value.node = &(array_of_tokens->address[*pointer]);
        (*pointer)++;

        ReturnValue value_two = {};
        CHECK_RETURN_VALUE(value_two, getPrimaryExpression(array_of_tokens, pointer));

        value.node->right = value_two.node;
    }

    return value;
}

ReturnValue getPrimaryExpression(TokensInfo* array_of_tokens, int* pointer)
{
    CHECK_NULL_ADDR_RET_VAL(array_of_tokens, NULL_ADDRESS_ERROR);
    CHECK_NULL_ADDR_RET_VAL(pointer,         NULL_ADDRESS_ERROR);

    ReturnValue value = {};

    if(array_of_tokens->address[*pointer].type == OP &&
       array_of_tokens->address[*pointer].value.operation == '(')
    {
        (*pointer)++;

        CHECK_RETURN_VALUE(value, getExpression(array_of_tokens, pointer));

        if(array_of_tokens->address[*pointer].type != OP ||
           array_of_tokens->address[*pointer].value.operation != ')')
        {
            value.error = SYNTAX_ERROR;
            return value;
        }

        (*pointer)++;
    }
    else
    {
        CHECK_RETURN_VALUE(value, getVariable(array_of_tokens, pointer));
        if(value.node != NULL)
        {
            return value;
        }

        CHECK_RETURN_VALUE(value, getNumeral(array_of_tokens, pointer));
        if(value.node != NULL)
        {
            return value;
        }

        CHECK_RETURN_VALUE(value, getFuncWithOneArg(array_of_tokens, pointer));
        if(value.node != NULL)
        {
            return value;
        }

        if(value.node == NULL)
        {
            value.error = SYNTAX_ERROR;
            return value;
        }
    }

    return value;
}

ReturnValue getFuncWithOneArg(TokensInfo* array_of_tokens, int* pointer)
{
    CHECK_NULL_ADDR_RET_VAL(array_of_tokens, NULL_ADDRESS_ERROR);
    CHECK_NULL_ADDR_RET_VAL(pointer,         NULL_ADDRESS_ERROR);

    ReturnValue value = {};
    value.node = &(array_of_tokens->address[*pointer]);

    if(array_of_tokens->address[*pointer].type != OP ||
       array_of_tokens->address[*pointer].value.operation != COS)
    {
        value.error = SYNTAX_ERROR;
        return value;
    }

    (*pointer)++;

    if(array_of_tokens->address[*pointer].type != OP ||
       array_of_tokens->address[*pointer].value.operation != '(')
    {
        value.error = SYNTAX_ERROR;
        return value;
    }
    (*pointer)++;

    ReturnValue value_two = getExpression(array_of_tokens, pointer);

    value.node->left = value_two.node;

    if(array_of_tokens->address[*pointer].type != OP ||
       array_of_tokens->address[*pointer].value.operation != ')')
    {
        value.error = SYNTAX_ERROR;
        return value;
    }
    (*pointer)++;

    return value;
}

ReturnValue getVariable(TokensInfo* array_of_tokens, int* pointer)
{
    CHECK_NULL_ADDR_RET_VAL(array_of_tokens, NULL_ADDRESS_ERROR);
    CHECK_NULL_ADDR_RET_VAL(pointer,         NULL_ADDRESS_ERROR);

    ReturnValue value = {};

    if(array_of_tokens->address[*pointer].type == ID)
    {
        value.node = &(array_of_tokens->address[*pointer]);
        (*pointer)++;
    }

    return value;
}

ReturnValue getNumeral(TokensInfo* array_of_tokens, int* pointer)
{
    CHECK_NULL_ADDR_RET_VAL(array_of_tokens, NULL_ADDRESS_ERROR);
    CHECK_NULL_ADDR_RET_VAL(pointer,         NULL_ADDRESS_ERROR);

    ReturnValue value = {};

    if(array_of_tokens->address[*pointer].type == NUM)
    {
        value.node = &(array_of_tokens->address[*pointer]);
        (*pointer)++;
    }

    return value;
}
