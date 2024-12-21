#include "WriteAssemblerCode.h"


ErrorNumbers writeAssemblerCode(FILE* log_file, FILE* file_to_write, Node* node)
{
    CHECK_NULL_ADDR_ERROR(log_file,      NULL_ADDRESS_ERROR);
    CHECK_NULL_ADDR_ERROR(file_to_write, NULL_ADDRESS_ERROR);
    CHECK_NULL_ADDR_ERROR(node,          NULL_ADDRESS_ERROR);

    ErrorNumbers check_error = NO_ERROR;

    if(node->type == NUM)
    {
        CHECK_ERROR(check_error, writeAsmNum(log_file, file_to_write, node));
    }
    else if(node->type == ID)
    {
        CHECK_ERROR(check_error, writeAsmVar(log_file, file_to_write, node));
    }
    else if(node->type == OP)
    {
        CHECK_ERROR(check_error, writeAsmOperation(log_file, file_to_write, node));
    }
    else
    {
        check_error = TYPE_ERROR;
        return check_error;
    }

    return check_error;
}

ErrorNumbers writeAsmNum(FILE* log_file, FILE* file_to_write, Node* node)
{
    CHECK_NULL_ADDR_ERROR(log_file,      NULL_ADDRESS_ERROR);
    CHECK_NULL_ADDR_ERROR(file_to_write, NULL_ADDRESS_ERROR);
    CHECK_NULL_ADDR_ERROR(node,          NULL_ADDRESS_ERROR);

    fprintf(file_to_write, "push %lg\n", node->value.numeral);

    return NO_ERROR;
}

ErrorNumbers writeAsmVar(FILE* log_file, FILE* file_to_write, Node* node)
{
    CHECK_NULL_ADDR_ERROR(log_file,      NULL_ADDRESS_ERROR);
    CHECK_NULL_ADDR_ERROR(file_to_write, NULL_ADDRESS_ERROR);
    CHECK_NULL_ADDR_ERROR(node,          NULL_ADDRESS_ERROR);

    fprintf(file_to_write, "push [%d] ; ", node->value.id_info.number);
    for(int i = 0; i < node->value.id_info.length; i++)
    {
        fprintf(file_to_write, "%c", node->value.id_info.id[i]);
    }

    fprintf(file_to_write, "\n");

    return NO_ERROR;
}

ErrorNumbers writeAsmOperation(FILE* log_file, FILE* file_to_write, Node* node)
{
    CHECK_NULL_ADDR_ERROR(log_file,      NULL_ADDRESS_ERROR);
    CHECK_NULL_ADDR_ERROR(file_to_write, NULL_ADDRESS_ERROR);
    CHECK_NULL_ADDR_ERROR(node,          NULL_ADDRESS_ERROR);

    ErrorNumbers check_error = NO_ERROR;

    static int counter = 0;

    switch(node->value.operation)
    {
        case ADD:
        {
            CHECK_ERROR(check_error, writeAdd(log_file, file_to_write, node));
            break;
        }
        case SUB:
        {
            CHECK_ERROR(check_error, writeSub(log_file, file_to_write, node));
            break;
        }
        case MUL:
        {
            CHECK_ERROR(check_error, writeMul(log_file, file_to_write, node));
            break;
        }
        case DIV:
        {
            CHECK_ERROR(check_error, writeDiv(log_file, file_to_write, node));
            break;
        }
        case POW:
        {
            CHECK_ERROR(check_error, writePow(log_file, file_to_write, node));
            break;
        }
        case JA:
        {
            CHECK_ERROR(check_error, writeJa(log_file, file_to_write, node, &counter));
            break;
        }
        case JB:
        {
            CHECK_ERROR(check_error, writeJb(log_file, file_to_write, node, &counter));
            break;
        }
        case JAE:
        {
            CHECK_ERROR(check_error, writeJae(log_file, file_to_write, node, &counter));
            break;
        }
        case JBE:
        {
            CHECK_ERROR(check_error, writeJbe(log_file, file_to_write, node, &counter));
            break;
        }
        case COS:
        {
            CHECK_ERROR(check_error, writeCos(log_file, file_to_write, node));
            break;
        }
        case EQUAL:
        {
            CHECK_ERROR(check_error, writeEqual(log_file, file_to_write, node));
            break;
        }
        case PRINTF:
        {
            CHECK_ERROR(check_error, writePrintf(log_file, file_to_write, node));
            break;
        }
        case IF:
        {
            CHECK_ERROR(check_error, writeIf(log_file, file_to_write, node, &counter));
            break;
        }
        case WHILE:
        {
            CHECK_ERROR(check_error, writeWhile(log_file, file_to_write, node, &counter));
            break;
        }
        case EOP:
        {
            CHECK_ERROR(check_error, writeEndOp(log_file, file_to_write, node));
            break;
        }
        case L_SK:
        {
            check_error = TYPE_ERROR;
            return check_error;
            break;
        }
        case R_SK:
        {
            check_error = TYPE_ERROR;
            return check_error;
            break;
        }
        case LF_SK:
        {
            check_error = TYPE_ERROR;
            return check_error;
            break;
        }
        case RF_SK:
        {
            check_error = TYPE_ERROR;
            return check_error;
            break;
        }
        case END:
        {
            check_error = TYPE_ERROR;
            return check_error;
            break;
        }
        case POISON:
        {
            check_error = TYPE_ERROR;
            return check_error;
            break;
        }
        default:
        {
            check_error = TYPE_ERROR;
            return check_error;
            break;
        }
    }

    return check_error;
}

