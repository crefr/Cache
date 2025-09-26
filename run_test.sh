#!/bin/bash

# Script for running tests
# Usage: ./run_test.sh test_program test.dat

if [ $# -ne 2 ]; then
    echo "Error: Required 2 arguments" >&2
    echo "Usage: $0 test_program test.dat" >&2
    exit 1
fi

TESTER="$1"
TEST="$2"

if [ ! -f "$TESTER" ]; then
    echo "Error: Test program '$TESTER' not found" >&2
    exit 1
fi

test_log_file="${TEST%.*}.log"
test_ans_file="${TEST%.*}.ans"

if [ ! -f "$TEST" ]; then
    echo "Error: Test '$TEST' not found" >&2
    exit 1
fi

if [ ! -f "$test_ans_file" ]; then
    echo "Warn: Test answer '$test_ans_file' not found, making new" >&2
    "$TESTER" < "$TEST" > "$test_ans_file"
    exit 0
fi

"$TESTER" < "$TEST" > "$test_log_file"

if diff -q "$test_log_file" "$test_ans_file" > /dev/null; then
    echo "PASS: $TEST"
    rm $test_log_file
    exit 0
fi

echo "FAIL: $TEST (differences found)"
diff -u "$test_ans_file" "$test_log_file"
exit 1
