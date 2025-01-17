CC = gcc

CFLAGS=-std=c99 -Wextra -Wall
FILES = main.c utils/*.c emulator/*.c functional/*.c
TEST = utils/*c tests/*.c emulator/*.c
LIBS=-lm

all:
	${CC} ${CFLAGS} -pedantic -o main ${FILES} ${LIBS}
test:
	${CC} ${CFLAGS} -fsanitize=address -pedantic -o test -g ${TEST} ${LIBS}
debug:
	${CC} ${CFLAGS} -fsanitize=address -o dbg -g ${FILES} ${LIBS}
clean:
	rm -f main test dbg
