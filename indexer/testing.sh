# Testing script for index.c
# Thomas Clark updated from Temi Prioleau
# Date: May 13, 2020
#
# usage: bash -v testing.sh

# Define variables 
# no vars
#####################################
### These tests should fail ###
echo "indexer" 
echo "These should fail"
# 1 argument
./indexer
echo $?

# 2 arguments
./indexer ../crawler/lettersDirectory
echo $?

# invalid directory
./indexer lettersDirectory realFileName
echo $?

# not a crawler directory
mkdir fakeDirectory
./indexer fakeDirectory realFileName
echo $?
rm -f -r fakeDirectory
echo 

echo "indexTest" 
echo "These should fail"

# 1 argument
./indextest
echo $?

# 2 arguments
./indextest lettersIndex
echo $?

# invalid file
./indextest fakefile lettersIndexTest
echo $? 
echo 



######################################
### These tests should pass ####

echo "These should pass"

echo "indexer" 

#letters directory contains multiple files 
echo "./indexer ../crawler/lettersDirectory lettersIndex"
./indexer ../crawler/lettersDirectory lettersIndex
cat lettersIndex 
echo 

#wikidirectory only contains one file 
echo "./indexer ../crawler/wikiDirectory wikiIndex"
./indexer ../crawler/wikiDirectory wikiIndex
cat wikiIndex
echo

echo "indextest"
#indextest copies a file 
echo "./indextest lettersIndex lettersIndexTest"
./indextest lettersIndex lettersIndexTest
#sort, then compare the indexes 
gawk -f indexsort.awk lettersIndex > lettersIndex.sorted
gawk -f indexsort.awk lettersIndexTest > lettersIndexTest.sorted
echo "testing differences"
diff lettersIndex.sorted lettersIndexTest.sorted
echo "completed testing differences"
printf "\n\n"

#indextest copies a file 
echo "./indextest wikiIndex  wikiIndexTest"
./indextest wikiIndex  wikiIndexTest
#sort, then compare the indexes 
gawk -f indexsort.awk wikiIndex > wikiIndex.sorted
gawk -f indexsort.awk wikiIndexTest > wikiIndexTest.sorted
echo "testing differences"
diff wikiIndex.sorted wikiIndexTest.sorted
echo "completed testing differences"

make clean &> /dev/null