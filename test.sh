#!/bin/bash

# Params:
# 1 -> test name
function run_test() {
    python3 lex.py "test/$1/$1.lex" ./base ./src
    make
    ACTUAL_OUT=`build/lexer "test/$1/$1.tst"`
    EXPECTED_OUT=`cat "test/$1/$1.out"`
    
    if [[ $ACTUAL_OUT == $EXPECTED_OUT ]] ; then
        echo ""
        echo "Pass!"
        echo ""
    else
        echo ""
        echo "Fail!"
        echo ""
        echo "Actual:"
        echo $ACTUAL_OUT
        echo ""
        echo "Expected output:"
        echo $EXPECTED_OUT
        echo ""
        exit 1
    fi
}

echo "Running all tests..."
echo ""

run_test "first"
run_test "comments"

echo ""
echo "Done!"

