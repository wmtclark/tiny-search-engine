# Testing script for crawler.c
# Thomas Clark updates from Temi Prioleau
# Date: May 6, 2020
#
# usage: bash -v testing.sh

# Define variables
seedURL=http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/index.html
externalURL=www.google.com
dir=lettersDirectory
#####################################
### These tests should fail ###

# 1 argument
./crawler
echo 
# 2 arguments
./crawler $seedURL
echo 
# 3 arguments
./crawler $seedURL letterDirectory
echo
# 4 arguments + externalURL
./crawler $externalURL letterDirectory 2
echo 
######################################
### These tests should pass ####

# at depth 0
mkdir $dir
./crawler $seedURL lettersDirectory 0
echo "Number of files in directory: "
ls -1 $dir | wc -l
rm -f -r $dir
echo 
mkdir $dir
# at depth 1 
./crawler $seedURL lettersDirectory/ 1
echo "Number of files in directory: "
ls -1 $dir | wc -l
rm -f -r $dir
echo 
mkdir $dir

# at depth 2 
./crawler $seedURL lettersDirectory/ 2
echo "Number of files in directory: "
ls -1 $dir | wc -l
rm -f -r $dir
echo 
mkdir $dir
# at depth 3
./crawler $seedURL lettersDirectory/ 3
echo "Number of files in directory: "
ls -1 $dir | wc -l
rm -f -r $dir
echo 
mkdir $dir
# at depth 4 should be brought down to 3 
./crawler $seedURL lettersDirectory/ 4
echo "Number of files in directory: "
ls -1 $dir | wc -l
rm -f -r $dir
echo 
# at depth 1 wikipedia 
mkdir $dir
./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/ wikiDirectory/ 0
echo "Number of files in directory: "
ls -l wikiDirectory | wc -l 
rm -f -r $dir

