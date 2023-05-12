#!/bin/bash

file_path="$1"

# Compile the C file and capture the output
output=$(gcc -o /dev/null -Wall -Wextra -pedantic -std=c11 "$file_path" 2>&1)

# Extract the number of errors and warnings from the output
num_errors=$(echo "$output" | grep -c "error")
num_warnings=$(echo "$output" | grep -c "warning")

# Print the number of errors and warnings
echo "Errors: $num_errors"
echo "Warnings: $num_warnings"
