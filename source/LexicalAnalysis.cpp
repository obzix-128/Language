#include "LexicalAnalysis.h"
#include "WorkWithLogFile.h"


ErrorNumbers lexicalAnalysis(FILE* log_file, BufferInfo* program, TokensInfo* array_of_tokens,
                             IdTableInfo* table                                               )
{
    CHECK_NULL_ADDR_ERROR(log_file,        NULL_ADDRESS_ERROR);
    CHECK_NULL_ADDR_ERROR(program,         NULL_ADDRESS_ERROR);
    CHECK_NULL_ADDR_ERROR(array_of_tokens, NULL_ADDRESS_ERROR);
    CHECK_NULL_ADDR_ERROR(table,           NULL_ADDRESS_ERROR);

    ErrorNumbers check_error = NO_ERROR;

    CHECK_ERROR(check_error, arrayOfTokensCtor(log_file, array_of_tokens));
    CHECK_ERROR(check_error, tableOfIdCtor    (log_file, table          ));

    CHECK_ERROR(check_error, skipSpaces(program));

    while(program->buffer[program->pointer] != '$')
    {
        if('0' <= program->buffer[program->pointer] && program->buffer[program->pointer] <= '9')
        {
            CHECK_ERROR(check_error, getNumeral(log_file, program, array_of_tokens));
        }
        else
        {
            CHECK_ERROR(check_error, getId(log_file, program, array_of_tokens, table));
        }
        CHECK_ERROR(check_error, skipSpaces(program));
    }

    CHECK_ERROR(check_error, getId(log_file, program, array_of_tokens, table));

    return check_error;
}

ErrorNumbers tableOfIdCtor(FILE* log_file, IdTableInfo* table)
{
    CHECK_NULL_ADDR_ERROR(log_file, NULL_ADDRESS_ERROR);
    CHECK_NULL_ADDR_ERROR(table,    NULL_ADDRESS_ERROR);

    const int CAPACITY_OF_TABEL_WITH_ID = 32;

    table->capacity = CAPACITY_OF_TABEL_WITH_ID;
    table->size     = 0;

    table->cell = (IdInfo*) calloc(table->capacity, sizeof(IdInfo));

    return NO_ERROR;
}

ErrorNumbers insertIntoTableOfId(FILE* log_file, Node* token, IdTableInfo* table)
{
    CHECK_NULL_ADDR_ERROR(log_file, NULL_ADDRESS_ERROR);
    CHECK_NULL_ADDR_ERROR(token,    NULL_ADDRESS_ERROR);

    ErrorNumbers check_error = NO_ERROR;

    if(table->size == table->capacity)
    {
        CHECK_ERROR(check_error, reallocMemory(log_file, (void**)(&(table->cell)),
                                               &(table->capacity), sizeof(IdInfo) ));
    }

    for(int i = 0; table->cell[i].id != NULL; i++)
    {
        if(0 == strncmp(token->value.id_info.id, table->cell[i].id, token->value.id_info.length) &&
           token->value.id_info.length == table->cell[i].length)
        {
            token->value.id_info.number = table->cell[i].number;
            return check_error;
        }
    }

    table->cell[table->size].id     = token->value.id_info.id;
    table->cell[table->size].length = token->value.id_info.length;
    table->cell[table->size].number = table->size;
    token->value.id_info.number     = table->size;

    table->size += 1;

    return check_error;
}

ErrorNumbers arrayOfTokensCtor(FILE* log_file, TokensInfo* array_of_tokens)
{
    CHECK_NULL_ADDR_ERROR(log_file,        NULL_ADDRESS_ERROR);
    CHECK_NULL_ADDR_ERROR(array_of_tokens, NULL_ADDRESS_ERROR);

    const int CAPACITY_OF_TOKENS_ARRAY = 128;

    array_of_tokens->capacity = CAPACITY_OF_TOKENS_ARRAY;
    array_of_tokens->size = 0;

    array_of_tokens->address = (Node*) calloc(array_of_tokens->capacity, sizeof(Node));
    CHECK_NULL_ADDR_ERROR(array_of_tokens->address, CALLOC_ERROR);

    return NO_ERROR;
}