ErrorNumbers writeAdd(FILE* log_file, FILE* file_to_write, Node* node)
{
    CHECK_NULL_ADDR_ERROR(log_file,      NULL_ADDRESS_ERROR);
    CHECK_NULL_ADDR_ERROR(file_to_write, NULL_ADDRESS_ERROR);
    CHECK_NULL_ADDR_ERROR(node,          NULL_ADDRESS_ERROR);

    ErrorNumbers check_error = NO_ERROR;

    CHECK_ERROR(check_error, writeAssemblerCode(log_file, file_to_write, node->left ));
    CHECK_ERROR(check_error, writeAssemblerCode(log_file, file_to_write, node->right));

    fprintf(file_to_write, "add\n");

    return NO_ERROR;
}

ErrorNumbers writeSub(FILE* log_file, FILE* file_to_write, Node* node)
{
    CHECK_NULL_ADDR_ERROR(log_file,      NULL_ADDRESS_ERROR);
    CHECK_NULL_ADDR_ERROR(file_to_write, NULL_ADDRESS_ERROR);
    CHECK_NULL_ADDR_ERROR(node,          NULL_ADDRESS_ERROR);

    ErrorNumbers check_error = NO_ERROR;

    CHECK_ERROR(check_error, writeAssemblerCode(log_file, file_to_write, node->left ));
    CHECK_ERROR(check_error, writeAssemblerCode(log_file, file_to_write, node->right));

    fprintf(file_to_write, "sub\n");

    return NO_ERROR;
}

ErrorNumbers writeMul(FILE* log_file, FILE* file_to_write, Node* node)
{
    CHECK_NULL_ADDR_ERROR(log_file,      NULL_ADDRESS_ERROR);
    CHECK_NULL_ADDR_ERROR(file_to_write, NULL_ADDRESS_ERROR);
    CHECK_NULL_ADDR_ERROR(node,          NULL_ADDRESS_ERROR);

    ErrorNumbers check_error = NO_ERROR;

    CHECK_ERROR(check_error, writeAssemblerCode(log_file, file_to_write, node->left ));
    CHECK_ERROR(check_error, writeAssemblerCode(log_file, file_to_write, node->right));

    fprintf(file_to_write, "mul\n");

    return NO_ERROR;
}

