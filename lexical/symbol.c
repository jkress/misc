/**
 * symbol.c
 * 
 * Library to control the symbol table during compilation
 * 
 * COP3402
 * Jason Kress - jason@micrux.net
 * Chad Sine - chad@unixenthusiast.org
 * Last updated October 14, 2007
 */

#include "symbol.h"

/* Allocate the symbol table */
void symbol_init() {
    if (symbol == NULL)
        symbol = (struct symbol_table **) calloc(TABLESIZE, sizeof(struct symbol_table));
}

/* Calculate the symbol table address hash */
unsigned int symbol_hash(char *token) {

    if (token != NULL && isalpha(token[0]))
        return (token[0] + token[strlen(token) - 1]) % TABLESIZE;

    else {
        util_err("Attempting to hash invalid symbol");
        return -1;
    }
}

/* Create and add an entry into the symbol table
 * Returns -1 on failure
 * Returns the address in the table on success */
struct symbol_pos *symbol_add(char *name, int type) {
    int index, offset = 0;
    struct symbol_table *new, *cur;
    struct symbol_pos *new_pos;

    if (symbol == NULL)
        symbol_init();

    new = (struct symbol_table*) malloc(sizeof(struct symbol_table));
    new_pos = (struct symbol_pos*) malloc(sizeof(struct symbol_pos));

    /* Create the node's contents */
    new->type = type;
    new->next = NULL;
    strcpy(new->name, name);

    /* Insert it into the linked list */
    index = symbol_hash(name);
    cur = symbol[index];

    if (cur == NULL)
        symbol[index] = new;

    else {
        ++offset;
        while (cur->next != NULL) {
            cur = cur->next;
            ++offset;
        }
        cur->next = new;
    }

    /* Return position */
    new_pos->index = index;
    new_pos->offset = offset;
    return new_pos;
}

/* Returns 1 if the token exists in the table
 * Returns 0 if the token does not exist */
struct symbol_pos *symbol_exist(char *name) {

    int index, offset = 0;
    struct symbol_table *cur;
    struct symbol_pos *temp_pos;

    temp_pos = (struct symbol_pos*) malloc(sizeof(struct symbol_pos));

    if (symbol == NULL)
        symbol_init();

    if (name == NULL) {
        util_err("Cannot lookup empty string in Symbol Table.");
        return NULL;
    }

    index = symbol_hash(name);
    temp_pos->index = index;
    cur = symbol[index];

    /* Empty LL, not found */
    if (cur == NULL) {
        free(temp_pos);
        return NULL;
    }

    /* First node is == name */
    if (strcmp(cur->name, name) == 0) {
        temp_pos->offset = offset;
        return temp_pos;
    } else {
        while (cur != NULL) {

            /* Found node */
            if (strcmp(cur->name, name) == 0) {
                temp_pos->offset = offset;
                return temp_pos;
            }
            cur = cur->next;
            ++offset;
        }
    }

    return NULL;
}

/* Returns a pointer to the entry in the symbol table
 * referenced by position pos */
struct symbol_table *symbol_get(struct symbol_pos *pos) {
    struct symbol_table *cur;
    int i;

    if (symbol == NULL)
        return NULL;

    cur = symbol[pos->index];

    for (i = 0; i < pos->offset; i++)
        cur = cur->next;

    return cur;
}

/* Print out the contents of the symbol table in hash table format
 * This is for debugging, mostly */
void symbol_print() {
    int i;
    struct symbol_table *temp;

    if (symbol == NULL)
        return;

    for(i=0;i<TABLESIZE;i++) {
        printf("%d: -> ",i);
        if(symbol[i] == NULL)
            printf("(null)\n");
        else {
            temp = symbol[i];
            while(temp != NULL) {
                printf("%s(%d) -> ",temp->name,temp->type);
                temp = temp->next;
            }
            printf("(null)\n");
        }
    }
}

void symbol_finished() {
    int i;
    struct symbol_table *temp, *next;

    if(symbol == NULL)
        return;

    for(i=0;i<TABLESIZE;i++) {
        if(symbol[i] != NULL) {
            temp = symbol[i];
            next = temp->next;
            while(temp != NULL) {
                free(temp);
                temp = next;
                if(next != NULL) next = next->next;
            }
        }
    }
    free(symbol);
}
