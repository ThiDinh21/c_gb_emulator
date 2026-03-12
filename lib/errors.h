#ifndef ERRORS_H
#define ERRORS_H

#include "types.h"

/// Prints err_msg and code to stderr, then terminates the program.
void __attribute__((noreturn)) panic(char *err_msg, ErrorCode code);

/// Prints the location of the unimplemented feature to stderr, then terminates the program.
void __attribute__((noreturn)) panic_unimplemented(char *panic_location);

#endif
