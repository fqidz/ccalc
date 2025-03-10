CC = clang
ARGS = -std=c99 -g -Wall -Wextra -pedantic -Werror
# PROGRAM = calc
# HEADERS = calc.h tests.h

out:
	mkdir ./out


calc.o: out calc.c calc.h
	$(CC) $(ARGS) -c calc.c -o out/calc.o

libcalc.a: calc.o
	ar rv out/libcalc.a out/calc.o
	ranlib out/libcalc.a


main.o: out main.c libcalc.a
	$(CC) $(ARGS) -Lout/ main.c -o out/main.o -lcalc

tests.o: out tests.c tests.h libcalc.a
	$(CC) $(ARGS) -Lout/ tests.c -o out/tests.o -lcalc


main: main.o
	$(CC) out/main.o -o out/main -v

tests: tests.o
	$(CC) out/tests.o -o out/tests


run: main
	out/main

test: main
	out/tests

clean:
	rm -rf out/

# calc.o: calc.c out $(HEADERS)
# 	$(CC) $(ARGS) -c $(PROGRAM).c -o ./out/$(PROGRAM).o
#
# tests.o: tests.c out $(HEADERS) calc
# 	$(CC) $(ARGS) -I/home/faidz/Projects/c-calc/out tests.c -o ./out/tests.o -lcalc -v
#
# calc: calc.o
# 	$(CC) ./out/$(PROGRAM).o -o ./out/$(PROGRAM)
#
# tests: tests.o
# 	$(CC) ./out/tests.o -o ./out/tests
#
#
# run_test: tests
# 	./out/tests
#
# run: calc
# 	./out/$(PROGRAM)
#
# test_and_run: run_test run
#
#
# clean:
# 	rm -rf ./out
