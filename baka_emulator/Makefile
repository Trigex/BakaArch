all: baka_emulator

CC = gcc
WARNINGS = -Wall
DEBUG = -ggdb -fno-omit-frame-pointer
OPTIMIZE = -O2
FILES = baka_emulator.c emulator.h emulator.c

baka_emulator: Makefile $(FILES)
	$(CC) -o emulator $(WARNINGS) $(DEBUG) $(OPTIMIZE) $(FILES)

clean:
	rm -f baka_emulator

# Builder will call this to install the application before running.
install:
	echo "Installing is not supported"

# Builder uses this target to run your application.
run:
	./baka_emulator
