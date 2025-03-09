CC = clang
ARGS = -std=c99 -g -Wall -Werror
PROGRAM = calc
HEADERS = calc.h tests.h

out:
	mkdir ./out

calc.o: calc.c out $(HEADERS)
	$(CC) $(ARGS) -c $(PROGRAM).c -o ./out/$(PROGRAM).o

tests.o: tests.c out $(HEADERS) calc
	$(CC) $(ARGS) -I/home/faidz/Projects/c-calc/out tests.c -o ./out/tests.o -lcalc -v

calc: calc.o
	$(CC) ./out/$(PROGRAM).o -o ./out/$(PROGRAM)

tests: tests.o
	$(CC) ./out/tests.o -o ./out/tests


run_test: tests
	./out/tests

run: calc
	./out/$(PROGRAM)

test_and_run: run_test run


clean:
	rm -rf ./out
