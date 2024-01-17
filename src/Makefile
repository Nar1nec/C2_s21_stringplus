OS:=$(shell uname)

ifeq ($(OS), Linux)
D_ENV = __LINUX__
OPEN_CMD = xdg-open
endif
ifeq ($(OS), Darwin)
D_ENV = __MAC_OS__
OPEN_CMD = open
endif

CC = gcc
CFLAGS = -Wall -Werror -Wextra -std=c11 -D$(D_ENV)
SOURCES = $(wildcard s21_*.c)
OBJECTS = $(SOURCES:.c=.o)

## tests
TESTS_SOURSES = $(wildcard ./tests/*.check)
LDFLAGS = -O2 -Wall -Wextra -Werror -std=c11
CFLAGS_TEST = $(CFLAGS) $(shell pkg-config --cflags check)
CLIBS_TEST = -lcheck -lm $(shell pkg-config --libs check)
CFLAGS_GCOV = -fprofile-arcs --coverage

all: s21_string.a 

rebuild: clean all 

s21_string.a: $(OBJECTS) s21_string.h
	ar rcs $@ $(OBJECTS)
	ranlib -D $@

lib_cov.a: $(SOURCES) s21_string.h
	$(CC) $(CFLAGS) $(CFLAGS_GCOV) -c  $(SOURCES)
	ar rc lib_cov.a $(OBJECTS)
	ranlib lib_cov.a

test: clean $(TESTS_SOURSES) s21_string.a
	checkmk clean_mode=1 $(TESTS_SOURSES) > test.c
	$(CC) test.c $(CFLAGS_TEST) $(CLIBS_TEST) s21_string.a -o test -lm
	./test

test_valgrind:
	valgrind --trace-children=yes --track-fds=yes --track-origins=yes --leak-check=full --show-leak-kinds=all ./test

gcov_report: clean $(TESTS_SOURSES) lib_cov.a
	checkmk clean_mode=1 $(TESTS_SOURSES) > test.c
	$(CC)  test.c -c
	$(CC)  test.o -g $(CFLAGS_TEST) lib_cov.a -o test $(CLIBS_TEST) $(CFLAGS_GCOV)
	./test > /dev/null
	lcov -t "t" -o test.info -c -d . 
	genhtml -o report test.info 
	$(OPEN_CMD) ./report/index.html
	rm -f test.c

style_check:
	cp ../materials/linters/.clang-format .
	clang-format -n *.c *.h
	rm .clang-format

style:
	cp ../materials/linters/.clang-format .
	clang-format -i *.h *.c
	rm .clang-format

debug_build: clean debug_add_flags s21_string.a

debug_add_flags: 
	$(eval$(CFLAGS) += -g)

clean: 
	rm -rf *.o *.a *.test *test.c *.gcda *.gcno *.info test.c test report *.out tg_test
