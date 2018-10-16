
CC = mpicc
CFLAGS = -Wall -std=gnu99 -O3 -pg -fopenmp -g
DEBUG = -g
TARGETS = helper.o io.o main

# Mark the default target to run (otherwise make will select the first target in the file)
.DEFAULT: all clean
# Mark targets as not generating output files (ensure the targets will always run)
.PHONY: all debug clean

all: $(TARGETS)

# A debug target to update flags before cleaning and compiling all targets
debug: CFLAGS += $(DEBUG)
debug: clean $(TARGETS)

helper.o: helper.c helper.h
	$(CC) $(CFLAGS) -c helper.c -o helper.o 

io.o: io.c io.h
	$(CC) $(CFLAGS) -c io.c -o io.o 

main: main.c helper.o io.o
	$(CC) $(CFLAGS) helper.o io.o main.c -o centrality-solver

# Clean up our directory - remove objects and binaries
clean:
	        rm -f *.o

