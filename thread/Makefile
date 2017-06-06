CC=g++
RM=rm -rf
AR=ar

CFLAGS=-g -Wall
LDFLAGS=-lthread -pthread -levent -lhiredis

INCDIR=../third/include
LIBDIR=../third/lib

INCS=-I $(INCDIR)
LIBS=-L $(LIBDIR)

SRCS=$(wildcard *.cpp)
OBJS=$(SRCS:.cpp=.o)

LIBPREX=thread
LIBNAME=lib$(LIBPREX).a

EXEC = main

main: main.o work_thread.o end_point.o
	$(CC) $^ $(CFLAGS) $(LDFLAGS) $(LIBS) -o $@

%.o : %.cpp
	$(CC) -c $< $(CFLAGS) $(INCS) -o $@

clean:
	$(RM) $(OBJS) $(EXEC)


