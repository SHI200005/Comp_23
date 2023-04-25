#!/bin/bash

# Define input data
input_file="waveparams.txt"
expected_output_file="original.dat"

# Run wave1d program
./wave1d $input_file

# Verify output
if ! diff results.dat $expected_output_file; then
    echo "Test failed: output does not match expected output"
    exit 1
fi

# Report results
echo "Test passed!"
