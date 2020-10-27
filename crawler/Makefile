# Makefile for 'bag' module
#
# David Kotz, April 2016, 2017
# updated by Xia Zhou, July 2016, 2018, January 2019
# updated by Temi Prioleau, January 2020

L = ../libcs50
LL = ../common
PROG = crawler
OBJS = crawler.o
LIBS = $(L)/libcs50-given.a
LIBS1 = $(LL)/common.a
# uncomment the following to turn on verbose memory logging
# TESTING=-DMEMTEST

CFLAGS = -Wall -pedantic -std=c11 -ggdb $(TESTING) -lm -I$(L) -I$(LL)
CC = gcc
MAKE = make

$(PROG): $(OBJS) $(LIBS) $(LIBS1)
	$(CC) $(CFLAGS) $^ -o $@

crawler.o: crawler.c


.PHONY: test clean

test: ./testing.sh
	  ./testing.sh &> testing.out



clean:
	rm -f *~ *.o *.dSYM
	rm -f $(PROG)
	rm -f stocks
	rm -f core

