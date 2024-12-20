#include "Language.h"
#include "RecursiveDescent.h"
#include "LexicalAnalysis.h"
#include "WriteAssemblerCode.h"


int main(const int argc, const char** argv)
{
    const int _NUMBERS_OF_ARGUMENTS = 4;
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

    ReturnValue value = recursiveDescent(&array_of_tokens);

    if(value.error != NO_ERROR)
    {
        errorHandler(value.error, __PRETTY_FUNCTION__);
        return value.error;
    }

    CHECK_ERROR(check_error, treeDump(log_file, value.node, __PRETTY_FUNCTION__, NULL));

    FILE* asm_code_file = NULL;
    CHECK_ERROR(check_error, openFile(&asm_code_file, argv[3], OPEN_FILE_IN_RECORDING_MODE));

    CHECK_ERROR(check_error, writeAssemblerCode(log_file, asm_code_file, value.node));
    fprintf(asm_code_file, "hlt\n");

    CHECK_ERROR(check_error, treeDtor(log_file, value.node));

    free(program.buffer         );
    free(array_of_tokens.address);
    free(table.cell             );

    fclose(asm_code_file);
    fclose(log_file     );

    printf("DONE\n");

    return check_error;
}