ErrorNumbers reallocMemory(FILE* log_file, void** address, int* capacity, size_t size_type)
{
    CHECK_NULL_ADDR_ERROR(log_file, NULL_ADDRESS_ERROR);
    CHECK_NULL_ADDR_ERROR(address,  NULL_ADDRESS_ERROR);
    CHECK_NULL_ADDR_ERROR(capacity, NULL_ADDRESS_ERROR);

    *address = realloc(*address, (*capacity) * size_type * 2);

    memset(((char*)(*address) + (*capacity) * size_type), 0, (*capacity) * size_type);

    *capacity = (*capacity) * 2;

    return NO_ERROR;
}

ErrorNumbers skipSpaces(BufferInfo* program)
{
    CHECK_NULL_ADDR_ERROR(program, NULL_ADDRESS_ERROR);

    while(true)
    {
        if(program->buffer[program->pointer] != '\n' &&
           program->buffer[program->pointer] != ' '    )
        {
            break;
        }
        else
        {
            program->pointer++;
        }
    }

    return NO_ERROR;
}

ErrorNumbers getNumeral(FILE* log_file, BufferInfo* program, TokensInfo* array_of_tokens)
{
    CHECK_NULL_ADDR_ERROR(log_file,        NULL_ADDRESS_ERROR);
    CHECK_NULL_ADDR_ERROR(program,            NULL_ADDRESS_ERROR);
    CHECK_NULL_ADDR_ERROR(array_of_tokens, NULL_ADDRESS_ERROR);

    ErrorNumbers check_error = NO_ERROR;

    if(array_of_tokens->size == array_of_tokens->capacity)
    {
        CHECK_ERROR(check_error, reallocMemory(log_file, (void**)(&(array_of_tokens->address)),
                                               &(array_of_tokens->capacity), sizeof(Node)      ));
    }

    char* end_of_num = NULL;

    double value = strtod(&(program->buffer[program->pointer]), &end_of_num);

    array_of_tokens->address[array_of_tokens->size].type = NUM;
    array_of_tokens->address[array_of_tokens->size].value.numeral = value;
    array_of_tokens->size += 1;

    program->pointer += (int)(end_of_num - &(program->buffer[program->pointer]));

    return NO_ERROR;
}

ErrorNumbers getId(FILE* log_file, BufferInfo* program, TokensInfo* array_of_tokens,
                   IdTableInfo* table)
{
    CHECK_NULL_ADDR_ERROR(log_file,        NULL_ADDRESS_ERROR);
    CHECK_NULL_ADDR_ERROR(program,         NULL_ADDRESS_ERROR);
    CHECK_NULL_ADDR_ERROR(array_of_tokens, NULL_ADDRESS_ERROR);
    CHECK_NULL_ADDR_ERROR(table,           NULL_ADDRESS_ERROR);

    ErrorNumbers check_error = NO_ERROR;

    if(array_of_tokens->size == array_of_tokens->capacity)
    {
        CHECK_ERROR(check_error, reallocMemory(log_file, (void**)(&(array_of_tokens->address)),
                                               &(array_of_tokens->capacity), sizeof(Node)      ));
    }

    array_of_tokens->address[array_of_tokens->size].type = ID;
    array_of_tokens->address[array_of_tokens->size].value.id_info.id =
    &(program->buffer[program->pointer]);

    if(isalnum(program->buffer[program->pointer]))
    {
        CHECK_ERROR(check_error, findVerbKeyword(log_file, program, array_of_tokens, table))
    }
    else if(program->buffer[program->pointer]     == '/' &&
            program->buffer[program->pointer + 1] == '*'   )
    {
        while(program->buffer[program->pointer]     != '*' ||
              program->buffer[program->pointer + 1] != '/'   )
        {
            program->pointer += 1;
        }
        program->pointer += 2;
        return NO_ERROR;
    }
    else if(ispunct(program->buffer[program->pointer]))
    {
        CHECK_ERROR(check_error, findPunctKeyword(log_file, program, array_of_tokens, table))
    }
    else
    {
        fprintf(stdout, "symbol number %d, '%c'\n", program->pointer,
                program->buffer[program->pointer]                    );
        fprintf(stdout, "Here: \"%.11s\"\n", &(program->buffer[program->pointer - 5]));
        return SYNTAX_ERROR;
    }

    array_of_tokens->size += 1;

    return NO_ERROR;
}


