#!/bin/bash

CONVERTER=../debug-build/shadowdash
TEST_DIR=testing/

# Build converter
cd ..
/bin/bash build.sh
cd converter

# Run tests
# loop through each test build.ninja file
for input_file in "$TEST_DIR"/*; do
    # extract the base filename (e.g., "test1" from "test_inputs/test1.txt")
    base_name=$(basename "$input_file")

    # run the binary, passing the input file and directing output to the output directory
    $CONVERTER -f "$input_file" > output.cc

    # check if the binary exited successfully
    if [ $? -ne 0 ]; then
        echo "test failed for $base_name: non-zero exit code"
        continue
    fi

    # check if compilation works of the converter generated output.cc
    clang++ -shared -fPIC -o libmanifest.so -I include/ output.cc
    # check if the binary exited successfully
    if [ $? -ne 0 ]; then
        echo "test failed for $base_name: unable to compile it"
        continue
    fi
    echo "test passed for $base_name"

    # cleanup
    rm -f output.cc libmanifest.so
done
