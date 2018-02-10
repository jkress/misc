/**
 * main.c
 *
 * Lexical Analyzer
 * 
 * COP3402
 * Jason Kress - jason@micrux.net
 * Chad Sine - chad@unixenthusiast.org
 * Last updated October 14, 2007
 */

#include <stdio.h>
#include <stdlib.h>
#include "lexical.h"
#include "utilities.h"

int main(int argc, char *argv[]) {

    FILE *source;
    source = util_open_file(argc, argv);

    if (source == NULL)
        return EXIT_FAILURE;

    util_print_banner(argv[1]);
    if (!lexical_scanner(source)) {
        util_err("Lexical Analysis Failure, cannot continue");
        return EXIT_FAILURE;
    }

    util_print_screen();
    util_print_symbolic();
    util_lexeme_token_attribute();
    util_print_file("lexical.out");
    util_log("Token stream written to 'lexical.out'");
    util_log("Analysis Complete");

    lexical_finished();
    symbol_finished();
    util_finished();
    fclose(source);


    return EXIT_SUCCESS;
}
