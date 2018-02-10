/* Chad Sine
 * Jason Kress
 * COP3402
 * P-machine simulator
 * Compiles on GCC 4.1.2 and Olympus
 */

#include <stdio.h>
#include <stdlib.h>

#define ARRAYLENGTH 30
#define STACKINIT 13

struct instruction {
	int op;
	int l;
	int m;
};
struct ar_stack {
	int *stack;
	int top;
	int stacksize;
};
typedef struct instruction instruction;
typedef struct ar_stack ar_stack;

void p_init();
void p_fetch();
void p_execute();
void growstack(int num);
int popstack();
void p_opr();
void p_import_code();
void p_print_code();
void p_print_program();
void p_print_stack();
int p_base(int num_levels, int base);
void ar_init();
void ar_push(int bp);
int ar_pop();

int instructions;
instruction *code;
int *stack;
int stacksize = STACKINIT;
int bp = 1;
int sp = 0;
int pc;
instruction *ir;
ar_stack ar;
FILE *source_file;
char opcodes[10][4] = {"", "LIT", "OPR", "LOD", "STO", "CAL", "INC", "JMP", "JPC", "WRT"};

int main(int argc, char** argv)
{
	/* Initialize everything */
	p_init();
	
	/* Create the code store */ 
	p_import_code();
	
	/* Print instructions */
	p_print_code();
	p_print_program();
	
	printf("\t\tpc\tbp\tsp\tstack\nInitial values\t%d\t%d\t%d\n\n", pc, bp, sp);
	
	/* Main Cycle */
	do {
		printf("%2d ",pc);
		p_fetch();
		p_execute();
		
		printf("%s %d %d\t%d\t%d\t%d\t", opcodes[ir->op], ir->l, ir->m, pc, bp, sp);
		p_print_stack();
		
	} while (bp != 0 || ir->op != 2 || ir->m != 0);
	
	/* Free resources */
	free(stack);
	free(code);
	fclose(source_file);
	
	return 0;
}

/* Initialize stacks */
void p_init()
{
	stack = calloc(STACKINIT,sizeof(int));
	ar.stack = calloc(STACKINIT,sizeof(int));
	ar.stacksize = STACKINIT;
	ar.top = 0;
}
/* Fetch the next instruction */
void p_fetch()
{
	ir = &code[pc];
	pc += 1;
}
/* Decode and execute current instruction */
void p_execute()
{
	switch (ir->op) {
		case 1:
			/* LIT */
			growstack(1);
			stack[sp] = ir->m;
			break;
			
		case 2:
			/* OPR */
			p_opr();
			break;
			
		case 3:
			/* LOD */
			growstack(1);
			stack[sp] = stack[p_base(ir->l,bp) + ir->m];
			break;
			
		case 4:
			/* STO */
			stack[p_base(ir->l,bp) + ir->m] = stack[sp];
			popstack();
			break;
			
		case 5:
			/* CAL */
			growstack(3);
			stack[sp - 2] = p_base(ir->l,bp);	/* static link */
			stack[sp - 1] = bp;					/* dynamic link */
			stack[sp] = pc;						/* return address */
			sp -= 3;
			bp = sp + 1;
			pc = ir->m;
			
			/* Update AR stack */
			ar_push(bp);
			
			break;
			
		case 6:
			/* INC */
			growstack(ir->m);
			break;
			
		case 7:
			/* JMP */
			pc = ir->m;
			break;
			
		case 8:
			/* JPC */
			if(stack[sp] == 0) {
				pc = ir->m;
				popstack();
			}
			break;
			
		case 9:
			/* WRT */
			printf("%d\n", popstack());
			break;
	}
}
/* Decode OPR instructions */
void p_opr()
{
	switch(ir->m) {
		case 0:
			/* Return Operation */
			sp = bp - 1;
			pc = stack[sp + 3];
			bp = stack[sp + 2];
			ar_pop();
			break;
			
		case 1:
			/* NEG */
			stack[sp] = -stack[sp];
			break;
			
		case 2:
			/* ADD */
			stack[sp - 1] = stack[sp - 1] + stack[sp];
			popstack();
			break;
			
		case 3:
			/* SUB */
			stack[sp - 1] = stack[sp - 1] - stack[sp];
			popstack();
			break;
			
		case 4:
			/* MUL */
			stack[sp - 1] = stack[sp - 1] * stack[sp];
			popstack();
			break;
			
		case 5:
			/* DIV */
			stack[sp - 1] = stack[sp - 1] / stack[sp];
			popstack();
			break;
			
		case 6:
			/* ODD */
			stack[sp] = stack[sp] % 2;
			break;
			
		case 7:
			/* MOD */
			stack[sp - 1] = stack[sp - 1] % stack[sp];
			popstack();
			break;
			
		case 8:
			/* EQL */
			stack[sp - 1] = stack[sp - 1] == stack[sp];
			popstack();
			break;
			
		case 9:
			/* NEQ */
			stack[sp - 1] = stack[sp - 1] != stack[sp];
			popstack();
			break;
			
		case 10:
			/* LSS */
			stack[sp - 1] = (stack[sp - 1] < stack[sp]);
			popstack();
			break;
			
		case 11:
			/* LEQ */
			stack[sp - 1] = (stack[sp - 1] <= stack[sp]);
			popstack();
			break;
			
		case 12:
			/* GTR */
			stack[sp - 1] = (stack[sp - 1] > stack[sp]);
			popstack();
			break;
			
		case 13:
			/* GEQ */
			stack[sp - 1] = (stack[sp - 1] >= stack[sp]);
			popstack();
			break;
	}
}