ErrorNumbers writeDiv(FILE* log_file, FILE* file_to_write, Node* node)
{
    CHECK_NULL_ADDR_ERROR(log_file,      NULL_ADDRESS_ERROR);
    CHECK_NULL_ADDR_ERROR(file_to_write, NULL_ADDRESS_ERROR);
    CHECK_NULL_ADDR_ERROR(node,          NULL_ADDRESS_ERROR);

    ErrorNumbers check_error = NO_ERROR;

    CHECK_ERROR(check_error, writeAssemblerCode(log_file, file_to_write, node->left ));
    CHECK_ERROR(check_error, writeAssemblerCode(log_file, file_to_write, node->right));

    fprintf(file_to_write, "div\n");

    return NO_ERROR;
}

ErrorNumbers writePow(FILE* log_file, FILE* file_to_write, Node* node)
{
    CHECK_NULL_ADDR_ERROR(log_file,      NULL_ADDRESS_ERROR);
    CHECK_NULL_ADDR_ERROR(file_to_write, NULL_ADDRESS_ERROR);
    CHECK_NULL_ADDR_ERROR(node,          NULL_ADDRESS_ERROR);

    ErrorNumbers check_error = NO_ERROR;

    CHECK_ERROR(check_error, writeAssemblerCode(log_file, file_to_write, node->left ));
    CHECK_ERROR(check_error, writeAssemblerCode(log_file, file_to_write, node->right));

    fprintf(file_to_write, "pow\n");

    return NO_ERROR;
}

ErrorNumbers writeJa(FILE* log_file, FILE* file_to_write, Node* node, int* counter)
{
    CHECK_NULL_ADDR_ERROR(log_file,      NULL_ADDRESS_ERROR);
    CHECK_NULL_ADDR_ERROR(file_to_write, NULL_ADDRESS_ERROR);
    CHECK_NULL_ADDR_ERROR(node,          NULL_ADDRESS_ERROR);

    ErrorNumbers check_error = NO_ERROR;

    fprintf(file_to_write, "\n; begining of '>'\n");

    CHECK_ERROR(check_error, writeAssemblerCode(log_file, file_to_write, node->right));
    CHECK_ERROR(check_error, writeAssemblerCode(log_file, file_to_write, node->left ));

    fprintf(file_to_write, "jb ja_%d:\n", *counter);
    fprintf(file_to_write, "push 0\n");
    fprintf(file_to_write, "jmp end_ja_%d:\n", *counter);

    fprintf(file_to_write, "ja_%d:\n", *counter);
    fprintf(file_to_write, "push 1\n");

    fprintf(file_to_write, "end_ja_%d:\n", *counter);

    fprintf(file_to_write, "\n; end of '>'\n");

    *counter += 1;

    return check_error;
}

ErrorNumbers writeJb(FILE* log_file, FILE* file_to_write, Node* node, int* counter)
{
    CHECK_NULL_ADDR_ERROR(log_file,      NULL_ADDRESS_ERROR);
    CHECK_NULL_ADDR_ERROR(file_to_write, NULL_ADDRESS_ERROR);
    CHECK_NULL_ADDR_ERROR(node,          NULL_ADDRESS_ERROR);

    ErrorNumbers check_error = NO_ERROR;

    fprintf(file_to_write, "\n; begining of '<'\n");

    CHECK_ERROR(check_error, writeAssemblerCode(log_file, file_to_write, node->left ));
    CHECK_ERROR(check_error, writeAssemblerCode(log_file, file_to_write, node->right));

    fprintf(file_to_write, "jb jb_%d:\n", *counter);
    fprintf(file_to_write, "push 0\n");
    fprintf(file_to_write, "jmp end_jb_%d:\n", *counter);

    fprintf(file_to_write, "jb_%d:\n", *counter);
    fprintf(file_to_write, "push 1\n");

    fprintf(file_to_write, "end_jb_%d:\n", *counter);

    fprintf(file_to_write, "\n; end of '<'\n");

    *counter += 1;

    return check_error;
}

