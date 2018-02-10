/**
 * symbol.h
 * Header file for symbol.c
 * 
 * Library to control the symbol table during compilation
 * 
 * COP3402
 * Jason Kress - jason@micrux.net
 * Chad Sine - chad@unixenthusiast.org
 * Last updated October 14, 2007
 */

#ifndef _SYMBOL_H
#define _SYMBOL_H

#define TABLESIZE 40

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "utilities.h"

/* All of our lexeme types */
enum token_type {
    nulsym = 1, idsym, numbersym, plussym, minussym,
    multsym,  slashsym, oddsym, eqsym, neqsym, lessym, leqsym,
    gtrsym, geqsym, lparentsym, rparentsym, commasym, semicolonsym,
    periodsym, becomessym, beginsym, endsym, ifsym, thensym,
    whilesym, dosym, callsym, constsym, varsym, procsym, writesym
};

/* Definition of the symbol table */
struct symbol_table {
    int type;
    char name[10];
    int val;
    int level;
    int adr;
    struct symbol_table *next;
};

/* Definition of the symbol location */
struct symbol_pos {
    int index;
    int offset;
};

struct symbol_table **symbol;

void symbol_init();
unsigned int symbol_hash(char *token);
struct symbol_pos *symbol_add(char *name, int type);
struct symbol_pos *symbol_exist(char *name);
struct symbol_table *symbol_get(struct symbol_pos *pos);
void symbol_print();
void symbol_finished();

#endif
