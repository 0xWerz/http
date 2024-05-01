#!/bin/bash

# Set the compiler and flags based on the platform
if [[ "$OSTYPE" == "linux-gnu"* ]]; then
    CC=gcc
    CFLAGS="-Wall -Werror"
elif [[ "$OSTYPE" == "darwin"* ]]; then
    CC=clang
    CFLAGS="-Wall -Werror"
elif [[ "$OSTYPE" == "msys" || "$OSTYPE" == "cygwin" ]]; then
    CC=gcc
    CFLAGS="-Wall -Werror"
else
    echo "Unsupported platform"
    exit 1
fi

# Set the source file and output executable name
SOURCE_FILES="main.c server.c router.c"
OUTPUT_FILE="bin/http_server"

# Compile the code
$CC $CFLAGS $SOURCE_FILES -o $OUTPUT_FILE