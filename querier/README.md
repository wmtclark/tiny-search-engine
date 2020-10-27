# CS50 Lab 6

## Thomas Clark, CS50 5/6/20 2020

### Querier

Querier contains querier, a program used to search for words in a set of webpages. These reuslts are returned as scores to a user.

### Usage

command line: `./querier pageDirectory IndexFileName`

To compile, `make` or `make querier`

To run test cases `make test`

To clean, `make clean`

### Assumptions

Quierier takes two input, an indexFile and a directory.

It assumes that the indexFile is in the format of an index file, as outlined in the index directory. Generally, this assumes that the indexFile was created by index.c from the index directory.

The Directory is assumed to be a crawler directory. Each file is named Directory/ID with ID ranging from 1 until the number of files. Additionally, the Directory is assumed to align with the index, that each pageID in the index corresponds to one in Directory. Finally the first line in each file should be the URL of a webapage associated with that ID.
