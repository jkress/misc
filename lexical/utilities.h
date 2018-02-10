/**
 * utilities.h
 * Header file for utilities.c
 * 
 * Library with assorted helper functions for 
 * I/O, Logging and Printing
 * 
 * COP3402
 * Jason Kress - jason@micrux.net
 * Chad Sine - chad@unixenthusiast.org
 * Last updated October 14, 2007
 */

#ifndef _UTILITIES_H
#define _UTILITIES_H

#include "symbol.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

FILE *token_stream;

/* I/O */
void util_print_banner();
FILE *util_open_file(int argc, char *argv[]);

/* Logging */
void util_log(const char*);
void util_err(const char*);

void util_write_token(int);

void util_print_screen();
void util_print_file(const char*);
void util_print_symbolic();
void util_lexeme_token_attribute();
void util_finished();

#endif
