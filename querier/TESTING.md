# CS50 TSE querier

## Thomas Clark, CS50 Spring 2020

### Testing 'querier'

To test the *querier* I wrote a test program, testing.sh, to explore the capabilities of the program and ensure it properly catches errors.

The testing file first runs through improper command line usage and improper directories

### Test run

An example test run is in `testing.out`, built with

./testing.sh &> testing.out

In the file, one can see the ouptut of the testing commands, found in testing.sh

Fuzztesting can also be used by piping the output of the fuzzquery into the querier program. For example

```bash
 ./fuzzquery .gitignore/tse-output/wikipedia-index-1 100 1 | ./querier .gitignore/tse-output/wikipedia-depth-1 .gitignore/tse-output/wikipedia-index-1
 ```

### Limitations

The program is unable to run on improperly formatted input files and directories.
