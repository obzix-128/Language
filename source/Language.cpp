#include "Language.h"
#include "RecursiveDescent.h"
#include "LexicalAnalysis.h"


int main(const int argc, const char** argv)
{
    const int _NUMBERS_OF_ARGUMENTS = 3;
    if(argc != _NUMBERS_OF_ARGUMENTS)
    {
        errorHandler(NUMBER_OF_ARG_ERROR, __PRETTY_FUNCTION__);
        return NUMBER_OF_ARG_ERROR;
    }

    ErrorNumbers check_error = NO_ERROR;

    FILE* log_file = NULL;
    CHECK_ERROR(check_error, openFile(&log_file, argv[1], OPEN_FILE_IN_RECORDING_MODE));
    fprintf(log_file, "<pre>\n");

    BufferInfo program = {};

    CHECK_ERROR(check_error, readFile(argv[2], &(program.buffer)));

    TokensInfo  array_of_tokens = {};
    IdTableInfo table           = {};

    CHECK_ERROR(check_error, lexicalAnalysis(log_file, &program, &array_of_tokens, &table));

    for(int i = 0; i < array_of_tokens.size; i++)
    {
        CHECK_ERROR(check_error, treeDump(log_file, &(array_of_tokens.address[i]),
                                          __PRETTY_FUNCTION__, NULL               ));
    }

    ReturnValue value = recursiveDescent(&array_of_tokens);
    if(value.error != NO_ERROR)
    {
        errorHandler(value.error, __PRETTY_FUNCTION__);
        return value.error;
    }

    CHECK_ERROR(check_error, treeDump(log_file, value.node, __PRETTY_FUNCTION__, NULL));

    CHECK_ERROR(check_error, treeDtor(log_file, value.node));

    free(program.buffer);
    free(array_of_tokens.address);
    free(table.cell);
    fclose(log_file);

    printf("DONE\n");

    return check_error;
}
