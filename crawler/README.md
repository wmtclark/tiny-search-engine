# CS50 Lab 4

## Thomas Clark, CS50 5/6/20 2020

### Crawler

Crawler.c contains a web crawler that beginds on a seed URL and will find every link from that page and links from the links of that page, etc. until a given depth of pages. All pages found will be printed to a directory name that is given by the user, and stored with a unique numerical ID. Once the crawler has run, the user will see a directory from the command line with the give

### Usage

command line:

```bash
./crawler seedURL pageDirectory maxDepth
```

Example:

```bash
./crawler http://old-www.cs.dartmouth.edu/~cs50/index.html pageDirectory/ 2
```

### Assumptions

Directory we are passing to exists. Any files in the directory with the same names as written files will be overriden.

### Compilation and Testing

To compile, `make`

To test, `make test`

To clean `make clean`

testing.out shows the output of testing crawler.c. This program checks the functionality of crawler going through different error messages and ensuring that the correct outputs are found for different seeds.
