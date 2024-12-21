#include "ReadAndWrite.h"


ErrorNumbers treeWrite(FILE* log_file, FILE* file_with_tree, Node* node)
{
    CHECK_NULL_ADDR_ERROR(log_file,       NULL_ADDRESS_ERROR);
    CHECK_NULL_ADDR_ERROR(file_with_tree, NULL_ADDRESS_ERROR);
    CHECK_NULL_ADDR_ERROR(node,           NULL_ADDRESS_ERROR);

    ErrorNumbers check_error = NO_ERROR;

    static int size_of_indentation = 0;

    fprintf(file_with_tree, "{");
    switch(node->type)
    {
        case NUM:
        {
            fprintf(file_with_tree, "NUM:\"%lg\"", node->value.numeral);
            break;
        }
        case ID:
        {
            fprintf(file_with_tree, "ID:\"");
            for(int i = 0; i < node->value.id_info.length; i++)
            {
                fprintf(file_with_tree, "%c", node->value.id_info.id[i]);
            }
            fprintf(file_with_tree, "\"");
            break;
        }
        case OP:
        {
            fprintf(file_with_tree, "OP:\"");
            CHECK_ERROR(check_error, writeOperation(log_file, file_with_tree, node));
            fprintf(file_with_tree, "\"");
            break;
        }
        case POISON_TYPE:
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
    size_of_indentation += 1;

    if(node->left != 0)
    {
        fprintf(file_with_tree, "\n");
        CHECK_ERROR(check_error, indentText(file_with_tree, size_of_indentation));
        CHECK_ERROR(check_error, treeWrite(log_file, file_with_tree, node->left));
    }
    if(node->right != 0)
    {
        CHECK_ERROR(check_error, treeWrite(log_file, file_with_tree, node->right));
    }

    fprintf(file_with_tree, "}\n");
    size_of_indentation -= 1;
    CHECK_ERROR(check_error, indentText(file_with_tree, size_of_indentation));

    return check_error;
}

ErrorNumbers writeOperation(FILE* log_file, FILE* file_with_tree, Node* node)
{
    CHECK_NULL_ADDR_ERROR(log_file,       NULL_ADDRESS_ERROR);
    CHECK_NULL_ADDR_ERROR(file_with_tree, NULL_ADDRESS_ERROR);
    CHECK_NULL_ADDR_ERROR(node,           NULL_ADDRESS_ERROR);

    ErrorNumbers check_error = NO_ERROR;

    switch(node->value.operation)
    {
        case ADD:
        {
            fprintf(file_with_tree, "+");
            break;
        }
        case SUB:
        {
            fprintf(file_with_tree, "-");
            break;
        }
        case MUL:
        {
            fprintf(file_with_tree, "*");
            break;
        }
        case DIV:
        {
            fprintf(file_with_tree, "/");
            break;
        }
        case POW:
        {
            fprintf(file_with_tree, "^");
            break;
        }
        case JA:
        {
            fprintf(file_with_tree, "more");
            break;
        }
        case JB:
        {
            fprintf(file_with_tree, "less");
            break;
        }
        case JAE:
        {
            fprintf(file_with_tree, "jae");
            break;
        }
        case JBE:
        {
            fprintf(file_with_tree, "jbe");
            break;
        }
        case COS:
        {
            fprintf(file_with_tree, "cos");
            break;
        }
        case L_SK:
        {
            fprintf(file_with_tree, "(");
            break;
        }
        case R_SK:
        {
            fprintf(file_with_tree, ")");
            break;
        }
        case LF_SK:
        {
            fprintf(file_with_tree, "{");
            break;
        }
        case RF_SK:
        {
            fprintf(file_with_tree, "}");
            break;
        }
        case EQUAL:
        {
            fprintf(file_with_tree, "=");
            break;
        }
        case PRINTF:
        {
            fprintf(file_with_tree, "printf");
            break;
        }
        case IF:
        {
            fprintf(file_with_tree, "if");
            break;
        }
        case WHILE:
        {
            fprintf(file_with_tree, "while");
            break;
        }
        case EOP:
        {
            fprintf(file_with_tree, ";");
            break;
        }
        case END:
        {
            fprintf(file_with_tree, "$");
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

ErrorNumbers indentText(FILE* file_with_tree, int size_of_indentation)
{
    CHECK_NULL_ADDR_ERROR(file_with_tree, NULL_ADDRESS_ERROR);

    for(int i = 0; i < size_of_indentation; i++)
    {
        fprintf(file_with_tree, "    ");
    }

    return NO_ERROR;
}
