# Makefile for 'indexer' module
#
# David Kotz, April 2016, 2017
# updated by Xia Zhou, July 2016, 2018, January 2019
# updated by Temi Prioleau, January 2020
# updated by Thomas Clark, Spring 2020

L = ../libcs50
LL = ../common
PROG = indexer
TEST = indextest
OBJS = indexer.o 
TESTOBJS = indextest.o
LIBS = $(L)/libcs50-given.a
LIBS1 = $(LL)/common.a
# uncomment the following to turn on verbose memory logging
# TESTING=-DMEMTEST

CFLAGS = -Wall -pedantic -std=c11 -lm -ggdb $(TESTING) -I$(LL) -I$(L)
CC = gcc
MAKE = make
all: $(PROG) $(TEST)

$(PROG): $(OBJS) $(LIBS1) $(LIBS)
	$(CC) $(CFLAGS) $^ -o $@


$(TEST): $(TESTOBJS) $(LIBS1) $(LIBS)
	$(CC) $(CFLAGS) $^ -o $@

.PHONY: all test clean 

test: $(PROG) $(TEST)
	./testing.sh &> testing.out


clean:
	rm -f *~ *.o *.dSYM
	rm -f $(PROG)
	rm -f $(TEST)
	rm -f stocks
	rm -f core
	rm -f letters* wiki*
