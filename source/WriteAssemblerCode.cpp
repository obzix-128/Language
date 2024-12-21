#include "WriteAssemblerCode.h"


ErrorNumbers writeAssemblerCode(FILE* log_file, FILE* file_to_write, Node* node)
{
    CHECK_NULL_ADDR_ERROR(log_file,      NULL_ADDRESS_ERROR);
    CHECK_NULL_ADDR_ERROR(file_to_write, NULL_ADDRESS_ERROR);
    CHECK_NULL_ADDR_ERROR(node,          NULL_ADDRESS_ERROR);

    ErrorNumbers check_error = NO_ERROR;

    if(node->type == NUM)
    {
        fprintf(file_to_write, "push %lg\n", node->value.numeral);
    }
    else if(node->type == ID)
    {
        fprintf(file_to_write, "push [%d] ; ", node->value.id_info.number);
        for(int i = 0; i < node->value.id_info.length; i++)
        {
            fprintf(file_to_write, "%c", node->value.id_info.id[i]);
        }

        fprintf(file_to_write, "\n");
    }
    else if(node->type == OP)
    {
        CHECK_ERROR(check_error, writeOperation(log_file, file_to_write, node));
    }
    else
    {
        check_error = TYPE_ERROR;
        return check_error;
    }

    return check_error;
}