ErrorNumbers writeJae(FILE* log_file, FILE* file_to_write, Node* node, int* counter)
{
    CHECK_NULL_ADDR_ERROR(log_file,      NULL_ADDRESS_ERROR);
    CHECK_NULL_ADDR_ERROR(file_to_write, NULL_ADDRESS_ERROR);
    CHECK_NULL_ADDR_ERROR(node,          NULL_ADDRESS_ERROR);

    ErrorNumbers check_error = NO_ERROR;

    fprintf(file_to_write, "\n; begining of '>='\n");

    CHECK_ERROR(check_error, writeAssemblerCode(log_file, file_to_write, node->right));
    CHECK_ERROR(check_error, writeAssemblerCode(log_file, file_to_write, node->left ));

    fprintf(file_to_write, "jbe jae_%d:\n", *counter);
    fprintf(file_to_write, "push 0\n");
    fprintf(file_to_write, "jmp end_jae_%d:\n", *counter);

    fprintf(file_to_write, "jae_%d:\n", *counter);
    fprintf(file_to_write, "push 1\n");

    fprintf(file_to_write, "end_jae_%d:\n", *counter);

    fprintf(file_to_write, "\n; end of '>='\n");

    *counter += 1;

    return check_error;
}

ErrorNumbers writeJbe(FILE* log_file, FILE* file_to_write, Node* node, int* counter)
{
    CHECK_NULL_ADDR_ERROR(log_file,      NULL_ADDRESS_ERROR);
    CHECK_NULL_ADDR_ERROR(file_to_write, NULL_ADDRESS_ERROR);
    CHECK_NULL_ADDR_ERROR(node,          NULL_ADDRESS_ERROR);

    ErrorNumbers check_error = NO_ERROR;

    fprintf(file_to_write, "\n; begining of '<='\n");

    CHECK_ERROR(check_error, writeAssemblerCode(log_file, file_to_write, node->left ));
    CHECK_ERROR(check_error, writeAssemblerCode(log_file, file_to_write, node->right));

    fprintf(file_to_write, "jbe jbe_%d:\n", *counter);
    fprintf(file_to_write, "push 0\n");
    fprintf(file_to_write, "jmp end_jbe_%d:\n", *counter);

    fprintf(file_to_write, "jbe_%d:\n", *counter);
    fprintf(file_to_write, "push 1\n");

    fprintf(file_to_write, "end_jbe_%d:\n", *counter);

    fprintf(file_to_write, "\n; end of '<='\n");

    *counter += 1;

    return check_error;
}

ErrorNumbers writeCos(FILE* log_file, FILE* file_to_write, Node* node)
{
    CHECK_NULL_ADDR_ERROR(log_file,      NULL_ADDRESS_ERROR);
    CHECK_NULL_ADDR_ERROR(file_to_write, NULL_ADDRESS_ERROR);
    CHECK_NULL_ADDR_ERROR(node,          NULL_ADDRESS_ERROR);

    ErrorNumbers check_error = NO_ERROR;

    CHECK_ERROR(check_error, writeAssemblerCode(log_file, file_to_write, node->left));

    fprintf(file_to_write, "cos\n");

    return NO_ERROR;
}

ErrorNumbers writeEqual(FILE* log_file, FILE* file_to_write, Node* node)
{
    CHECK_NULL_ADDR_ERROR(log_file,      NULL_ADDRESS_ERROR);
    CHECK_NULL_ADDR_ERROR(file_to_write, NULL_ADDRESS_ERROR);
    CHECK_NULL_ADDR_ERROR(node,          NULL_ADDRESS_ERROR);

    ErrorNumbers check_error = NO_ERROR;

    if(node->left->type != ID)
    {
        check_error = TYPE_ERROR;
        return check_error;
    }
    fprintf(file_to_write, "\n; begining of assignment\n");

    CHECK_ERROR(check_error, writeAssemblerCode(log_file, file_to_write, node->right));

    fprintf(file_to_write, "pop [%d] ; ", node->left->value.id_info.number);

    for(int i = 0; i < node->left->value.id_info.length; i++)
    {
        fprintf(file_to_write, "%c", node->left->value.id_info.id[i]);
    }
    fprintf(file_to_write, "\n; end of assignment\n");

    return check_error;
}

