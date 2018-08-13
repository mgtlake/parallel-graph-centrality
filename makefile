
CC = gcc
CFLAGS = -Wall -std=gnu99
DEBUG = -g
OPT = -O3
TARGETS = priority_queue.o ass1

# Mark the default target to run (otherwise make will select the first target in the file)
.DEFAULT: all clean
# Mark targets as not generating output files (ensure the targets will always run)
.PHONY: all debug clean

all: $(TARGETS)

# A debug target to update flags before cleaning and compiling all targets
debug: CFLAGS += $(DEBUG)
optimise: CFLAGS += $(OPT)
debug: clean $(TARGETS)

priority_queue.o: priority_queue.c priority_queue.h
	$(CC) $(CFLAGS) -c priority_queue.c -o priority_queue.o 

ass1: ass1.c priority_queue.o
	$(CC) $(CFLAGS) priority_queue.o ass1.c -o ass1

# Clean up our directory - remove objects and binaries
clean:
	        rm -f *.o

