CC=g++
RM=rm -rf
AR=ar

CFLAGS=-g -Wall
LDFLAGS=-lthread -pthread -levent -lhiredis -lcolib -ldl

INCDIR=../common/include
LIBDIR=../common/lib

INCS=-I $(INCDIR)
LIBS=-L $(LIBDIR)

SRCS=$(wildcard *.cpp)
OBJS=$(SRCS:.cpp=.o)

LIBPREX=thread
LIBNAME=lib$(LIBPREX).a

EXEC = main

main: frame_main.o frame_work.o end_point.o
	$(CC) $^ $(CFLAGS) $(LDFLAGS) $(LIBS) -o $@

%.o : %.cpp
	$(CC) -c $< $(CFLAGS) $(INCS) -o $@

clean:
	$(RM) $(OBJS) $(EXEC)
