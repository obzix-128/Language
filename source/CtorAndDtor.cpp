#include "CtorAndDtor.h"


Node* newNode(NodeType type, double numeral_value, char variable_value, Operations operation_value,
              Node* left, Node* right)
{
    Node* new_node = (Node*) calloc(1, sizeof(Node));

    new_node->type  = type;
    new_node->left  = left;
    new_node->right = right;

    if(new_node->type == NUM)
    {
        new_node->value.numeral = numeral_value;
    }
    else if(new_node->type == ID)
    {
        new_node->value.id_info.id = &variable_value;
    }
    else if(new_node->type == OP)
    {
        new_node->value.operation = operation_value;
    }

    return new_node;
}

ErrorNumbers treeDtor(FILE* log_file, Node* node)
{
    CHECK_NULL_ADDR_ERROR(log_file, NULL_ADDRESS_ERROR);
    CHECK_NULL_ADDR_ERROR(node,     NULL_ADDRESS_ERROR);

    ErrorNumbers check_error = NO_ERROR;

    if(node->left != 0)
    {
        CHECK_ERROR(check_error, treeDtor(log_file, node->left));
    }
    if(node->right != 0)
    {
        CHECK_ERROR(check_error, treeDtor(log_file, node->right));
    }

    node->type  = POISON_TYPE;
    node->value.numeral = 0;
    node->left  = NULL;
    node->right = NULL;

    free(node);

    return check_error;
}