ErrorNumbers writeOperation(FILE* log_file, FILE* file_to_write, Node* node)
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
            CHECK_ERROR(check_error, writeAssemblerCode(log_file, file_to_write, node->left ));
            CHECK_ERROR(check_error, writeAssemblerCode(log_file, file_to_write, node->right));

            fprintf(file_to_write, "add\n");
            break;
        }
        case SUB:
        {
            CHECK_ERROR(check_error, writeAssemblerCode(log_file, file_to_write, node->left ));
            CHECK_ERROR(check_error, writeAssemblerCode(log_file, file_to_write, node->right));

            fprintf(file_to_write, "sub\n");
            break;
        }
        case MUL:
        {
            CHECK_ERROR(check_error, writeAssemblerCode(log_file, file_to_write, node->left ));
            CHECK_ERROR(check_error, writeAssemblerCode(log_file, file_to_write, node->right));

            fprintf(file_to_write, "mul\n");
            break;
        }
        case DIV:
        {
            CHECK_ERROR(check_error, writeAssemblerCode(log_file, file_to_write, node->left ));
            CHECK_ERROR(check_error, writeAssemblerCode(log_file, file_to_write, node->right));

            fprintf(file_to_write, "div\n");
            break;
        }
        case POW:
        {
            CHECK_ERROR(check_error, writeAssemblerCode(log_file, file_to_write, node->left ));
            CHECK_ERROR(check_error, writeAssemblerCode(log_file, file_to_write, node->right));

            fprintf(file_to_write, "pow\n");
            break;
        }
        case JA:
        {
            fprintf(file_to_write, "\n; begining of '>'\n");

            CHECK_ERROR(check_error, writeAssemblerCode(log_file, file_to_write, node->right));
            CHECK_ERROR(check_error, writeAssemblerCode(log_file, file_to_write, node->left ));

            fprintf(file_to_write, "jb ja_%d:\n", counter);
            fprintf(file_to_write, "push 0\n");
            fprintf(file_to_write, "jmp end_ja_%d:\n", counter);

            fprintf(file_to_write, "ja_%d:\n", counter);
            fprintf(file_to_write, "push 1\n");

            fprintf(file_to_write, "end_ja_%d:\n", counter);

            fprintf(file_to_write, "\n; end of '>'\n");

            counter += 1;

            break;
        }
        case JB:
        {
            fprintf(file_to_write, "\n; begining of '<'\n");

            CHECK_ERROR(check_error, writeAssemblerCode(log_file, file_to_write, node->left ));
            CHECK_ERROR(check_error, writeAssemblerCode(log_file, file_to_write, node->right));

            fprintf(file_to_write, "jb jb_%d:\n", counter);
            fprintf(file_to_write, "push 0\n");
            fprintf(file_to_write, "jmp end_jb_%d:\n", counter);

            fprintf(file_to_write, "jb_%d:\n", counter);
            fprintf(file_to_write, "push 1\n");

            fprintf(file_to_write, "end_jb_%d:\n", counter);

            fprintf(file_to_write, "\n; end of '<'\n");

            counter += 1;

            break;
        }
        case JAE:
        {
            fprintf(file_to_write, "\n; begining of '>='\n");

            CHECK_ERROR(check_error, writeAssemblerCode(log_file, file_to_write, node->right));
            CHECK_ERROR(check_error, writeAssemblerCode(log_file, file_to_write, node->left ));

            fprintf(file_to_write, "jbe jae_%d:\n", counter);
            fprintf(file_to_write, "push 0\n");
            fprintf(file_to_write, "jmp end_jae_%d:\n", counter);

            fprintf(file_to_write, "jae_%d:\n", counter);
            fprintf(file_to_write, "push 1\n");

            fprintf(file_to_write, "end_jae_%d:\n", counter);

            fprintf(file_to_write, "\n; end of '>='\n");

            counter += 1;

            break;
        }
        case JBE:
        {
            fprintf(file_to_write, "\n; begining of '<='\n");

            CHECK_ERROR(check_error, writeAssemblerCode(log_file, file_to_write, node->left ));
            CHECK_ERROR(check_error, writeAssemblerCode(log_file, file_to_write, node->right));

            fprintf(file_to_write, "jbe jbe_%d:\n", counter);
            fprintf(file_to_write, "push 0\n");
            fprintf(file_to_write, "jmp end_jbe_%d:\n", counter);

            fprintf(file_to_write, "jbe_%d:\n", counter);
            fprintf(file_to_write, "push 1\n");

            fprintf(file_to_write, "end_jbe_%d:\n", counter);

            fprintf(file_to_write, "\n; end of '<='\n");

            counter += 1;

            break;
        }
        case COS:
        {
            CHECK_ERROR(check_error, writeAssemblerCode(log_file, file_to_write, node->left));

            fprintf(file_to_write, "cos\n");
            break;
        }
        case EQUAL:
        {
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

            break;
        }
        case PRINTF:
        {
            fprintf(file_to_write, "\n; begining of printf\n");

            CHECK_ERROR(check_error, writeAssemblerCode(log_file, file_to_write, node->left));

            fprintf(file_to_write, "out\n");
            fprintf(file_to_write, "; end of printf\n");

            break;
        }
        case IF:
        {
            int number = counter;
            counter += 1;

            fprintf(file_to_write, "\n; begining of if\n");

            CHECK_ERROR(check_error, writeAssemblerCode(log_file, file_to_write, node->left));

            fprintf(file_to_write, "push 0\n");
            fprintf(file_to_write, "je end_if_%d:\n", number);

            CHECK_ERROR(check_error, writeAssemblerCode(log_file, file_to_write, node->right));

            fprintf(file_to_write, "end_if_%d:\n", number);
            fprintf(file_to_write, "; end of if\n");

            break;
        }
        case WHILE:
        {
            int number = counter;
            counter += 1;

            fprintf(file_to_write, "\n; begining of while\n");

            fprintf(file_to_write, "begining_while_%d:\n", number);

            CHECK_ERROR(check_error, writeAssemblerCode(log_file, file_to_write, node->left));

            fprintf(file_to_write, "push 0\n");
            fprintf(file_to_write, "je end_while_%d:\n", number);

            CHECK_ERROR(check_error, writeAssemblerCode(log_file, file_to_write, node->right));

            fprintf(file_to_write, "jmp begining_while_%d:\n", number);

            fprintf(file_to_write, "end_while_%d:\n", number);
            fprintf(file_to_write, "; end of while\n");

            break;
        }
        case EOP:
        {
            CHECK_ERROR(check_error, writeAssemblerCode(log_file, file_to_write, node->left));
            fprintf(file_to_write, "; EOP\n");

            if(node->right != NULL)
            {
                CHECK_ERROR(check_error, writeAssemblerCode(log_file, file_to_write, node->right));
            }

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
