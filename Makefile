.PHONY: test

all:
	$(CC) -g -O0 prog.c grid.c draw.c compiler.c life.c main.c

test:
	$(CC) -g -O0 prog.c grid.c draw.c compiler.c life.c test.c -o test
	./test
