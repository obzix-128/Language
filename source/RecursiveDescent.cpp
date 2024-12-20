#include "RecursiveDescent.h"


ReturnValue recursiveDescent(TokensInfo* array_of_tokens)
{
    CHECK_NULL_ADDR_RET_VAL(array_of_tokens, NULL_ADDRESS_ERROR);

    int pointer = 0;

    ReturnValue value = {};

    CHECK_RETURN_VALUE(value, getOperation(array_of_tokens, &pointer));

    if(array_of_tokens->address[pointer].value.operation != '$')
    {
        fprintf(stderr, "Not detected $\n");
        value.error = SYNTAX_ERROR;
        return value;
    }

    pointer++;
    return value;
}

ReturnValue getOperation(TokensInfo* array_of_tokens, int* pointer)
{
    CHECK_NULL_ADDR_RET_VAL(array_of_tokens, NULL_ADDRESS_ERROR);
    CHECK_NULL_ADDR_RET_VAL(pointer,         NULL_ADDRESS_ERROR);

    ReturnValue value    = {};
    ReturnValue l_branch = {};

    CHECK_RETURN_VALUE(l_branch, getAssignment(array_of_tokens, pointer));
    if(l_branch.node == NULL)
    {
        CHECK_RETURN_VALUE(l_branch, getFuncWithOneArg(array_of_tokens, pointer));
        if(l_branch.node == NULL)
        {
            return value;
        }
    }

    ReturnValue r_branch = {};

    if(array_of_tokens->address[*pointer].value.operation == ';')
    {
        value.node = &(array_of_tokens->address[*pointer]);
        (*pointer)++;

        CHECK_RETURN_VALUE(r_branch, getOperation(array_of_tokens, pointer));

        value.node->left  = l_branch.node;
        value.node->right = r_branch.node;
    }
    else
    {
        fprintf(stderr, "the ';' character was expected, but the '%c' character was received\n",
                array_of_tokens->address[*pointer].value.operation                               );
        value.error = SYNTAX_ERROR;
        return value;
    }

    return value;
}

ReturnValue getAssignment(TokensInfo* array_of_tokens, int* pointer)
{
    CHECK_NULL_ADDR_RET_VAL(array_of_tokens, NULL_ADDRESS_ERROR);
    CHECK_NULL_ADDR_RET_VAL(pointer,         NULL_ADDRESS_ERROR);

    ReturnValue result = {};

    ReturnValue l_value = {};
    ReturnValue r_value = {};

    CHECK_RETURN_VALUE(l_value, getVariable(array_of_tokens, pointer));

    if(l_value.node == NULL)
    {
        return result;
    }

    if(array_of_tokens->address[*pointer].type == OP &&
       array_of_tokens->address[*pointer].value.operation == '=')
    {
        array_of_tokens->address[*pointer].left = l_value.node;
        result.node = &(array_of_tokens->address[*pointer]);
        (*pointer)++;

        CHECK_RETURN_VALUE(r_value, getExpression(array_of_tokens, pointer));
        result.node->right = r_value.node;
    }
    else
    {
        (*pointer) -= 1;
        return result;
    }

    return result;
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
            fprintf(stderr, "the ')' character was expected, but the '%c' character was received\n",
                    array_of_tokens->address[*pointer].value.operation                               );
            value.error = SYNTAX_ERROR;
            return value;
        }

        (*pointer)++;
    }
    else
    {
        switch(array_of_tokens->address[*pointer].type)
        {
            case NUM:
            {
                CHECK_RETURN_VALUE(value, getNumeral(array_of_tokens, pointer));
                break;
            }
            case ID:
            {
                CHECK_RETURN_VALUE(value, getVariable(array_of_tokens, pointer));
                break;
            }
            case OP:
            {
                CHECK_RETURN_VALUE(value, getFuncWithOneArg(array_of_tokens, pointer));
                break;
            }
            case POISON_TYPE:
            {
                value.error = SYNTAX_ERROR;
                return value;
                break;
            }
            default:
            {
                value.error = SYNTAX_ERROR;
                return value;
                break;
            }
        }
    }

    return value;
}

ReturnValue getFuncWithOneArg(TokensInfo* array_of_tokens, int* pointer)
{
    CHECK_NULL_ADDR_RET_VAL(array_of_tokens, NULL_ADDRESS_ERROR);
    CHECK_NULL_ADDR_RET_VAL(pointer,         NULL_ADDRESS_ERROR);

    ReturnValue value = {};

    if(array_of_tokens->address[*pointer].value.operation != COS &&
       array_of_tokens->address[*pointer].value.operation != PRINTF)
    {
        return value;
    }

    value.node = &(array_of_tokens->address[*pointer]);
    (*pointer)++;

    if(array_of_tokens->address[*pointer].type != OP ||
       array_of_tokens->address[*pointer].value.operation != '(')
    {
        fprintf(stderr, "the '(' character was expected, but the '%c' character was received\n",
                array_of_tokens->address[*pointer].value.operation                               );
        value.error = SYNTAX_ERROR;
        return value;
    }
    (*pointer)++;

    ReturnValue value_two = getExpression(array_of_tokens, pointer);

    value.node->left = value_two.node;

    if(array_of_tokens->address[*pointer].type != OP ||
       array_of_tokens->address[*pointer].value.operation != ')')
    {
        fprintf(stderr, "the ')' character was expected, but the '%c' character was received\n",
                array_of_tokens->address[*pointer].value.operation                               );
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

    if(array_of_tokens->address[*pointer].type != ID)
    {
        return value;
    }

    value.node = &(array_of_tokens->address[*pointer]);
    (*pointer)++;

    return value;
}

ReturnValue getNumeral(TokensInfo* array_of_tokens, int* pointer)
{
    CHECK_NULL_ADDR_RET_VAL(array_of_tokens, NULL_ADDRESS_ERROR);
    CHECK_NULL_ADDR_RET_VAL(pointer,         NULL_ADDRESS_ERROR);

    ReturnValue value = {};

    value.node = &(array_of_tokens->address[*pointer]);
    (*pointer)++;

    return value;
}
