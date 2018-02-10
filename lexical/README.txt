Lexical Analyzer
----------------

Authors
-------

Jason Kress - jason@micrux.net 
            - j1528054
Chad Sine   - chad@unixenthusiast.org
            - c2197902
October 14, 2007

If you have any questions or comments, please feel free to contact either of us.

Description
-----------

This program is a lexical analyzer designed for PL/0 compliant programs.

Building Lexical
----------------

  -Unzip lexical.zip
  -Move into the lexical folder
  -Type 'make' to build the program
  -A 'lexical' executible will be created
  -In case make is not available, gcc can be used instead
  -gcc lexical.c symbol.c main.c utilities.c -o lexical

Using Lexical
-------------

  -USAGE: ./lexical <input file>
  -The program will automatically create a token file named lexical.out

Notes
-----

  -This program implements the symbol table as a hash table. So, any
   references to the symbol table have two values instead of just one.
   The first value is the index to the hash table, and the second value
   is the offset in case there are multiple entries at the same index.
   (E.g. 2 8 0 in the symbol table represents an identifier, located at
   index 8, offset 0 in the symbol table.)

  -The following is the list of reserved words that this program handles:
   null, begin, call, const, do, end, if, procedure, then, var, while, write

  -The following is the list of special symbols that this program handles:
     +  PLUS
     -  MINUS
     *  MULTIPLICATION
     /  DIVISION
     =  EQUAL TO
     != NOT EQUAL TO
     <  LESS THAN
     <= LESS THAN OR EQUAL TO
     >  GREATER THAN
     >= GREATER THAN OR EQUAL TO
     (  LEFT PARENTHESIS
     )  RIGHT PARENTHESIS
     ,  COMMA
     ;  SEMICOLON
     .  PERIOD
     := BECOMES
