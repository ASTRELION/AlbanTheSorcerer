all: main

main: main.o dungeonManager.o heap.o gradientMap.o gameLoop.o
	gcc main.o dungeonManager.o heap.o gradientMap.o gameLoop.o -o main -lm

main.o: main.c dungeonManager.h
	gcc -c -Wall -Werror -g main.c -o main.o -lm

dungeonManager.o: dungeonManager.c dungeonManager.h
	gcc -c -Wall -Werror -g dungeonManager.c -o dungeonManager.o -lm

heap.o: heap.c heap.h
	gcc -c -Wall -Werror -g heap.c -o heap.o -lm

gradientMap.o: gradientMap.c gradientMap.h
	gcc -c -Wall -Werror -g gradientMap.c -o gradientMap.o -lm

gameLoop.o: gameLoop.c gameLoop.h
	gcc -c -Wall -Werror -g gameLoop.c -o gameLoop.o -lm

clean:
	rm -f main *~ *.o
