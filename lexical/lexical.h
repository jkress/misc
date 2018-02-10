/**
 * lexical.h
 * Header file for lexical.c
 * 
 * The lexical analyzer portion of our compiler. Splits
 * a program source stream up into legal tokens and adds
 * identifiers to the symbol table.
 *
 * COP3402
 * Jason Kress - jason@micrux.net
 * Chad Sine - chad@unixenthusiast.org
 * Last updated October 14, 2007
 */

#ifndef _LEXICAL_H
#define _LEXICAL_H

#include "symbol.h"
#include "utilities.h"

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define  NORW      12           /* number of reserved words */
#define  IMAX   32767           /* maximum integer value */
#define  NMAX       6           /* maximum number of digits in integer */
#define  CMAX      12           /* maximum number of chars for idents */
#define  NESTMAX    5           /* maximum depth of block nesting */
#define  STRMAX   256           /* maximum length of strings */
#define  ASCIIMAX 128           /* maximum ascii value */
#define  MSSYM      2           /* maximum length for multi-symbols */

int *ssym;

void lexical_init_symbols();
int lexical_check_symbol(char token);
int lexical_check_mutlisymbol(char token[]);
int lexical_check_reserved(char token[]);
int lexical_check_whitespace(char token);
int lexical_check_delimit(char token);
int lexical_scanner(FILE *source);
void lexical_finished();

#endif
