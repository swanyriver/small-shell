CC=gcc
SOURCES=smallsh.c parsecmd.c prepare.c error.c
EXECUTABLE=smallsh

all:
	$(CC) $(SOURCES) -o $(EXECUTABLE)