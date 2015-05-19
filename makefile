#Makefile template from http://mrbook.org/tutorials/make
#Uses sources and executable variables to build a binary
#first by building objects when they need updating
#then by linking them togethr

CC=gcc
CFLAGS=-c
#LDFLAGS=-lncurses
SOURCES=smallsh.c parsecmd.c prepare.c error.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=smallsh

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS)
	$(CC) -g $(OBJECTS) -o $@ $(LDFLAGS)

.c.o:
	$(CC) -g $(CFLAGS) $< -o $@

clean:
	rm $(OBJECTS) $(EXECUTABLE)