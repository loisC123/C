all: main run

###
### maze
###

main: main.c polynomial.o polynomial.h
	$(CC) $(CCFLAGS) -o main main.c polynomial.o $(LFLAGS) $(LIBS)

run: main
	./main

###
###
###

EXPORTS	   =
EXPORTDIR  =
OBJS	   =
LIB_OUT    =

include ../../../makefile.inc

HEADERS = $(EXPORTS)
OBJECTS = $(OBJS)

