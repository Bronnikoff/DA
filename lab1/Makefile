all: run

run: vector.o main.o
	gcc -std=c99 -w -pipe -O3 -Wextra -Werror -Wall -Wno-sign-compare -pedantic -lm vector.o main.o -o run
	rm *.o

main.o: main.c
	gcc -std=c99 -c main.c

vector.o: vector.c
	gcc -std=c99 -c vector.c

clean:
	rm -rf hello
