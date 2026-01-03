#ifndef ERRORS_H
#define ERRORS_H

typedef enum
{
    ERR_NONE = 0,
    ERR_INVALID_MEMORY_ACCESS,
    ERR_ROM_LOAD_FAILURE,
    ERR_UNKNOWN_INSTRUCTION,
    ERR_UNIMPLEMENTED,
} ErrorCode;

void __attribute__((noreturn)) panic(char *err_msg, ErrorCode code);

void __attribute__((noreturn)) panic_unimplemented();

#endif