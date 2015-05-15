#Makefile template from http://mrbook.org/tutorials/make
#Uses sources and executable variables to build a binary
#first by building objects when they need updating
#then by linking them togethr

##----------------------------------------------------------------------
## 20. Demonstrates at least one makefile
##----------------------------------------------------------------------

CC=gcc
#CFLAGS=-c
#LDFLAGS=-lncurses
OS=2>errors.txt
SOURCES=smallsh.c parsecmd.c prepare.c
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=small

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS)
	$(CC)  $(OBJECTS) -o $@ $(LDFLAGS) $(OS)

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

