#!/bin/bash

# Define input data
input_file="precisewave.nc"
output_file="energies_test.tsv"
expected_output_file="energies.tsv"

# Run wave1d program
mpirun -n 4 ./mpianalyzewave $input_file $output_file

# Verify output
if ! diff $output_file $expected_output_file; then
    echo "Test failed: output does not match expected output"
    exit 1
fi

# Report results
echo "Test passed!"
