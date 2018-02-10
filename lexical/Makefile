CC=gcc
LD=gcc
CFLAGS=-O2

lexical: lexical.o symbol.o main.o utilities.o
	$(LD) lexical.o symbol.o main.o utilities.o -o lexical 

lexical.o: lexical.c lexical.h
	$(CC) -c lexical.c $(CFLAGS)

symbol.o: symbol.c symbol.h
	$(CC) -c symbol.c $(CFLAGS)

utilities.o: utilities.c utilities.h
	$(CC) -c utilities.c $(CFLAGS)

main.o: main.c
	$(CC) -c main.c $(CFLAGS)

style: lexical.c lexical.h symbol.c symbol.h main.c utilities.c utilities.h
	astyle -c --style=kr -S lexical.c lexical.h symbol.c symbol.h main.c utilities.c utilities.h

clean:
	rm -fv *.o *.gch
