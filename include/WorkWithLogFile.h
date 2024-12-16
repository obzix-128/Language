#ifndef WORKWITHLOGFILE_H
#define WORKWITHLOGFILE_H

#include "Language.h"
#include "ErrorHandler.h"

#define OPEN_FILE_IN_RECORDING_MODE "w"
#define OPEN_FILE_IN_READING_MODE   "r"

ErrorNumbers openFile(FILE** file, const char* file_name, const char* opening_mode);
ErrorNumbers readFile(const char* file_name, char** task_buffer);
ErrorNumbers getFileSize(FILE* the_task_file, int* size);
ErrorNumbers treeDump(FILE* log_file, Node* root, const char* func_name, Node* new_node);
ErrorNumbers createGraf(FILE* log_file, Node* node, Node* new_node);
ErrorNumbers buildAllNodes(Node* node, FILE* file_to_write, Node* new_node);

#endif // WORKWITHLOGFILE_H
