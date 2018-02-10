/**
 * lexical.c
 *
 * Lexical Analyzer
 *
 * Library with assorted helper functions for 
 * I/O, Logging and Printing
 * 
 * COP3402
 * Jason Kress - jason@micrux.net
 * Chad Sine - chad@unixenthusiast.org
 * Last updated October 14, 2007
 */

#include "utilities.h"

char *token_type_symbolic[] = {"","nulsym","idsym","numbersym","plussym","minussym",
                               "multsym","slashsym","oddsym","eqsym","neqsym","lessym","leqsym",
                               "gtrsym","geqsym","lparentsym","rparentsym","commasym","semicolonsym",
                               "periodsym","becomessym","beginsym","endsym","ifsym","thensym",
                               "whilesym","dosym","callsym","constsym","varsym","procsym","writesym"};

char *token_type_literal[] = {"","null","","","+","-",
                              "*","/","odd","=","!=","<","<=",
                              ">",">=","(",")",",",";",
                              ".",":=","begin","end","if","then",
                              "while","do","call","const","var","procedure","write"};

/* I/O */
void util_print_banner(char *filename) {
    printf("  *******************************\n");
    printf("  *       Lexical Analyzer      *\n");
    printf("  * By: Chad Sine & Jason Kress *\n");
    printf("  *******************************\n\n");
    printf("Analyzing source file: %s\n\n",filename);
}

FILE *util_open_file(int argc, char *argv[]) {
    FILE *source;

    if(argc != 2) {
        fprintf(stderr,"USAGE: %s <input file>\n",argv[0]);
        return NULL;
    }
    if((source = fopen(argv[1], "r")) == NULL) {
        perror("File open failure");
        return NULL;
    }
    return source;
}

/* Logging */
void util_log(const char *message) {
    fprintf(stdout,"lexical: %s\n",message);
}

void util_err(const char *message) {
    fprintf(stderr,"ERROR: %s\n",message);
}

void util_write_token(int value) {
    if(token_stream == NULL)
        token_stream = tmpfile();

    fprintf(token_stream,"%d ",value);
}

void util_print_screen() {
    char c;

    if(token_stream == NULL)
        util_err("There is no token stream");
    else {
        printf("\nInternal Representation:\n------------------------\n");
        rewind(token_stream);
        c = fgetc(token_stream);
        while(c != EOF) {
            putc(c,stdout);
            c = fgetc(token_stream);
        }
        putc('\n',stdout);
    }

    return;
}

void util_print_file(const char *filename) {
    char c;
    FILE *out;

    if(token_stream == NULL)
        util_err("There is no token stream");
    else {
        out = fopen(filename, "w");
        rewind(token_stream);
        c = fgetc(token_stream);
        while(c != EOF) {
            fputc(c,out);
            c = fgetc(token_stream);
        }
        fputc('\n',out);
    }
    fclose(out);
    return;
}

void util_print_symbolic() {
    int cur;
    int eof = 0;

    if(token_stream == NULL)
        util_err("There is no token stream");
    else {
        printf("\nSymbolic Representation:\n------------------------\n");
        rewind(token_stream);
        fscanf(token_stream,"%d ",&cur);
        while(!eof) {
            printf("%s ",token_type_symbolic[cur]);

            /* Special places in the token stream */
            if(cur == 2) {
                fscanf(token_stream,"%d ",&cur);
                printf("%d ",cur);
                fscanf(token_stream,"%d ",&cur);
                printf("%d ",cur);
            } else if(cur == 3) {
                fscanf(token_stream,"%d ",&cur);
                printf("%d ",cur);
            }
            eof = feof(token_stream);
            fscanf(token_stream,"%d ",&cur);
        }
        printf("\n");
    }
}

void util_lexeme_token_attribute() {
    int cur, value, eof = 0;
    struct symbol_table *temp;
    struct symbol_pos pos;

    if(token_stream == NULL)
        util_err("There is no token stream");

    else {
        printf("\n\n");
        printf("Lexeme      Token\tAttribute (Symbol Table Index & Offset)\n");
        printf("---------------------------------\n");
        rewind(token_stream);
        fscanf(token_stream,"%d ",&cur);
        while(!eof) {
            if(cur == 2) {
                fscanf(token_stream,"%d %d ",&(pos.index),&(pos.offset));

                temp = symbol_get(&pos);
                printf("%-12s%d\t\t%d %d\n",temp->name,cur,pos.index,pos.offset);
            } else if(cur == 3) {
                fscanf(token_stream,"%d ",&value);
                printf("%-12d%d\n",value,cur);
            } else {
                printf("%-12s%d\n",token_type_literal[cur],cur);
            }
            eof = feof(token_stream);
            fscanf(token_stream,"%d ",&cur);
        }
        printf("\n");
    }
}

void util_finished() {
    fclose(token_stream);
}
