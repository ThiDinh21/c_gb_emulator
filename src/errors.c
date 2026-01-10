#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "errors.h"

void __attribute__((noreturn)) panic(char *err_msg, ErrorCode code)
{
    fprintf(stderr, "%s\n", err_msg);
    exit(code);
}

void __attribute__((noreturn)) panic_unimplemented(char *panic_location)
{
    char err_msg[100] = "Unimplemented code reached at ";
    strcat(err_msg, panic_location);
    panic(err_msg, ERR_UNIMPLEMENTED);
}