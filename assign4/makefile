BIN  = nn

CXX=g++
CC=g++
SHELL=/bin/bash

CPPFLAGS= -L /usr/lib/libp/  -pg
CPPFLAGS=-g -Wall
CPPFLAGS=-O3 -Wall
CPPFLAGS=-std=c++11

CFLAGS=$(CPPFLAGS)
LIBS = -lm

SRCS=\
nn.cpp\
nnoneof.cpp\
mat.cpp\
rand.cpp\
randf.cpp\
randmt.cpp

HDRS=\
mat.h\
rand.h

OBJS=\
mat.o\
rand.o

nn:	$(OBJS) nn.o
	$(CC) $(CFLAGS) $(OBJS) nn.o $(LIBS) -o nn

nnoneof:	$(OBJS) nnoneof.o
	$(CC) $(CFLAGS) $(OBJS) nnoneof.o $(LIBS) -o nnoneof

size:	$(HDRS)  $(SRCS) 
	wc -l $?

srcs:	$(HDRS)  $(SRCS) 
	echo $(HDRS)  $(SRCS) 

all:
	touch $(HDRS)  $(SRCS) 

clean:
	/bin/rm -f *.o $(BIN)*.tar *~ core gmon.out a.out

tar: makefile $(SRCS) $(HDRS)
	tar -cvf $(BIN).tar makefile $(SRCS) $(HDRS) $(DOCS)
	ls -l $(BIN).tar