/* Fetch instructions from file */
void p_import_code()
{
	char filename[ARRAYLENGTH];
	int ch;
	int i;
	instructions = 0;
	
	/* Get filename and open it */
	do {
		printf("Please enter the name of the file: ");
		scanf( "%s", filename);
		source_file = fopen(filename, "r");
		
		if (source_file == NULL)
			printf("Invalid file name!\n\n");
			
	} while (source_file == NULL);
	printf("\n");
	
	/* Count number of instructions in file */
	while ((ch = fgetc(source_file)) != EOF )
		if ( ch == '\n' )
			++instructions;
	rewind(source_file);
	
	/* Allocate memory for instructions */
	code = (instruction*) malloc(sizeof(instruction) * instructions);
	
	/* Import instructions */
	for (i = 0; i < instructions; i++)
		fscanf(source_file, "%d %d %d", &code[i].op, &code[i].l, &code[i].m);
}
/* Print instructions */
void p_print_code()
{
	int i;
	for (i = 0; i < instructions; i++)
		printf("%d %d %d\n", code[i].op, code[i].l, code[i].m);
	printf("\n");
	
}
/* Print instructions with OPR codes */
void p_print_program()
{
	int i;
	for (i = 0; i < instructions; i++)
		printf("%d %s %d %d\n", i, opcodes[code[i].op], code[i].l, code[i].m);
	printf("\n");
}
/* Print stack */
void p_print_stack()
{
	int i, arindex;
	int mysp = sp;
	
	/* Print extra 3 stack values if CAL is current instruction */
	if (ir->op == 5)
		mysp += 3;
	
	/* Flag if ar.stack is empty */
	if (ar.top >= 1)
		arindex = 0;
	else
		arindex = -1;
		
	/* Print stack, separating ARs */
	for (i = 1; i <= mysp; i++) {
		if (i == ar.stack[arindex]) {
			printf("| ");
			
			if (arindex < ar.top - 1)
				++arindex;
		}
		printf("%-2d ",stack[i]);
	}
	printf("\n");
}

/* Checks the stack to make sure it can handle 'num' more items
 * if not, then it realloc's the stack */
void growstack(int num)
{
	/* If stack will be overflowed, make it larger */
	if (sp + num > stacksize - 1) {
		
		/* Double the stacksize until it is large enough */
		do {
			stacksize *= 2;
		} while (sp + num > stacksize - 1);
		
		stack = (int*) realloc(stack, stacksize * sizeof(int));
	}
	
	/* Increate the stack pointer */
	sp += num;
}

/* Pops the top item off the stack and return it */
int popstack()
{
	int temp;
	
	if (sp >= 1) {
		
		/* Pop the stack */
		temp = stack[sp];
		--sp;
		
		/* Shrink stack if needed */
		if (sp < (stacksize - 1) / 2) {
			stacksize /= 2;
			stack = (int*) realloc(stack, stacksize * sizeof(int));
		}
		
		return temp;
		
	} else
		return stack[sp];
}

/* Find base pointer num_levels levels down  */
int p_base(int num_levels, int base)
{
	while(num_levels > 0) {
		base = stack[base];
		num_levels--;
	}
	return base;
}

/* Record bp in AR stack */
void ar_push(int bp)
{
	/* If stack will be overflowed, make it larger */
	if (ar.top >= ar.stacksize - 1) {
		
		/* Double the stacksize until it is large enough */
		do {
			ar.stacksize *= 2;
		} while (ar.top >= ar.stacksize - 1);
		
		ar.stack = (int*) realloc(ar.stack, ar.stacksize * sizeof(int));
	}
	
	/* Increate the stack pointer */
	ar.stack[ar.top] = bp;
	ar.top++;
}

/* Pop top AR value in stack */
int ar_pop()
{
	int temp;
	
	/* Pop the stack */
	temp = ar.stack[ar.top - 1];
	--ar.top;
		
	/* Shrink stack if needed */
	if (ar.top < (ar.stacksize - 1) / 2) {
		ar.stacksize /= 2;
		ar.stack = (int*) realloc(ar.stack, ar.stacksize * sizeof(int));
	}
		
	return temp;
}

