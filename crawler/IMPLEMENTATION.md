# Implementation

## crawler

The crawler.c code implements the crawler module. The crawler module crawls through a set of linked webapges and saves them to a pageDirectory, given in the command line.
The crawler module utilizes a pagedirectory module, a hashtable, and a bag in order to find, track, and save the webpages it crawls through.

The crawler is implemented primarily as a while loop in c. First, the given URL is stored in a `webpage_t` data structure, which contains a depth, URL, and the HTML of a webpage. This `webpage_t` is then passed to a `bag_t` structure, which creates a linked list of items. Then a while loop is created until there are no items left in the `bag_t` structure. This loop first attempts to extract the HTML for a given `webpage_t` structure. If HTML is found, the page is then checked to ensure it is not past the max depth set by the command line. If the page is inside this max depth, it is written into a file in the pageDirectory given by the user by calling `writePage`, a function from the pageDirectory module. This code simply generates the name of the file and writes to the given file. Once written, a page is then scanned for URL's with functions from the *webpage.h* library. A URL is ensured to be of the proper format and inside the "playground" that the crawler was written for by using `IsInternalURL`, a function of the *webpage.h* library. Finally, a URL is checked against a hashtable of already seen URLs. If it has been seen, it is skipped, if it has not been seen, a webpage is generated for the URL and added to the bag.

Each bag_t node is a simple FIFO linked list, where items are added and removed from the head. Items in this case are `webpage_t`

The hashtable is a structure that stores sets of key-value pairs in `set_t` structures. For the purposes of this code, each key is a URL and the value is simply a random character.

## functions

```c
//crawler takes a URL, a directory, and a depth, and prints every linked page
//in that range to the directory.
void crawler (const char *seedURL, char *pageDirectory, int maxDepth);
//url delete is a function that is passed to hashtable_delete.
//it has no function except avoiding a NULL error when hashtable_delete is called
void URLdelete(void *URL);
```

## errors

1 - the command line does not have the three arguments necessary for the crawler program

2- the directory one is attempting to write to does not exist.
