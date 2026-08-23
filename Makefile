.PHONY: all test gen

all:
	$(CC) -g -O0 prog.c grid.c draw.c compiler.c life.c load.c main.c

gen:
	$(CC) -g -O0 gen.c -o gen

test:
	$(CC) -g -O0 prog.c grid.c draw.c compiler.c life.c load.c test.c -o test
	./test
