#include "logging.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *error_to_string(Error error)
{
    switch (error.type) {
    case INVALID_CHAR:;
        size_t needed =
                (size_t)snprintf(NULL, 0,
                                 RED BOLD
                                 "[ERROR]: " RESET "%s\n%*c^\n" RED BOLD
                                 "char %i: Invalid character '%c'" RESET,
                                 error.input_string, (int)error.char_pos + 9,
                                 ' ', (int)error.char_pos,
                                 error.input_string[error.char_pos]) +
                1;
        char *buffer = malloc(needed);
        sprintf(buffer,
                RED BOLD "[ERROR]: " RESET "%s\n%*c^\n" RED BOLD
                         "char %i: Invalid character '%c'" RESET,
                error.input_string, (int)error.char_pos + 9, ' ',
                (int)error.char_pos, error.input_string[error.char_pos]);

        return buffer;
    case NO_ERROR:
    default:
        UNREACHABLE;
    }
}