ErrorNumbers findPunctKeyword(FILE* log_file, BufferInfo* program, TokensInfo* array_of_tokens,
                             IdTableInfo* table)
{
    CHECK_NULL_ADDR_ERROR(log_file,        NULL_ADDRESS_ERROR);
    CHECK_NULL_ADDR_ERROR(program,         NULL_ADDRESS_ERROR);
    CHECK_NULL_ADDR_ERROR(array_of_tokens, NULL_ADDRESS_ERROR);
    CHECK_NULL_ADDR_ERROR(table,           NULL_ADDRESS_ERROR);

    ErrorNumbers check_error = NO_ERROR;

    array_of_tokens->address[array_of_tokens->size].value.id_info.id =
    &(program->buffer[program->pointer]);

    array_of_tokens->address[array_of_tokens->size].value.id_info.length = 1;
    program->pointer += 1;

    array_of_tokens->address[array_of_tokens->size].type = OP;

    if(array_of_tokens->address[array_of_tokens->size].value.id_info.id[0] == '+')
    {
        array_of_tokens->address[array_of_tokens->size].value.operation = ADD;
    }
    else if(array_of_tokens->address[array_of_tokens->size].value.id_info.id[0] == '-')
    {
        array_of_tokens->address[array_of_tokens->size].value.operation = SUB;
    }
    else if(array_of_tokens->address[array_of_tokens->size].value.id_info.id[0] == '*')
    {
        array_of_tokens->address[array_of_tokens->size].value.operation = MUL;
    }
    else if(array_of_tokens->address[array_of_tokens->size].value.id_info.id[0] == '/')
    {
        array_of_tokens->address[array_of_tokens->size].value.operation = DIV;
    }
    else if(array_of_tokens->address[array_of_tokens->size].value.id_info.id[0] == '^')
    {
        array_of_tokens->address[array_of_tokens->size].value.operation = POW;
    }
    else if(array_of_tokens->address[array_of_tokens->size].value.id_info.id[0] == '>')
    {
        if(array_of_tokens->address[array_of_tokens->size].value.id_info.id[1] == '=')
        {
            array_of_tokens->address[array_of_tokens->size].value.operation = JAE;
            program->pointer += 1;
        }
        else
        {
            array_of_tokens->address[array_of_tokens->size].value.operation = JA;
        }
    }
    else if(array_of_tokens->address[array_of_tokens->size].value.id_info.id[0] == '<')
    {
        if(array_of_tokens->address[array_of_tokens->size].value.id_info.id[1] == '=')
        {
            array_of_tokens->address[array_of_tokens->size].value.operation = JBE;
            program->pointer += 1;
        }
        else
        {
            array_of_tokens->address[array_of_tokens->size].value.operation = JB;
        }
    }
    else if(array_of_tokens->address[array_of_tokens->size].value.id_info.id[0] == '(')
    {
        array_of_tokens->address[array_of_tokens->size].value.operation = L_SK;
    }
    else if(array_of_tokens->address[array_of_tokens->size].value.id_info.id[0] == ')')
    {
        array_of_tokens->address[array_of_tokens->size].value.operation = R_SK;
    }
    else if(array_of_tokens->address[array_of_tokens->size].value.id_info.id[0] == '{')
    {
        array_of_tokens->address[array_of_tokens->size].value.operation = LF_SK;
    }
    else if(array_of_tokens->address[array_of_tokens->size].value.id_info.id[0] == '}')
    {
        array_of_tokens->address[array_of_tokens->size].value.operation = RF_SK;
    }
    else if(array_of_tokens->address[array_of_tokens->size].value.id_info.id[0] == '=')
    {
        array_of_tokens->address[array_of_tokens->size].value.operation = EQUAL;
    }
    else if(array_of_tokens->address[array_of_tokens->size].value.id_info.id[0] == ';')
    {
        array_of_tokens->address[array_of_tokens->size].value.operation = EOP;
    }
    else if(array_of_tokens->address[array_of_tokens->size].value.id_info.id[0] == '$')
    {
        array_of_tokens->address[array_of_tokens->size].value.operation = END;
    }
    else
    {
        fprintf(stderr, "Invalid character = '%c'\n",
                array_of_tokens->address[array_of_tokens->size].value.id_info.id[0]);
        check_error = SYNTAX_ERROR;
        return check_error;
    }

    return check_error;
}

