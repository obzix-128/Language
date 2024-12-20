#ifndef READFILE_H
#define READFILE_H

#include <assert.h>
#include <errno.h>

#include "Language.h"
#include "ErrorHandler.h"
#include "CtorAndDtor.h"
#include "WorkWithLogFile.h"
#include "LexicalAnalysis.h"

struct ReturnValue
{
    Node* node;
    ErrorNumbers error;
};

ReturnValue recursiveDescent    (TokensInfo* array_of_tokens);
ReturnValue getOperation        (TokensInfo* array_of_tokens, int* pointer);
ReturnValue checkSemicolon      (TokensInfo* array_of_tokens, int* pointer);
ReturnValue getIf               (TokensInfo* array_of_tokens, int* pointer);
ReturnValue getAssignment       (TokensInfo* array_of_tokens, int* pointer);
ReturnValue getExpression       (TokensInfo* array_of_tokens, int* pointer);
ReturnValue getTerm             (TokensInfo* array_of_tokens, int* pointer);
ReturnValue getPower            (TokensInfo* array_of_tokens, int* pointer);
ReturnValue getPrimaryExpression(TokensInfo* array_of_tokens, int* pointer);
ReturnValue getFuncWithOneArg   (TokensInfo* array_of_tokens, int* pointer);
ReturnValue getVariable         (TokensInfo* array_of_tokens, int* pointer);
ReturnValue getNumeral          (TokensInfo* array_of_tokens, int* pointer);

#define CHECK_RETURN_VALUE(var, func)                 \
    var = func;                                       \
    if(var.error != NO_ERROR)                         \
    {                                                 \
        errorHandler(var.error, __PRETTY_FUNCTION__); \
        return var;                                   \
    }
#define CHECK_NULL_ADDR_RET_VAL(var, error_name) \
    if(var == NULL)                              \
    {                                            \
        ReturnValue error_found = {};            \
        error_found.error = error_name;          \
        return error_found;                      \
    }

#endif // READFILE_H
