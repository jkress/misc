/**
 * lexical.c
 *
 * Lexical Analyzer
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

#include "lexical.h"

void lexical_init_symbols() {

    if(ssym == NULL) {

        int i;
        ssym = (int *) calloc(128,sizeof(int));

        /* Clear the ssym table */
        for(i=0;i<ASCIIMAX;i++)
            ssym[i] = -1;

        /* Symbol id number lookup table */
        ssym['+']=plussym;
        ssym['-']=minussym;
        ssym['*']=multsym;
        ssym['/']=slashsym;
        ssym['(']=lparentsym;
        ssym[')']=rparentsym;
        ssym['=']=eqsym;
        ssym[',']=commasym;
        ssym['.']=periodsym;
        ssym['<']=lessym;
        ssym['>']=gtrsym;
        ssym[';']=semicolonsym;
    }
}

/* Checks whether a character is a reserved symbol or not.
 * Returns the token_type value if it is a reserved symbol.
 * Returns -1 if it is not a reserved symbol. */
int lexical_check_symbol(char token) {
    int i, ret = -1;

    /* Ensure symbols are initialized */
    lexical_init_symbols();

    /* If token is only one character long, and it's value is valid,
     * then reference the ssym table */
    i = token;
    if(i < ASCIIMAX)
        ret = ssym[(int)token];

    return ret;
}
/* Checks whether the symbol array contains a valid
 * combination. Returns token_type if vaid, or -1 if invalid  */
int lexical_check_multisymbol(char token[]) {

    /* Check for legal multi-symbols */
    if (token[0] == '<' || token[0] == '>' || token[0] == '!') {
        if (token[1] == '=') {

            switch (token[0]) {
                case '<':
                    return leqsym;
                case '>':
                    return geqsym;
                case '!':
                    return neqsym;
            }
        }
    }
    return -1;
}

/* Checks whether a token is a reserved word or not.
 * Returns the value of the reserved word from token_type.
 * Returns -1 if the token is NOT a reserved word. */
int lexical_check_reserved(char token[]) {
    int i;

    /* List of reserved words. Index corresponds with wsym[] */
    char *word[] = {"null", "begin", "call", "const", "do",
                    "end", "if", "procedure", "then",
                    "var", "while", "write"};

    /* List of reserved word id numbers. Index corresponds with *word[] */
    int wsym[] =  { nulsym, beginsym, callsym, constsym, dosym, endsym, ifsym,
                    procsym, thensym, varsym, whilesym, writesym};

    /* Check if the token is a reserved word */
    for(i=0;i<NORW;i++) {
        if(strcmp(token,word[i]) == 0) {
            return wsym[i];
        }
    }

    /* The token is not a reserved word */
    return -1;
}

int lexical_check_whitespace(char token) {
    return (token == ' ' || token == '\n' || token == '\t');
}

/* Returns true if token is an identifer delimiting
 * Symbol */
int lexical_check_delimit(char token) {
    if (lexical_check_symbol(token))
        return 1;

    if (lexical_check_whitespace(token) || token == ':')
        return 1;

    return 0;
}

/* Reads the source from the file. Runs through a state machine
 * looking for tokens. */
