all: baka_assembler

CC = gcc
WARNINGS = -Wall
DEBUG = -ggdb -fno-omit-frame-pointer
OPTIMIZE = -O2
FILES = baka_assembler.c io.h io.c parser.h parser.c

baka_assembler: Makefile $(FILES)
	$(CC) -o assembler $(WARNINGS) $(DEBUG) $(OPTIMIZE) $(FILES)

clean:
	rm -f baka_assembler

# Builder will call this to install the application before running.
install:
	echo "Installing is not supported"

# Builder uses this target to run your application.
run:
	./baka_assembler
