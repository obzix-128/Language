#ifndef CTORANDDTOR_H
#define CTORANDDTOR_H

#include "Language.h"
#include "ErrorHandler.h"

Node* newNode(NodeType type, double numeral_value, char variable, Operations operation_value,
              Node* left, Node* right);
ErrorNumbers treeDtor(FILE* log_file, Node* node);

#endif // CTORANDDTOR_H
