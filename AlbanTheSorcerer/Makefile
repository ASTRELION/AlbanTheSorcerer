all: main

main: main.o dungeonManager.o
	gcc main.o dungeonManager.o -o main -lm

main.o: main.c dungeonManager.h
	gcc -c -Wall -Werror -g main.c -o main.o -lm

dungeonManager.o: dungeonManager.c dungeonManager.h
	gcc -c -Wall -Werror -g dungeonManager.c -o dungeonManager.o -lm

clean:
	rm -f main *~
