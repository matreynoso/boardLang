#ifndef ABSTRACT_SYNTAX_TREE_PRINTER_HEADER
#define ABSTRACT_SYNTAX_TREE_PRINTER_HEADER

#include "../../support/configuration/Environment.h"
#include "../../support/logging/Logger.h"
#include "../../support/type/ModuleDestructor.h"
#include "AbstractSyntaxTree.h"
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>

/** Initialize module's internal state. */
ModuleDestructor initializeAbstractSyntaxTreePrinterModule();

/**
 * Prints the AST in the standard output as an indented tree, but only if the
 * "PRINT_AST" environment variable is "true". It's a debugging aid, to verify
 * the trees built by the parser (not only that a program is accepted).
 */
void printAbstractSyntaxTree(Program * program);

#endif
