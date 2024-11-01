#! /bin/bash

WRITE_FILE=$1
WRITE_PATH=${1%/*}
WRITE_STR=$2

if [ -z $WRITE_STR ]; then
	echo "Nothing to write!"
	exit 1
elif [ -z $WRITE_FILE ]; then 
        echo "File to write into is not specified!"
        exit 1
elif [ ! -d $WRITE_PATH ]; then
	mkdir -p $WRITE_PATH
fi

echo "$WRITE_STR" > $WRITE_FILE

