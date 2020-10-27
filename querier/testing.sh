# Testing script for querier.c
# Thomas Clark updated from Temi Prioleau
# Date: May 24, 2020
#
# usage: bash -v testing.sh

# Define variables 
DIR=".gitignore/tse-output/wikipedia-depth-1"
INDEX=".gitignore/tse-output/wikipedia-index-1"
#####################################
### These tests should fail ###
echo "querier" 
echo "These should fail"
# 1 argument
./querier
echo $?

# 2 arguments
./querier dir
echo $?

# invalid directory
./querier notADirectory realFileName
echo $?

# not a crawler directory
mkdir fakeDirectory
./querier fakeDirectory realFileName
echo $?
rm -f -r fakeDirectory
echo 

######################################
### These tests should pass ####

echo "These should pass"

echo "./querier $DIR $INDEX"
./querier $DIR $INDEX < .gitignore/testQueries
printf "\n\n\n\nfuzztesting!!!!\n\n\n\n"
./fuzzquery .gitignore/tse-output/wikipedia-index-1 20 1 > testfile
./querier .gitignore/tse-output/wikipedia-depth-1 .gitignore/tse-output/wikipedia-index-1 < testfile

printf "\n\nEnd of Test Script"
make clean &> /dev/null