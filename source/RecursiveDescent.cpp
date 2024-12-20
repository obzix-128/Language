#include "RecursiveDescent.h"


ReturnValue recursiveDescent(TokensInfo* array_of_tokens)
{
    CHECK_NULL_ADDR_RET_VAL(array_of_tokens, NULL_ADDRESS_ERROR);

    int pointer = 0;

    ReturnValue value = {};

    while(array_of_tokens->address[pointer].value.operation != '$')
    {
        CHECK_RETURN_VALUE(value, getOperation(array_of_tokens, &pointer));
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
    if(l_branch.node != NULL)
    {
        CHECK_RETURN_VALUE(value, checkSemicolon(array_of_tokens, pointer));

        value.node->left = l_branch.node;
        return value;
    }

    CHECK_RETURN_VALUE(l_branch, getFuncWithOneArg(array_of_tokens, pointer));
    if(l_branch.node != NULL)
    {
        CHECK_RETURN_VALUE(value, checkSemicolon(array_of_tokens, pointer));

        value.node->left = l_branch.node;
        return value;
    }

    CHECK_RETURN_VALUE(l_branch, getIf(array_of_tokens, pointer));
    if(l_branch.node != NULL)
    {
        CHECK_RETURN_VALUE(value, checkSemicolon(array_of_tokens, pointer));

        value.node->left = l_branch.node;
        return value;
    }

    CHECK_RETURN_VALUE(l_branch, getWhile(array_of_tokens, pointer));
    if(l_branch.node != NULL)
    {
        CHECK_RETURN_VALUE(value, checkSemicolon(array_of_tokens, pointer));

        value.node->left = l_branch.node;
        return value;
    }

    if(array_of_tokens->address[*pointer].value.operation != '{')
    {
        return value;
    }
    (*pointer) += 1;

    CHECK_RETURN_VALUE(value, getOperation(array_of_tokens, pointer));
    if(value.node == NULL)
    {
        fprintf(stderr, "No operators detected\n");
        value.error = SYNTAX_ERROR;
        return value;
    }

    ReturnValue old_operation = value;
    ReturnValue new_operation = {};

    while(old_operation.node != NULL)
    {
        CHECK_RETURN_VALUE(new_operation, getOperation(array_of_tokens, pointer));

        old_operation.node->right = new_operation.node;
        old_operation = new_operation;

        if(*pointer >= array_of_tokens->size)
        {
            value.error = SIZE_ERROR;
            return value;
        }
    };

    if(array_of_tokens->address[*pointer].value.operation != '}')
    {
        fprintf(stderr, "the '}' character was expected, but the '%c' character was received\n",
                array_of_tokens->address[*pointer].value.operation                              );
        value.error = SYNTAX_ERROR;
        return value;
    }
    (*pointer) += 1;

    return value;
}

ReturnValue checkSemicolon(TokensInfo* array_of_tokens, int* pointer)
{
    CHECK_NULL_ADDR_RET_VAL(array_of_tokens, NULL_ADDRESS_ERROR);
    CHECK_NULL_ADDR_RET_VAL(pointer,         NULL_ADDRESS_ERROR);

    ReturnValue value = {};

    if(array_of_tokens->address[*pointer].value.operation != ';')
    {
        fprintf(stderr, "the ';' character was expected, but the '%c' character was received\n",
                array_of_tokens->address[*pointer].value.operation                              );
        value.error = SYNTAX_ERROR;
        return value;
    }

    value.node = &(array_of_tokens->address[*pointer]);
    (*pointer)++;

    return value;
}

ReturnValue getWhile(TokensInfo* array_of_tokens, int* pointer)
{
    CHECK_NULL_ADDR_RET_VAL(array_of_tokens, NULL_ADDRESS_ERROR);
    CHECK_NULL_ADDR_RET_VAL(pointer,         NULL_ADDRESS_ERROR);

    ReturnValue result = {};

    if(array_of_tokens->address[*pointer].type != OP ||
       array_of_tokens->address[*pointer].value.operation != 'w')
    {
        return result;
    }

    result.node = &(array_of_tokens->address[*pointer]);
    (*pointer)++;

    if(array_of_tokens->address[*pointer].type != OP ||
       array_of_tokens->address[*pointer].value.operation != '(')
    {
        fprintf(stderr, "the '(' character was expected, but the '%c' character was received\n",
                array_of_tokens->address[*pointer].value.operation                               );
        result.error = SYNTAX_ERROR;
        return result;
    }
    (*pointer)++;

    ReturnValue condition = getComparsion(array_of_tokens, pointer);

    result.node->left = condition.node;

    if(array_of_tokens->address[*pointer].type != OP ||
       array_of_tokens->address[*pointer].value.operation != ')')
    {
        fprintf(stderr, "the ')' character was expected, but the '%c' character was received\n",
                array_of_tokens->address[*pointer].value.operation                               );
        result.error = SYNTAX_ERROR;
        return result;
    }
    (*pointer)++;

    ReturnValue body = getOperation(array_of_tokens, pointer);
    result.node->right = body.node;

    return result;
}

ReturnValue getIf(TokensInfo* array_of_tokens, int* pointer)
{
    CHECK_NULL_ADDR_RET_VAL(array_of_tokens, NULL_ADDRESS_ERROR);
    CHECK_NULL_ADDR_RET_VAL(pointer,         NULL_ADDRESS_ERROR);

    ReturnValue result = {};

    if(array_of_tokens->address[*pointer].type != OP ||
       array_of_tokens->address[*pointer].value.operation != 'i')
    {
        return result;
    }

    result.node = &(array_of_tokens->address[*pointer]);
    (*pointer)++;

    if(array_of_tokens->address[*pointer].type != OP ||
       array_of_tokens->address[*pointer].value.operation != '(')
    {
        fprintf(stderr, "the '(' character was expected, but the '%c' character was received\n",
                array_of_tokens->address[*pointer].value.operation                               );
        result.error = SYNTAX_ERROR;
        return result;
    }
    (*pointer)++;

    ReturnValue condition = getComparsion(array_of_tokens, pointer);

    result.node->left = condition.node;

    if(array_of_tokens->address[*pointer].type != OP ||
       array_of_tokens->address[*pointer].value.operation != ')')
    {
        fprintf(stderr, "the ')' character was expected, but the '%c' character was received\n",
                array_of_tokens->address[*pointer].value.operation                               );
        result.error = SYNTAX_ERROR;
        return result;
    }
    (*pointer)++;

    ReturnValue body = getOperation(array_of_tokens, pointer);
    result.node->right = body.node;

    return result;
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

        CHECK_RETURN_VALUE(r_value, getComparsion(array_of_tokens, pointer));
        result.node->right = r_value.node;
    }
    else
    {
        (*pointer) -= 1;
        return result;
    }

    return result;
}

