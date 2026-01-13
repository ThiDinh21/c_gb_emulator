#ifndef ERRORS_H
#define ERRORS_H

#include "types.h"

void __attribute__((noreturn)) panic(char *err_msg, ErrorCode code);

void __attribute__((noreturn)) panic_unimplemented(char *panic_location);

#endif