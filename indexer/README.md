# CS50 Lab 5

## Thomas Clark, CS50 5/6/20 2020

### Indexer

Common contains indexer and indextest. Indexer reads a crawler directory into an index, and indextest copies an index from a file into another file.

### Usage

To compile both, `make`

To compile either `make index` or `make indextest`

To run test cases `make test`

To clean, `make clean`

### Assumptions

indexer assumes that it is passed a crawler directory whose filenames are of the form directoryName/id. It will print nothing if the filenames are of a different type. Further, the files in the directoryName must be of the format of a crawler output.

Indextest assumes it is passed a file with the format of indexer, that is a word followed by a list of pairs of integers.
