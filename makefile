all: main

main: shell.o
	gcc shell.o -o main

shell.o: shell.c
	gcc -c -Wall -pedantic shell.c

clean:
	rm *.o main
