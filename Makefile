CC = clang
ARGS = -std=c99 -g -Wall -Werror
PROGRAM = main

out:
	mkdir ./out

main.o: main.c out
	$(CC) $(ARGS) -c $(PROGRAM).c -o ./out/$(PROGRAM).o

main: main.o
	$(CC) ./out/$(PROGRAM).o -o ./out/$(PROGRAM)

run: main
	./out/$(PROGRAM)

clean:
	rm -rf ./out
