CC = gcc

CFLAGS=-std=c99 -Wextra -Wall
FILES = main.c utils/*.c
TEST = utils/*c tests/*.c
LIBS=-lm

all:
	${CC} ${CFLAGS} -pedantic -o main ${FILES} ${LIBS}

test:
	${CC} ${CFLAGS} -fsanitize=address -pedantic -o test -g ${TEST} ${LIBS}
debug:
	${CC} ${CFLAGS} -fsanitize=address -g -o  ${FILES} ${LIBS}

clean:
	rm -f main test