ErrorNumbers findVerbKeyword(FILE* log_file, BufferInfo* program, TokensInfo* array_of_tokens,
                             IdTableInfo* table)
{
    CHECK_NULL_ADDR_ERROR(log_file,        NULL_ADDRESS_ERROR);
    CHECK_NULL_ADDR_ERROR(program,         NULL_ADDRESS_ERROR);
    CHECK_NULL_ADDR_ERROR(array_of_tokens, NULL_ADDRESS_ERROR);
    CHECK_NULL_ADDR_ERROR(table,           NULL_ADDRESS_ERROR);

    ErrorNumbers check_error = NO_ERROR;

    array_of_tokens->address[array_of_tokens->size].value.id_info.id =
    &(program->buffer[program->pointer]);

    array_of_tokens->address[array_of_tokens->size].value.id_info.length = 0;

    while(isalnum(program->buffer[program->pointer]) ||
          program->buffer[program->pointer] == '_'     )
    {
        program->pointer++;
        array_of_tokens->address[array_of_tokens->size].value.id_info.length++;
    }

    if((0 == strncmp(array_of_tokens->address[array_of_tokens->size].value.id_info.id,
                     "cos", 3                                                         )) &&
       array_of_tokens->address[array_of_tokens->size].value.id_info.length == 3           )
    {
        array_of_tokens->address[array_of_tokens->size].type            = OP;
        array_of_tokens->address[array_of_tokens->size].value.operation = COS;
    }
    else if((0 == strncmp(array_of_tokens->address[array_of_tokens->size].value.id_info.id,
                          "printf", 6                                                      )) &&
            array_of_tokens->address[array_of_tokens->size].value.id_info.length == 6           )
    {
        array_of_tokens->address[array_of_tokens->size].type            = OP;
        array_of_tokens->address[array_of_tokens->size].value.operation = PRINTF;
    }
    else if((0 == strncmp(array_of_tokens->address[array_of_tokens->size].value.id_info.id,
                          "if", 2                                                          )) &&
            array_of_tokens->address[array_of_tokens->size].value.id_info.length == 2           )
    {
        array_of_tokens->address[array_of_tokens->size].type            = OP;
        array_of_tokens->address[array_of_tokens->size].value.operation = IF;
    }
    else if((0 == strncmp(array_of_tokens->address[array_of_tokens->size].value.id_info.id,
                          "while", 5                                                       )) &&
            array_of_tokens->address[array_of_tokens->size].value.id_info.length == 5)
    {
        array_of_tokens->address[array_of_tokens->size].type            = OP;
        array_of_tokens->address[array_of_tokens->size].value.operation = WHILE;
    }
    else
    {
        CHECK_ERROR(check_error,
                    insertIntoTableOfId(log_file,
                                        &(array_of_tokens->address[array_of_tokens->size]), table));
    }

    return check_error;
}
