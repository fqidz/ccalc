CC = clang
ARGS = -xc -std=c99 -g -Wall -Wextra -Wimplicit-fallthrough -pedantic -Werror

out:
	mkdir ./out


tokenizer.o: out tokenizer.c tokenizer.h logging.h
	$(CC) $(ARGS) -c tokenizer.c -o out/tokenizer.o

libtokenizer.a: tokenizer.o
	ar rcs out/libtokenizer.a out/tokenizer.o


main.o: out main.c libtokenizer.a
	$(CC) $(ARGS) -c main.c -o out/main.o

tests.o: out tests.c libtokenizer.a logging.h
	$(CC) $(ARGS) -c tests.c -o out/tests.o


main: main.o
	$(CC) -Lout out/main.o -o out/main -ltokenizer

tests: tests.o
	$(CC) -Lout out/tests.o -o out/tests -ltokenizer


.PHONY: run
run: main
	out/main

.PHONY: test
test: tests
	out/tests

.PHONY: run_and_test
run_and_test: main tests
	out/tests 2> /dev/null
	out/main


.PHONY: clean
clean:
	rm -rf out/
