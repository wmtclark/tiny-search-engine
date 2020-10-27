# TSE Indexer Design Spec

## User Interface

The indexer interacts with a user through command line input. It must have two arguments.

```bash
indexer pageDirectory indexFileName
```

For example

```bash
$indexer ../crawler/lettersDirectory lettersIndex
```

pageDirectory is not assumed to end in /

## Inputs and Outputs

Input: the command line inputs are the only direct inputs

Outputs: a file called indexFileName that contains every word from the files in pageDirectory, listed alongside their count on each page ID.

## Functional decomposition into modules

1. *main* which parses the command line and deals with errors

2. *indexer* which is called from main and runs the remaining functions, building the primary hashtable called the index

3. *indexSave* which takes the index structure and writes it to a file.

4. *indexMake* creates a new index structure from a crawler directory

5. *indexDelete* deletes the index and frees any associated memory.

## Pseudo code for logic/algorithmic flow

The indexer will create a structure called index and then fill it as follows:

1. for each page in the directory

    1. for each word in the page

        1. if the word is more than three letters
            1. normalize the word

            2. if there is a counter at the index and id

                1. iterate the counter

            3. if there is not a counter

                1. create a new counters object
                2. insert it into the hashtable

**normalize** means to convert a word to lower case.

## Dataflow through modules

main parses parameters and passes them to indexer.

Indexer uses a hashtable of words (key) and counters (value) to keep track of every word on each document. It calls make to begin and  insert on each item. It calls print to send it to a file.

indexSave calls hashtable_iterate and counters_iterate to print each counter to a file in a specified format.

indexMake creates a hashtable that will be the index, and returns it to Indexer.

indexDelete uses hashtable_delete on the hashtable called index.

Major data structures

Hashtable - used by index to store the actual index
set - used to store elements in a hashtable
counter - used to store the count on each page of the index.

## Testing plan

### Unit testing

Each module will be tested as it is constructed, and checked against valgrind for memory errors.

### Integration testing

Assemble the indexer and test it as a whole using testing.sh

#### First test parameters and other errors to ensure that the program correctly returns errors

Test the program with various forms of incorrect command-line arguments to ensure that its command-line parsing, and validation of those parameters, works correctly.

Test the indexer with a pageDirectory that does not exist, and one that is not a directory.

Test the indexer with a indexFileName is not a valid path name.

#### Second test for correct functionality of the fully functional indexer on crawler files

Test the indexer on a directory with multiple crawler directories.

Test a directory with only a single file

Test a directory with multiple files

The outputs of these files can be compared to the given outputs from the cs50 website in order to ensure that they are correctly indexed
