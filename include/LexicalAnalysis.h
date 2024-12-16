#ifndef LEXICALANALYSIS_H
#define LEXICALANALYSIS_H

#include <string.h>
#include <ctype.h>

#include "Language.h"
#include "ErrorHandler.h"

struct TokensInfo
{
    Node* address;
    int size;
    int capacity;
};

ErrorNumbers lexicalAnalysis(FILE* log_file, BufferInfo* program, TokensInfo* array_of_tokens,
                             IdTableInfo* table                                               );
ErrorNumbers skipSpaces(BufferInfo* task);
ErrorNumbers getNumeral(FILE* log_file, BufferInfo* task, TokensInfo* array_of_tokens);
ErrorNumbers getId     (FILE* log_file, BufferInfo* task, TokensInfo* array_of_tokens);

ErrorNumbers arrayOfTokensCtor(FILE* log_file, TokensInfo* array_of_tokens);
ErrorNumbers reallocMemory(FILE* log_file, void** address, int* capacity, size_t size_type);

ErrorNumbers findKeyword        (FILE* log_file, TokensInfo* array_of_tokens, IdTableInfo* table);
ErrorNumbers tableOfIdCtor      (FILE* log_file,                              IdTableInfo* table);
ErrorNumbers insertIntoTableOfId(FILE* log_file, Node* token,                 IdTableInfo* table);

#endif // LEXICALANALYSIS_H