ErrorNumbers writePrintf(FILE* log_file, FILE* file_to_write, Node* node)
{
    CHECK_NULL_ADDR_ERROR(log_file,      NULL_ADDRESS_ERROR);
    CHECK_NULL_ADDR_ERROR(file_to_write, NULL_ADDRESS_ERROR);
    CHECK_NULL_ADDR_ERROR(node,          NULL_ADDRESS_ERROR);

    ErrorNumbers check_error = NO_ERROR;

    fprintf(file_to_write, "\n; begining of printf\n");

    CHECK_ERROR(check_error, writeAssemblerCode(log_file, file_to_write, node->left));

    fprintf(file_to_write, "out\n");
    fprintf(file_to_write, "; end of printf\n");

    return NO_ERROR;
}

ErrorNumbers writeIf(FILE* log_file, FILE* file_to_write, Node* node, int* counter)
{
    CHECK_NULL_ADDR_ERROR(log_file,      NULL_ADDRESS_ERROR);
    CHECK_NULL_ADDR_ERROR(file_to_write, NULL_ADDRESS_ERROR);
    CHECK_NULL_ADDR_ERROR(node,          NULL_ADDRESS_ERROR);

    ErrorNumbers check_error = NO_ERROR;

    int number = *counter;
    *counter += 1;

    fprintf(file_to_write, "\n; begining of if\n");

    CHECK_ERROR(check_error, writeAssemblerCode(log_file, file_to_write, node->left));

    fprintf(file_to_write, "push 0\n");
    fprintf(file_to_write, "je end_if_%d:\n", number);

    CHECK_ERROR(check_error, writeAssemblerCode(log_file, file_to_write, node->right));

    fprintf(file_to_write, "end_if_%d:\n", number);
    fprintf(file_to_write, "; end of if\n");

    return check_error;
}

ErrorNumbers writeWhile(FILE* log_file, FILE* file_to_write, Node* node, int* counter)
{
    CHECK_NULL_ADDR_ERROR(log_file,      NULL_ADDRESS_ERROR);
    CHECK_NULL_ADDR_ERROR(file_to_write, NULL_ADDRESS_ERROR);
    CHECK_NULL_ADDR_ERROR(node,          NULL_ADDRESS_ERROR);

    ErrorNumbers check_error = NO_ERROR;

    int number = *counter;
    *counter += 1;

    fprintf(file_to_write, "\n; begining of while\n");

    fprintf(file_to_write, "begining_while_%d:\n", number);

    CHECK_ERROR(check_error, writeAssemblerCode(log_file, file_to_write, node->left));

    fprintf(file_to_write, "push 0\n");
    fprintf(file_to_write, "je end_while_%d:\n", number);

    CHECK_ERROR(check_error, writeAssemblerCode(log_file, file_to_write, node->right));

    fprintf(file_to_write, "jmp begining_while_%d:\n", number);

    fprintf(file_to_write, "end_while_%d:\n", number);
    fprintf(file_to_write, "; end of while\n");

    return check_error;
}

ErrorNumbers writeEndOp(FILE* log_file, FILE* file_to_write, Node* node)
{
    CHECK_NULL_ADDR_ERROR(log_file,      NULL_ADDRESS_ERROR);
    CHECK_NULL_ADDR_ERROR(file_to_write, NULL_ADDRESS_ERROR);
    CHECK_NULL_ADDR_ERROR(node,          NULL_ADDRESS_ERROR);

    ErrorNumbers check_error = NO_ERROR;

    CHECK_ERROR(check_error, writeAssemblerCode(log_file, file_to_write, node->left));
    fprintf(file_to_write, "; EOP\n");

    if(node->right != NULL)
    {
        CHECK_ERROR(check_error, writeAssemblerCode(log_file, file_to_write, node->right));
    }

    return NO_ERROR;
}
