#include <stdlib.h>
#include <stdio.h>
#include "errors.h"

void __attribute__((noreturn)) panic(char *err_msg, ErrorCode code)
{
    fprintf(stderr, "%s\n", err_msg);
    exit(code);
}

void __attribute__((noreturn)) panic_unimplemented()
{
    panic("Unimplemented code reached", ERR_UNIMPLEMENTED);
}