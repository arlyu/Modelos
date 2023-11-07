CC = gcc
CFLAGS = -O1 -lm

.PHONY: clean
.PHONY: r

all: a

a: main.c
	$(CC) *.c $(CFLAGS) 
	./a.out

r: a.out
	./a.out

clean:
	rm a.out
