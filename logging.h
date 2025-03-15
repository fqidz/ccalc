#ifndef LOGGING_H
#define LOGGING_H

#include <stdbool.h>
#include <stddef.h>

#define RED "\x1B[31m"
#define BOLD "\x1B[1m"
#define RESET "\x1B[0m"

#define LOG_MESSAGE(label, fmt, ...)                                         \
    fprintf(stderr, RED BOLD "[%s] %s:%i: " fmt RESET "\n", label, __FILE__, \
            __LINE__, __VA_ARGS__)

#define LOG_ASSERT(expr)                                           \
    do {                                                           \
        if (!(expr)) {                                             \
            LOG_MESSAGE("ERROR", "Assertion `%s` failed.", #expr); \
            exit(1);                                               \
        }                                                          \
    } while (0)

// #define LOG_TEST(message)

#define UNREACHABLE                                  \
    LOG_MESSAGE("ERROR", "Path unreachable!%s", ""); \
    exit(1)

typedef enum {
    NO_ERROR,
    INVALID_CHAR,
    NUMBER_EXTRA_DOT,
} ErrorType;

typedef struct {
    ErrorType type;
    size_t char_pos;
    char *input_string;
} Error;

char *error_to_string(Error error);

#endif /* ifndef LOGGING_H */
