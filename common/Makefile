# Makefile for CS50 library
#   Builds and compiles all these utility functions into a library.
#
# If you prefer our data-structure implementation over your own, modify 
# the rule for `$(LIB)`:
# $(LIB): libcs50-given.a
#	cp libcs50-given.a $(LIB)
#
# David Kotz, April 2016, July 2017, April 2019
# Updated by Xia Zhou, July 2016
# Updated by Temi Prioleau, January 2020
# Updated by Thomas Clark, May 2020

# object files, and the target library
LIB = common.a
LLIB = ../libcs50
OBJS = pagedir.o word.o index.o $(LLIB)/libcs50.a

# add -DNOSLEEP to disable the automatic sleep after web-page fetches
#               (students, please do not use -DNOSLEEP!)
# add -DMEMTEST for memory tracking report in indexer
# (and run `make clean; make` whenever you change this)
FLAGS = # -DMEMTEST  # -DNOSLEEP

CFLAGS = -Wall -pedantic -std=c11 -ggdb -lm $(FLAGS) -I$(LLIB)
CC = gcc
MAKE = make

# Build the library by archiving object files
$(LIB): $(OBJS)
	ar cr $(LIB) $(OBJS)

# Dependencies: object files depend on header files
pagedir.o: pagedir.h
word.o: word.h
index.o: index.h ../libcs50/counters.h ../libcs50/file.h ../libcs50/hashtable.h

.PHONY: clean sourcelist

# list all the sources and docs in this directory.
# (this rule is used only by the Professor in preparing the starter kit)
sourcelist: Makefile *.md *.c *.h
	@echo $^

# clean up after our compilation
clean:
	rm -f core
	rm -f $(LIB) *~ *.o
