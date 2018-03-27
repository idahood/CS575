BIN  = pca

CXX=g++
CC=g++
SHELL=/bin/bash

CPPFLAGS=-std=c++11

CFLAGS=$(CPPFLAGS)
LIBS = -lm

SRCS=\
pca.cpp\
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

pca:	$(OBJS) pca.o
	$(CC) $(CFLAGS) $(OBJS) pca.o $(LIBS) -o pca

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
