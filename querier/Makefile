# Makefile for 'querier' module
#
# David Kotz, April 2016, 2017
# updated by Xia Zhou, July 2016, 2018, January 2019
# updated by Temi Prioleau, January 2020
# updated by Thomas Clark, Spring 2020

L = ../libcs50
LL = ../common
PROG = querier
OBJS = querier.o 
FUZZ = fuzzquery
FUZZOBJS = fuzzquery.o
LIBS = $(L)/libcs50-given.a
LIBS1 = $(LL)/common.a
# uncomment the following to turn on verbose memory logging
# TESTING=-DMEMTEST

CFLAGS = -Wall -pedantic -std=gnu11 -lm -ggdb $(TESTING) -I$(LL) -I$(L)
CC = gcc
MAKE = make
all: $(PROG) $(FUZZ)

$(PROG): $(OBJS) $(LIBS1) $(LIBS)
	$(CC) $(CFLAGS) $^ -o $@

$(FUZZ): $(FUZZOBJS) $(LIBS1) $(LIBS)
	$(CC) $(CFLAGS) $^ -o $@


.PHONY: test clean 

test: $(PROG) $(FUZZ)
	./testing.sh &> testing.out


clean:
	rm -f *~ *.o *.dSYM
	rm -f $(PROG) $(FUZZ)
	rm -f stocks
	rm -f core
	rm -f testfile
