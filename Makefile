CC = clang
ARGS = -xc -std=c99 -g -Wall -Wextra -pedantic -Werror

out:
	mkdir ./out


calc.o: out calc.c calc.h
	$(CC) $(ARGS) -c calc.c -o out/calc.o

libcalc.a: calc.o
	ar rcs out/libcalc.a out/calc.o


main.o: out main.c libcalc.a
	$(CC) $(ARGS) -c main.c -o out/main.o

tests.o: out tests.c libcalc.a
	$(CC) $(ARGS) -c tests.c -o out/tests.o


main: main.o
	$(CC) -Lout out/main.o -o out/main -lcalc

tests: tests.o
	$(CC) -Lout out/tests.o -o out/tests -lcalc


.PHONY: run
run: main
	out/main

.PHONY: test
test: tests
	out/tests

.PHONY: run_and_test
run_and_test: main tests
	out/tests
	out/main


.PHONY: clean
clean:
	rm -rf out/
