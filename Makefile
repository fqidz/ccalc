CC = clang
ARGS = -xc\
	   -std=c99\
	   -g\
	   -Weverything\
	   -Werror\
	   -Wno-used-but-marked-unused\
	   -Wno-padded\
	   -Wno-declaration-after-statement\
	   -Wno-covered-switch-default\
	   -Wno-unsafe-buffer-usage\
	   -Wno-missing-prototypes\

out:
	mkdir ./out


tokenizer.o: out tokenizer.c tokenizer.h
	$(CC) $(ARGS) -c tokenizer.c -o out/tokenizer.o

libtokenizer.a: tokenizer.o
	ar rcs out/libtokenizer.a out/tokenizer.o

parse.o: out parse.c parse.h
	$(CC) $(ARGS) -c parse.c -o out/parse.o

libparse.a: parse.o
	ar rcs out/libparse.a out/parse.o

logging.o: out logging.c logging.h
	$(CC) $(ARGS) -c logging.c -o out/logging.o

liblogging.a: logging.o
	ar rcs out/liblogging.a out/logging.o


main.o: out main.c
	$(CC) $(ARGS) -c main.c -o out/main.o

tests.o: out tests/tests.c
	$(CC) $(ARGS) -c tests/tests.c -o out/tests.o


main: main.o libtokenizer.a libparse.a liblogging.a
	$(CC) -Lout out/main.o -o out/main -ltokenizer -lparse -llogging -lm

tests: tests.o libtokenizer.a libparse.a liblogging.a
	$(CC) -Lout out/tests.o -o out/tests -ltokenizer -lparse -llogging -lm


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
