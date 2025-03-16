#include "logging.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *error_to_string(Error error)
{
    size_t needed;
    char *buffer;

    switch (error.type) {
    case INVALID_CHAR:
        needed = (size_t)snprintf(NULL, 0,
                                  RED BOLD
                                  "[ERROR]: " RESET "%s\n%*c^\n" RED BOLD
                                  "char %i: Invalid character '%c'" RESET,
                                  error.input_string, (int)error.char_pos + 9,
                                  ' ', (int)error.char_pos,
                                  error.input_string[error.char_pos]) +
                 1;
        buffer = malloc(needed);
        sprintf(buffer,
                RED BOLD "[ERROR]: " RESET "%s\n%*c^\n" RED BOLD
                         "char %i: Invalid character '%c'" RESET,
                error.input_string, (int)error.char_pos + 9, ' ',
                (int)error.char_pos, error.input_string[error.char_pos]);

        return buffer;
    case NUMBER_EXTRA_DOT:
        needed = (size_t)snprintf(NULL, 0,
                                  RED BOLD
                                  "[ERROR]: " RESET "%s\n%*c^\n" RED BOLD
                                  "char %i: Number has extra dot '%c'" RESET,
                                  error.input_string, (int)error.char_pos + 9,
                                  ' ', (int)error.char_pos,
                                  error.input_string[error.char_pos]) +
                 1;
        buffer = malloc(needed);
        sprintf(buffer,
                RED BOLD "[ERROR]: " RESET "%s\n%*c^\n" RED BOLD
                         "char %i: Number has extra dot '%c'" RESET,
                error.input_string, (int)error.char_pos + 9, ' ',
                (int)error.char_pos, error.input_string[error.char_pos]);

        return buffer;
    case EXTRA_BRACKET:
        needed = (size_t)snprintf(NULL, 0,
                                  RED BOLD "[ERROR]: " RESET
                                           "%s\n%*c^\n" RED BOLD
                                           "char %i: Extra bracket '%c'" RESET,
                                  error.input_string, (int)error.char_pos + 9,
                                  ' ', (int)error.char_pos,
                                  error.input_string[error.char_pos]) +
                 1;
        buffer = malloc(needed);
        sprintf(buffer,
                RED BOLD "[ERROR]: " RESET "%s\n%*c^\n" RED BOLD
                         "char %i: Extra bracket '%c'" RESET,
                error.input_string, (int)error.char_pos + 9, ' ',
                (int)error.char_pos, error.input_string[error.char_pos]);

        return buffer;
    case NO_ERROR:
    default:
        UNREACHABLE;
    }
}
