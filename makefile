CC = gcc
OBJS = main.o game.o solver.o parser.o doubleLinkedList.o printUser.o gurobi.o mainAux.o stack.o command.o
EXEC = sudoku
COMP_FLAG = -ansi -O3 -Wall -Wextra -Werror -pedantic-errors
GUROBI_COMP = -I/usr/local/lib/gurobi563/include
GUROBI_LIB = -L/usr/local/lib/gurobi563/lib -lgurobi56

$(EXEC): $(OBJS)
	$(CC) $(OBJS) $(GUROBI_LIB) -o $@ -lm
main.o: main.c game.h parser.h
	$(CC) $(COMP_FLAG) -c $*.c
game.o: game.c game.h doubleLinkedList.h solver.h mainAux.h
		$(CC) $(COMP_FLAG) -c $*.c
mainAux.o: mainAux.c mainAux.h game.h gurobi.h stack.h
		$(CC) $(COMP_FLAG) -c $*.c
parser.o: parser.c parser.h game.h command.h
		$(CC) $(COMP_FLAG) -c $*.c
command.o: command.c command.h game.h solver.h gurobi.h stack.h
		$(CC) $(COMP_FLAG) -c $*.c
doubleLinkedList.o: doubleLinkedList.c doubleLinkedList.h
		$(CC) $(COMP_FLAG) -c $*.c
printUser.o : printUser.c printUser.h
		$(CC) $(COMP_FLAG) -c $*.c
solver.o : solver.c solver.h game.h  mainAux.h
		$(CC) $(COMP_FLAG) -c $*.c
stack.o: stack.c stack.h
		$(CC) $(COMP_FLAG) -c $*.c
gurobi.o: gurobi.c gurobi.h gurobi_c.h game.h solver.h
	$(CC) $(COMP_FLAG) $(GUROBI_COMP) -c $*.c


all: $(EXEC)


clean:
	rm -f $(OBJS) $(EXEC)
