all: baka_arch

CC = gcc
WARNINGS = -Wall
DEBUG = -ggdb -fno-omit-frame-pointer
OPTIMIZE = -O2
FILES = baka_arch.c emulator.h emulator.c

baka_arch: Makefile $(FILES)
	$(CC) -o $@ $(WARNINGS) $(DEBUG) $(OPTIMIZE) $(FILES)

clean:
	rm -f baka_arch

# Builder will call this to install the application before running.
install:
	echo "Installing is not supported"

# Builder uses this target to run your application.
run:
	./baka_arch
