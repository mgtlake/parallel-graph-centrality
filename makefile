
CC = gcc
CFLAGS = -Wall -std=gnu99
DEBUG = -g
OPT = -O3
TARGETS = helper.o io.o ass1

# Mark the default target to run (otherwise make will select the first target in the file)
.DEFAULT: all clean
# Mark targets as not generating output files (ensure the targets will always run)
.PHONY: all debug clean

all: $(TARGETS)

# A debug target to update flags before cleaning and compiling all targets
debug: CFLAGS += $(DEBUG)
optimise: CFLAGS += $(OPT)
debug: clean $(TARGETS)

helper.o: helper.c helper.h
	$(CC) $(CFLAGS) -c helper.c -o helper.o 

io.o: io.c io.h
	$(CC) $(CFLAGS) -c io.c -o io.o 

ass1: ass1.c helper.o io.o
	$(CC) $(CFLAGS) helper.o io.o ass1.c -o ass1

# Clean up our directory - remove objects and binaries
clean:
	        rm -f *.o

