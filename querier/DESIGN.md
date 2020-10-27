# TSE Querier Design Spec

## User Interface

The querier is run with a command line input. It then receives input from stdin until EOL is read.

Command line:

```bash
querier pageDirectory indexFileName
```

For example

```bash
$indexer .gitignore/lettersDirectory .gitignore/lettersIndex
```

pageDirectory is not assumed to end in /

Users then input to the querier from stdin. Queries are read when the user presses enter and the program is run until the user inputs EOF (Control + D).

For example:

```bash
./querier .gitignore/lettersDirectory .gitignore/lettersIndex
Query? this

Matches 1 documents (ranked):
Score:    1 docID:    1 URL:http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/index.html
-------------------------------------------
Query? not in file

Matches 0 documents (ranked):
-------------------------------------------
Query?
```

## Inputs and Outputs

Input: A directory and indexFile are accepted in command line.
The user is expected to input queries from stdin or a file.

Queries are read one line at a time. All letters are normalized to lowercase. Further, Queries can have two key words: AND or OR. And searches for webpages that contain both the word before and after it. Or searches for all pages that contain either the sequence before or after OR. The AND operator takes precedence. All sequences without and/or are implicitly connected by and. Consecutive and/or statements, or queries that begin or end in and/or are invalid. 

For Example: 

`help and me` returns all pages that contain the words "help" and "me", with preference to the pages that have the most combined hits.

`please or work` returns all pages that contain the word "please" or the word "work", with preference to the pages that contain the most of both.

`help and me or please work` returns all pages that contain ("help" and "me") OR ("please" and "work"). Parentheses used to emphasize that pages will be evaluated first for the AND, then for the OR.

Outputs: a ranked list of results, with pages listed by score and their URL.

## Functional decomposition into modules

1. *main* which parses the command line and deals with errors

2. *parser* which converts an input string into an array of words, known as a query.

3. *querier* which takes the query and the index and the directory and ensures the query is valid before scoring the pages and printing the results

4. *queryValidate* called from querier, and used to ensure that the query follows the syntax rules for a query.

5. *andMerge*/*orMerge* called by querier to combine the results of two words (or subqueries) according to the rules for and/or

6. *resultsPrint* prints out the results of the search.

7. *delete* cleans the memory for the queries, the resulsts, and the index.

## Pseudo code for logic/algorithmic flow

The querier searches through entries as follows

1. Check that all input words are valid (only alphabet and spaces are allowed)

2. Convert the input string into an array of words

3. Scan through the array of words

    1. get the values for pageID and count for a word

    2. merge the word into the current string with and UNLESS the previous word was OR

    3. IF the previous word was or, start a new string that is ORmerged into the result at the end of the input or when the next or is read

4. Create an array of results, based on score

5. Assign each element in the array to the URL associated with that ID

6. Print the results

## Dataflow through modules

main parses parameters it passes a string to parser, and the resulting array of words, alongside an index and the directory, to querier.

Parser takes a string and returns an array of words. Both are allocated and freed in main.

Querier takes an array of words, the index, and the directory. Then it pulls the counterset associated with a word from the index when it appears in the query. It creates a counterset and passes it to andMerge and orMerge. It opens files in the directory to read URLs. It finally cleans up by passing the results to delete.

queryValidate takes a query and does not return anything.

andMerge/orMerge take preallocated countersets, and free both before returning a newly created counter for both.

delete takes in results and frees the strings for the URLs, then deletes the countersets and array associated with the results. 

### Major data structures

Hashtable - used by querie to store the actual index
set - used to store elements in a hashtable
counter - used to store the count on each page of the index.

## Testing plan

### Unit testing

Each module will be tested as it is constructed, and checked against valgrind for memory errors.

### Integration testing

Assemble the querier and test it as a whole using testing.sh

#### First test parameters and other errors to ensure that the program correctly returns errors

Test the program with various forms of incorrect command-line arguments to ensure that its command-line parsing, and validation of those parameters, works correctly.

Test the querier with a Directory that does not exist, and one that is not a crawler directory.

Test the querier with a indexFileName is not a valid path name.

#### Second test for correct functionality of the fully buult querier

Test the command line: Strings that start/end with and/or. Strings that contain consecutive and/or. Strings with invalid characters.

Test the querier on a directory/index 

Test that words with no results return no results. 

Test that a single word returns the correct number of results. 

Test that and correctly unions two words.

Test that or correctly adds two words. 

Test that and/or precedence works.

### Finally test with fuzzquery 

Run fuzzquery over outputs to ensure no errors with large numbers of outputs 