#ifndef READANDWRITE_H
#define READANDWRITE_H

#include "Language.h"
#include "ErrorHandler.h"

ErrorNumbers treeWrite     (FILE* log_file, FILE* file_with_tree, Node* node);
ErrorNumbers writeOperation(FILE* log_file, FILE* file_with_tree, Node* node);
ErrorNumbers indentText    (FILE* file_with_tree, int size_of_indentation);

#endif // READANDWRITE_H
