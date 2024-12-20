#ifndef WRITEASSEMBLERCODE_H
#define WRITEASSEMBLERCODE_H

#include "Language.h"
#include "ErrorHandler.h"

ErrorNumbers writeAssemblerCode(FILE* log_file, FILE* file_to_write, Node* root);
ErrorNumbers writeOperation    (FILE* log_file, FILE* file_to_write, Node* node);

#endif // WRITEASSEMBLERCODE_H
