#ifndef WRITEASSEMBLERCODE_H
#define WRITEASSEMBLERCODE_H

#include "Language.h"
#include "ErrorHandler.h"

ErrorNumbers writeAssemblerCode(FILE* log_file, FILE* file_to_write, Node* root);
ErrorNumbers writeAsmNum       (FILE* log_file, FILE* file_to_write, Node* node);
ErrorNumbers writeAsmVar       (FILE* log_file, FILE* file_to_write, Node* node);
ErrorNumbers writeAsmOperation (FILE* log_file, FILE* file_to_write, Node* node);
ErrorNumbers writeAdd          (FILE* log_file, FILE* file_to_write, Node* node);
ErrorNumbers writeSub          (FILE* log_file, FILE* file_to_write, Node* node);
ErrorNumbers writeMul          (FILE* log_file, FILE* file_to_write, Node* node);
ErrorNumbers writeDiv          (FILE* log_file, FILE* file_to_write, Node* node);
ErrorNumbers writePow          (FILE* log_file, FILE* file_to_write, Node* node);
ErrorNumbers writeCos          (FILE* log_file, FILE* file_to_write, Node* node);
ErrorNumbers writePrintf       (FILE* log_file, FILE* file_to_write, Node* node);
ErrorNumbers writeEndOp        (FILE* log_file, FILE* file_to_write, Node* node);
ErrorNumbers writeEqual        (FILE* log_file, FILE* file_to_write, Node* node);
ErrorNumbers writeJa           (FILE* log_file, FILE* file_to_write, Node* node, int* counter);
ErrorNumbers writeJb           (FILE* log_file, FILE* file_to_write, Node* node, int* counter);
ErrorNumbers writeJae          (FILE* log_file, FILE* file_to_write, Node* node, int* counter);
ErrorNumbers writeJbe          (FILE* log_file, FILE* file_to_write, Node* node, int* counter);
ErrorNumbers writeIf           (FILE* log_file, FILE* file_to_write, Node* node, int* counter);
ErrorNumbers writeWhile        (FILE* log_file, FILE* file_to_write, Node* node, int* counter);


#endif // WRITEASSEMBLERCODE_H