int lexical_scanner(FILE *source) {
    int state = 0, sym;
    char cur;
    char curString[CMAX];
    int curIndex = 0, i;
    struct symbol_pos *cur_pos;

    cur = getc(source);
    while (cur != EOF && state != -1) {

        /* Do something */
        switch (state) {

                /* Initial state*/
            case 0:
                if (isalpha(cur)) {
                    curString[curIndex++] = cur;
                    curString[curIndex] = '\0';
                    state = 1;
                } else if (isdigit(cur)) {
                    curString[curIndex++] = cur;
                    curString[curIndex] = '\0';
                    state = 2;
                } else if (cur == '/') {
                    state = 3;
                } else if (lexical_check_whitespace(cur)) {
                    state = 0;
                } else if (cur == ':') {
                    state = 6;
                } else if (lexical_check_symbol(cur) != -1 || cur == '!') {
                    curString[curIndex++] = cur;
                    curString[curIndex] = '\0';
                    state = 7;
                } else {
                    util_err("Unknown symbol!");
                    state = -1;
                }
                break;

            case 1:

                if (curIndex >= CMAX) {
                    util_err("Identifier length overflow.");
                    state = -1;
                } else {
                    if(isalpha(cur) || isdigit(cur)) {
                        curString[curIndex++] = cur;
                        curString[curIndex] = '\0';
                        state = 1;

                    } else if(lexical_check_delimit(cur)) {
                        ungetc(cur, source);

                        /* Reserved word detected */
                        if ((sym = lexical_check_reserved(curString)) != -1) {
                            util_write_token(sym);

                            /* Identifier detected */
                        } else {
                            cur_pos = symbol_exist(curString);

                            if(cur_pos == NULL)
                                cur_pos = symbol_add(curString, idsym);

                            util_write_token(idsym);
                            util_write_token(cur_pos->index);
                            util_write_token(cur_pos->offset);

                            free(cur_pos);
                        }

                        /* Reset the string, reset the state machine */
                        curIndex = 0;
                        curString[curIndex] = '\0';
                        state = 0;
                    }
                }
                break;

            case 2:
                /* Number length check */
                if (curIndex >= NMAX) {
                    util_err("Number length overflow");
                    state = -1;

                    /* Continues to be a number */
                } else if(isdigit(cur)) {
                    curString[curIndex++] = cur;
                    curString[curIndex] = '\0';
                    state = 2;

                    /* If a letter comes, we fail */
                } else if(isalpha(cur)) {
                    util_err("Variable names MUST start with a letter");
                    state = -1;

                    /* If we reach a delimiter, we're done */
                } else if(lexical_check_delimit(cur)) {
                    ungetc(cur,source);

                    /* Write its token type and then value */
                    util_write_token(numbersym);
                    util_write_token(atoi(curString));

                    curIndex = 0;
                    curString[curIndex] = '\0';
                    state = 0;
                }

                break;
            case 3:
                /* A comment has started */
                if(cur == '*')
                    state = 4;

                /* A comment has not started, this is a special symbol */
                else {
                    ungetc(cur,source);
                    ungetc('/',source);
                    state = 7;
                }
                break;
            case 4:
                /* This is possibly the end of a comment */
                if(cur == '*')
                    state = 5;

                /* This is the body of a comment */
                else
                    state = 4;
                break;
            case 5:
                /* The is definitely the end of a comment */
                if(cur == '/')
                    state = 0;

                /* This is possibly the end of the comment */
                else if(cur == '*')
                    state = 5;

                /* This is not the end, the comment body continues */
                else
                    state = 4;
                break;
            case 6:
                /* The becomes symbol is received */
                if(cur == '=') {
                    util_write_token(becomessym);
                    state = 0;

                    /* Should be becomes, but it's not. We fail */
                } else {
                    util_err("Invalid symbol detected");
                    cur = EOF;
                }
                break;
            case 7:
                /* Multi-Symbol */
                if (cur == '=') {

                    /* Add cur to string */
                    curString[curIndex++] = cur;
                    curString[curIndex] = '\0';

                    i = lexical_check_multisymbol(curString);

                    if (i != -1) {
                        util_write_token(i);
                        state = 0;
                    } else {
                        util_err("Invalid Multi-Symbol detected");
                        state = -1;
                    }

                    /* Single Symbol */
                } else {

                    /* Put the second symbol back if not whitespace */
                    if (!lexical_check_whitespace(cur))
                        ungetc(cur, source);

                    /* Check if/what this symbol is */
                    i = lexical_check_symbol(curString[0]);

                    /* Write it if it is valid */
                    if (i != -1) {
                        util_write_token(i);
                        state = 0;

                        /* Fail if the symbol is not valid */
                    } else {
                        util_err("Invalid symbol detected");
                        printf("%c\n",cur);
                        state = -1;
                    }
                }
                curIndex = 0;
                curString[curIndex] = '\0';

                break;
            case 8:

                break;
            case 9:

                break;
        }

        /* We are forcing an EOF to end the loop */
        if(cur != EOF)
            cur = getc(source);
    }

    /* If EOF is received in state 4 or 5, then the comment was never closed */
    if(state == 4 || state == 5) {
        util_err("Open comment detected");
        state = -1;
    }

    if(state == -1)
        return 0;
    else
        return 1;
}

void lexical_finished() {
    free(ssym);
}