ReturnValue getComparsion(TokensInfo* array_of_tokens, int* pointer)
{
    CHECK_NULL_ADDR_RET_VAL(array_of_tokens, NULL_ADDRESS_ERROR);
    CHECK_NULL_ADDR_RET_VAL(pointer,         NULL_ADDRESS_ERROR);

    ReturnValue value = {};
    CHECK_RETURN_VALUE(value, getExpression(array_of_tokens, pointer));

    while( array_of_tokens->address[*pointer].type == OP &&               //TODO? стоит ли делать такой пробельчик?
          (array_of_tokens->address[*pointer].value.operation == '>' ||
           array_of_tokens->address[*pointer].value.operation == '<' ||
           array_of_tokens->address[*pointer].value.operation == 'a' ||
           array_of_tokens->address[*pointer].value.operation == 'b'   ))
    {
        array_of_tokens->address[*pointer].left = value.node;
        value.node = &(array_of_tokens->address[*pointer]);
        (*pointer)++;

        ReturnValue value_two = {};
        CHECK_RETURN_VALUE(value_two, getExpression(array_of_tokens, pointer));
        if(value_two.error != NO_ERROR)
        {
            return value_two;
        }

        value.node->right = value_two.node;
    }

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

        CHECK_RETURN_VALUE(value, getComparsion(array_of_tokens, pointer));

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

    ReturnValue value_two = getComparsion(array_of_tokens, pointer);

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
