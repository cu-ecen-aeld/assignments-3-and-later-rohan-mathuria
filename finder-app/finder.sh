#!/bin/sh

FILES_DIR=$1
SEARCH_STR=$2

if [ -z $FILES_DIR ]; then
	echo "Search path not set, specify where to find!"
	exit 1
elif [ ! -d $FILES_DIR ]; then
        echo "Directory to search within does not exist!"
        exit 1
elif [ -z $SEARCH_STR ]; then
	echo "Nothing to search, specify search string!"
	exit 1
fi

MATCHED_FILES=$( grep -irl $SEARCH_STR $FILES_DIR | wc -l )
MATCHING_LINES=$( grep -ir $SEARCH_STR $FILES_DIR | wc -l )
echo "The number of files are $MATCHED_FILES and the number of matching lines are $MATCHING_LINES"

